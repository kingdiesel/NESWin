#include "PPU.h"
#include "Source/NESConsole.h"
#include "PatternTable.h"
#include "Source/PatternTableTile.h"
#include <cassert>
#include "Palette.h"
#include <iostream>

PPU::PPU()
{
	m_frame_buffer_data = new uint32_t[256 * 240];
	memset(m_frame_buffer_data, 0x00, 256 * 240 * sizeof(uint32_t));
}

void PPU::PowerUp()
{
	// https://wiki.nesdev.com/w/index.php/PPU_power_up_state
	m_reg_ppu_control.Register = 0;
	m_reg_ppu_mask.Register = 0;
	m_reg_status.Register = 0xA0;
	m_write_toggle = 0;
	m_even_frame = true;
	m_current_vram.Register = 0;
	m_temp_vram.Register = 0;
}

void PPU::Reset()
{
	// https://wiki.nesdev.com/w/index.php/PPU_power_up_state
}

bool PPU::IsRenderingEnabled() const
{
	return GetMaskRegister().Bits.m_show_bkgrnd || GetMaskRegister().Bits.m_show_sprites;
}

void PPU::Run()
{
	Memory& memory = NESConsole::GetInstance()->GetMemory();

	// ==============================================================================
	// Every cycle the shifters storing pattern and attribute information shift
	// their contents by 1 bit. This is because every cycle, the output progresses
	// by 1 pixel. This means relatively, the state of the shifter is in sync
	// with the pixels being drawn for that 8 pixel section of the scanline.
	auto UpdateShifters = [&]()
	{
		if (GetMaskRegister().Bits.m_show_bkgrnd == 1)
		{
			// Shifting background tile pattern row
			m_bg_shifter_pattern_lo <<= 1;
			m_bg_shifter_pattern_hi <<= 1;

			// Shifting palette attributes by 1
			m_bg_shifter_attrib_lo <<= 1;
			m_bg_shifter_attrib_hi <<= 1;
		}

		if (GetMaskRegister().Bits.m_show_sprites == 1 && cycles >= 1 && cycles < 258)
		{
			uint8_t* secondary_oam = memory.GetSeconaryOAM();
			for (int i = 0; i < m_active_sprites; i++)
			{
				assert(((i * 4) + 3) < 64);
				uint8_t byte3 = secondary_oam[(i * 4) + 3];
				if (byte3 > 0)
				{
					secondary_oam[(i * 4) + 3]--;
				}
				else
				{
					m_shifter_pattern_sprite_low[i] <<= 1;
					m_shifter_pattern_sprite_high[i] <<= 1;
				}
			}
		}
	};

	if (scanlines >= -1 && scanlines < 240)
	{
		// https://wiki.nesdev.com/w/index.php/PPU_frame_timing#Even.2FOdd_Frames
		// With rendering enabled, each odd PPU frame is one PPU clock shorter than 
		// normal. This is done by skipping the first idle tick on the first visible scanline
		if (scanlines == 0 && cycles == 0 && !m_even_frame)
		{
			if (IsRenderingEnabled())
			{
				cycles = 1;
			}
		}

		// https://wiki.nesdev.com/w/index.php/PPU_rendering#Pre-render_scanline_.28-1_or_261.29
		// This is a dummy scanline, whose sole purpose is to fill the shift registers with the data 
		// for the first two tiles of the next scanline.
		if (scanlines == -1 && cycles == 1)
		{
			m_reg_status.Bits.m_vertical_blank_started = 0;
			m_reg_status.Bits.m_sprite_overflow = 0;
			m_reg_status.Bits.m_sprite_zero_hit = 0;
			memset(m_shifter_pattern_sprite_low, 0x0, 8);
			memset(m_shifter_pattern_sprite_high, 0x0, 8);
		}

		if ((cycles >= 2 && cycles < 258) || (cycles >= 321 && cycles < 338))
		{
			UpdateShifters();
			// Fortunately, for background rendering, we go through a fairly
			// repeatable sequence of events, every 2 clock cycles.
			switch ((cycles - 1) % 8)
			{
			case 0:
			{
				TransferLatchesToShiftRegisters();
				ExtractTileAddress();
			}
			break; 
			case 2:
				// Fetch the next background tile attribute. OK, so this one is a bit
				// more involved :P

				// Recall that each nametable has two rows of cells that are not tile 
				// information, instead they represent the attribute information that
				// indicates which palettes are applied to which area on the screen.
				// Importantly (and frustratingly) there is not a 1 to 1 correspondance
				// between background tile and palette. Two rows of tile data holds
				// 64 attributes. Therfore we can assume that the attributes affect
				// 8x8 zones on the screen for that nametable. Given a working resolution
				// of 256x240, we can further assume that each zone is 32x32 pixels
				// in screen space, or 4x4 tiles. Four system palettes are allocated
				// to background rendering, so a palette can be specified using just
				// 2 bits. The attribute byte therefore can specify 4 distinct palettes.
				// Therefore we can even further assume that a single palette is
				// applied to a 2x2 tile combination of the 4x4 tile zone. The very fact
				// that background tiles "share" a palette locally is the reason why
				// in some games you see distortion in the colours at screen edges.

				// As before when choosing the tile ID, we can use the bottom 12 bits of
				// the loopy register, but we need to make the implementation "coarser"
				// because instead of a specific tile, we want the attribute byte for a 
				// group of 4x4 tiles, or in other words, we divide our 32x32 address
				// by 4 to give us an equivalent 8x8 address, and we offset this address
				// into the attribute section of the target nametable.

				// Reconstruct the 12 bit loopy address into an offset into the
				// attribute memory

				// "(vram_addr.coarse_x >> 2)"        : integer divide coarse x by 4, 
				//                                      from 5 bits to 3 bits
				// "((vram_addr.coarse_y >> 2) << 3)" : integer divide coarse y by 4, 
				//                                      from 5 bits to 3 bits,
				//                                      shift to make room for coarse x

				// Result so far: YX00 00yy yxxx

				// All attribute memory begins at 0x03C0 within a nametable, so OR with
				// result to select target nametable, and attribute byte offset. Finally
				// OR with 0x2000 to offset into nametable address space on PPU bus.				
				m_bg_next_tile_attrib = memory.PPUReadByte(0x23C0 | (m_current_vram.Bits.m_nametable_y << 11)
					| (m_current_vram.Bits.m_nametable_x << 10)
					| ((m_current_vram.Bits.m_coarse_y >> 2) << 3)
					| (m_current_vram.Bits.m_coarse_x >> 2));

				// Right we've read the correct attribute byte for a specified address,
				// but the byte itself is broken down further into the 2x2 tile groups
				// in the 4x4 attribute zone.

				// The attribute byte is assembled thus: BR(76) BL(54) TR(32) TL(10)
				//
				// +----+----+			    +----+----+
				// | TL | TR |			    | ID | ID |
				// +----+----+ where TL =   +----+----+
				// | BL | BR |			    | ID | ID |
				// +----+----+			    +----+----+
				//
				// Since we know we can access a tile directly from the 12 bit address, we
				// can analyse the bottom bits of the coarse coordinates to provide us with
				// the correct offset into the 8-bit word, to yield the 2 bits we are
				// actually interested in which specifies the palette for the 2x2 group of
				// tiles. We know if "coarse y % 4" < 2 we are in the top half else bottom half.
				// Likewise if "coarse x % 4" < 2 we are in the left half else right half.
				// Ultimately we want the bottom two bits of our attribute word to be the
				// palette selected. So shift as required...				
				if (m_current_vram.Bits.m_coarse_y & 0x02) m_bg_next_tile_attrib >>= 4;
				if (m_current_vram.Bits.m_coarse_x & 0x02) m_bg_next_tile_attrib >>= 2;
				m_bg_next_tile_attrib &= 0x03;
				break;

				// Compared to the last two, the next two are the easy ones... :P

			case 4:
			{
				// Fetch the next background tile LSB bit plane from the pattern memory
				// The Tile ID has been read from the nametable. We will use this id to 
				// index into the pattern memory to find the correct sprite (assuming
				// the sprites lie on 8x8 pixel boundaries in that memory, which they do
				// even though 8x16 sprites exist, as background tiles are always 8x8).
				//
				// Since the sprites are effectively 1 bit deep, but 8 pixels wide, we 
				// can represent a whole sprite row as a single byte, so offsetting
				// into the pattern memory is easy. In total there is 8KB so we need a 
				// 13 bit address.

				// "(control.pattern_background << 12)"  : the pattern memory selector 
				//                                         from control register, either 0K
				//                                         or 4K offset
				// "((uint16_t)bg_next_tile_id << 4)"    : the tile id multiplied by 16, as
				//                                         2 lots of 8 rows of 8 bit pixels
				// "(vram_addr.fine_y)"                  : Offset into which row based on
				//                                         vertical scroll offset
				// "+ 0"                                 : Mental clarity for plane offset
				// Note: No PPU address bus offset required as it starts at 0x0000
				const uint8_t nametable_tile = memory.PPUReadByte(m_background_tile_addr);
				m_bg_next_tile_lsb = memory.PPUReadByte((GetControlRegister().Bits.m_bkgrnd_pattern_addr << 12)
					+ ((uint16_t)nametable_tile << 4)
					+ (m_current_vram.Bits.m_fine_y) + 0);
			}
			break;
			case 6:
			{
				// Fetch the next background tile MSB bit plane from the pattern memory
				// This is the same as above, but has a +8 offset to select the next bit plane
				const uint8_t nametable_tile = memory.PPUReadByte(m_background_tile_addr);
				m_bg_next_tile_msb = memory.PPUReadByte((GetControlRegister().Bits.m_bkgrnd_pattern_addr << 12)
					+ ((uint16_t)nametable_tile << 4)
					+ (m_current_vram.Bits.m_fine_y) + 8);
			}
			break;
			case 7:
				// https://wiki.nesdev.com/w/index.php/PPU_scrolling#Coarse_X_increment
				// The coarse X component of v needs to be incremented when the 
				// next tile is reached
				CoarseXIncrement();
				break;
			}
		}
	}

	// https://wiki.nesdev.com/w/index.php/PPU_scrolling#Y_increment
	// If rendering is enabled, fine Y is incremented at dot 256 of each scanline
	if (cycles == 256)
	{
		YIncrement();
	}

	//...and reset the x position
	if (cycles == 257)
	{
		TransferLatchesToShiftRegisters();
		// https://wiki.nesdev.com/w/index.php/PPU_scrolling#At_dot_257_of_each_scanline
		// If rendering is enabled, the PPU copies all bits related to horizontal 
		// position from t to v
		if (IsRenderingEnabled())
		{
			m_current_vram.Bits.m_nametable_x = m_temp_vram.Bits.m_nametable_x;
			m_current_vram.Bits.m_coarse_x = m_temp_vram.Bits.m_coarse_x;
		}
	}

	// https://wiki.nesdev.com/w/index.php/PPU_rendering#Cycles_337-340
	// two bytes are fetched, but the purpose for this is unknown. 
	// These fetches are 2 PPU cycles each.
	if (cycles == 338 || cycles == 340)
	{
		ExtractTileAddress();
	}

	if (scanlines == -1 && cycles >= 280 && cycles < 305)
	{
		// https://wiki.nesdev.com/w/index.php/PPU_scrolling#During_dots_280_to_304_of_the_pre-render_scanline_.28end_of_vblank.29
		// the PPU will repeatedly copy the vertical bits from 
		// t to v from dots 280 to 304, completing the full initialization of v from t
		if (IsRenderingEnabled())
		{
			m_current_vram.Bits.m_fine_y = m_temp_vram.Bits.m_fine_y;
			m_current_vram.Bits.m_nametable_y = m_temp_vram.Bits.m_nametable_y;
			m_current_vram.Bits.m_coarse_y = m_temp_vram.Bits.m_coarse_y;
		}
	}

	// Foreground Rendering ========================================================
	// I'm gonna cheat a bit here, which may reduce compatibility, but greatly
	// simplifies delivering an intuitive understanding of what exactly is going
	// on. The PPU loads sprite information successively during the region that
	// background tiles are not being drawn. Instead, I'm going to perform
	// all sprite evaluation in one hit. THE NES DOES NOT DO IT LIKE THIS! This makes
	// it easier to see the process of sprite evaluation.
	if (cycles == 257 && scanlines >= 0)
	{
		memory.ClearSecondaryOAM();
		m_active_sprites = 0;
		memset(m_shifter_pattern_sprite_low, 0x0, 8);
		memset(m_shifter_pattern_sprite_high, 0x0, 8);
		m_possible_sprite_zero_hit = false;

		const uint8_t* oam = memory.GetPrimaryOAM();
		uint8_t* secondary_oam = memory.GetSeconaryOAM();
		for (int i = 0; i < 256; i += 4)
		{
			const uint8_t byte0 = oam[i];
			if (byte0 > 239)
			{
				continue;
			}
			const uint8_t byte1 = oam[i + 1];
			const uint8_t byte2 = oam[i + 2];
			const uint8_t byte3 = oam[i + 3];

			int16_t diff = ((int16_t)scanlines - (int16_t)byte0);
			const int sprite_size = m_reg_ppu_control.Bits.m_sprite_size ? 16 : 8;
			if (diff >= 0 && diff < sprite_size && m_active_sprites < 8)
			{
				if (m_active_sprites < 8)
				{
					if (i == 0)
					{
						m_possible_sprite_zero_hit = true;
					}
					assert(m_active_sprites * 4 < 64);
					memcpy(&secondary_oam[m_active_sprites*4], &oam[i], 4);
				}
				m_active_sprites++;
			}
		}

		m_reg_status.Bits.m_sprite_overflow = (m_active_sprites >= 8);
	}


	if (cycles == 340)
	{
		uint8_t* secondary_oam = memory.GetSeconaryOAM();
		for (uint8_t i = 0; i < m_active_sprites; i++)
		{
			assert(((i * 4) + 3) < 64);
			const uint8_t byte0 = secondary_oam[(i * 4)];
			const uint8_t byte1 = secondary_oam[(i * 4) + 1];
			const uint8_t byte2 = secondary_oam[(i * 4) + 2];
			const uint8_t byte3 = secondary_oam[(i * 4) + 3];
			uint8_t sprite_pattern_bits_lo, sprite_pattern_bits_hi;
			uint16_t sprite_pattern_addr_lo, sprite_pattern_addr_hi;


			if (!GetControlRegister().Bits.m_sprite_size)
			{
				// 8x8 Sprite Mode - The control register determines the pattern table
				if (!(byte2 & 0x80))
				{
					// Sprite is NOT flipped vertically, i.e. normal    
					sprite_pattern_addr_lo =
						(GetControlRegister().Bits.m_sprite_pattern_addr << 12)  // Which Pattern Table? 0KB or 4KB offset
						| (byte1 << 4)  // Which Cell? Tile ID * 16 (16 bytes per tile)
						| (scanlines - byte0); // Which Row in cell? (0->7)

				}
				else
				{
					// Sprite is flipped vertically, i.e. upside down
					sprite_pattern_addr_lo =
						(GetControlRegister().Bits.m_sprite_pattern_addr << 12)  // Which Pattern Table? 0KB or 4KB offset
						| (byte1 << 4)  // Which Cell? Tile ID * 16 (16 bytes per tile)
						| (7 - (scanlines - byte0)); // Which Row in cell? (7->0)
				}

			}
			else
			{
				// 8x16 Sprite Mode - The sprite attribute determines the pattern table
				if (!(byte2 & 0x80))
				{
					// Sprite is NOT flipped vertically, i.e. normal
					if (scanlines - byte0 < 8)
					{
						// Reading Top half Tile
						sprite_pattern_addr_lo =
							((m_reg_ppu_control.Bits.m_sprite_pattern_addr & 0x01) << 12)  // Which Pattern Table? 0KB or 4KB offset
							| ((byte1 & 0xFE) << 4)  // Which Cell? Tile ID * 16 (16 bytes per tile)
							| ((scanlines - byte0) & 0x07); // Which Row in cell? (0->7)
					}
					else
					{
						// Reading Bottom Half Tile
						sprite_pattern_addr_lo =
							((m_reg_ppu_control.Bits.m_sprite_pattern_addr & 0x01) << 12)  // Which Pattern Table? 0KB or 4KB offset
							| (((byte1 & 0xFE) + 1) << 4)  // Which Cell? Tile ID * 16 (16 bytes per tile)
							| ((scanlines - byte0) & 0x07); // Which Row in cell? (0->7)
					}
				}
				else
				{
					// Sprite is flipped vertically, i.e. upside down
					if (scanlines - byte0 < 8)
					{
						// Reading Top half Tile
						sprite_pattern_addr_lo =
							((byte1 & 0x01) << 12)    // Which Pattern Table? 0KB or 4KB offset
							| (((byte1 & 0xFE) + 1) << 4)    // Which Cell? Tile ID * 16 (16 bytes per tile)
							| (7 - (scanlines - byte0) & 0x07); // Which Row in cell? (0->7)
					}
					else
					{
						// Reading Bottom Half Tile
						sprite_pattern_addr_lo =
							((byte1 & 0x01) << 12)    // Which Pattern Table? 0KB or 4KB offset
							| ((byte1 & 0xFE) << 4)    // Which Cell? Tile ID * 16 (16 bytes per tile)
							| (7 - (scanlines - byte0) & 0x07); // Which Row in cell? (0->7)
					}
				}
			}

			// Phew... XD I'm absolutely certain you can use some fantastic bit 
			// manipulation to reduce all of that to a few one liners, but in this
			// form it's easy to see the processes required for the different
			// sizes and vertical orientations

			// Hi bit plane equivalent is always offset by 8 bytes from lo bit plane
			sprite_pattern_addr_hi = sprite_pattern_addr_lo + 8;

			// Now we have the address of the sprite patterns, we can read them
			sprite_pattern_bits_lo = memory.PPUReadByte(sprite_pattern_addr_lo);
			sprite_pattern_bits_hi = memory.PPUReadByte(sprite_pattern_addr_hi);

			// If the sprite is flipped horizontally, we need to flip the 
			// pattern bytes. 
			if (byte2 & 0x40)
			{
				// This little lambda function "flips" a byte
				// so 0b11100000 becomes 0b00000111. It's very
				// clever, and stolen completely from here:
				// https://stackoverflow.com/a/2602885
				auto flipbyte = [](uint8_t b)
				{
					b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
					b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
					b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
					return b;
				};

				// Flip Patterns Horizontally
				sprite_pattern_bits_lo = flipbyte(sprite_pattern_bits_lo);
				sprite_pattern_bits_hi = flipbyte(sprite_pattern_bits_hi);
			}

			// Finally! We can load the pattern into our sprite shift registers
			// ready for rendering on the next scanline
			m_shifter_pattern_sprite_low[i] = sprite_pattern_bits_lo;
			m_shifter_pattern_sprite_high[i] = sprite_pattern_bits_hi;
		}
	}

	if (scanlines == 240)
	{
		// Post Render Scanline - Do Nothing!
	}

	if (scanlines >= 241 && scanlines < 261)
	{
		if (scanlines == 241 && cycles == 1)
		{
			// Effectively end of frame, so set vertical blank flag
			m_reg_status.Bits.m_vertical_blank_started = 1;

			// If the control register tells us to emit a NMI when
			// entering vertical blanking period, do it! The CPU
			// will be informed that rendering is complete so it can
			// perform operations with the PPU knowing it wont
			// produce visible artefacts
			if (m_reg_ppu_control.Bits.m_generate_nmi)
				m_cpu_nmi = true;
		}
	}

	// Composition - We now have background & foreground pixel information for this cycle

	// Background =============================================================
	uint8_t bg_pixel = 0x00;   // The 2-bit pixel to be rendered
	uint8_t bg_palette = 0x00; // The 3-bit index of the palette the pixel indexes

	// We only render backgrounds if the PPU is enabled to do so. Note if 
	// background rendering is disabled, the pixel and palette combine
	// to form 0x00. This will fall through the colour tables to yield
	// the current background colour in effect
	if (m_reg_ppu_mask.Bits.m_show_bkgrnd)
	{
		if (m_reg_ppu_mask.Bits.m_show_bkgrnd_leftmost || (cycles >= 9))
		{
			// Handle Pixel Selection by selecting the relevant bit
			// depending upon fine x scolling. This has the effect of
			// offsetting ALL background rendering by a set number
			// of pixels, permitting smooth scrolling
			uint16_t bit_mux = 0x8000 >> m_fine_x;

			// Select Plane pixels by extracting from the shifter 
			// at the required location. 
			uint8_t p0_pixel = (m_bg_shifter_pattern_lo & bit_mux) > 0;
			uint8_t p1_pixel = (m_bg_shifter_pattern_hi & bit_mux) > 0;

			// Combine to form pixel index
			bg_pixel = (p1_pixel << 1) | p0_pixel;

			// Get palette
			uint8_t bg_pal0 = (m_bg_shifter_attrib_lo & bit_mux) > 0;
			uint8_t bg_pal1 = (m_bg_shifter_attrib_hi & bit_mux) > 0;
			bg_palette = (bg_pal1 << 1) | bg_pal0;
		}
	}

	// Foreground =============================================================
	uint8_t fg_pixel = 0x00;   // The 2-bit pixel to be rendered
	uint8_t fg_palette = 0x00; // The 3-bit index of the palette the pixel indexes
	uint8_t fg_priority = 0x00;// A bit of the sprite attribute indicates if its
							   // more important than the background
	if (m_reg_ppu_mask.Bits.m_show_sprites)
	{
		// Iterate through all sprites for this scanline. This is to maintain
		// sprite priority. As soon as we find a nonf transparent pixel of
		// a sprite we can abort
		if (m_reg_ppu_mask.Bits.m_show_sprites_leftmost || (cycles >= 9))
		{
			m_sprite_zero_rendered = false;
			uint8_t* secondary_oam = memory.GetSeconaryOAM();
			for (uint8_t i = 0; i < m_active_sprites; i++)
			{
				assert(((i * 4) + 3) < 64);
				const uint8_t byte0 = secondary_oam[(i * 4)];
				const uint8_t byte1 = secondary_oam[(i * 4) + 1];
				const uint8_t byte2 = secondary_oam[(i * 4) + 2];
				const uint8_t byte3 = secondary_oam[(i * 4) + 3];
				// Scanline cycle has "collided" with sprite, shifters taking over
				if (byte3 == 0)
				{
					// Note Fine X scrolling does not apply to sprites, the game
					// should maintain their relationship with the background. So
					// we'll just use the MSB of the shifter

					// Determine the pixel value...
					uint8_t fg_pixel_lo = (m_shifter_pattern_sprite_low[i] & 0x80) > 0;
					uint8_t fg_pixel_hi = (m_shifter_pattern_sprite_high[i] & 0x80) > 0;
					fg_pixel = (fg_pixel_hi << 1) | fg_pixel_lo;

					// Extract the palette from the bottom two bits. Recall
					// that foreground palettes are the latter 4 in the 
					// palette memory.
					fg_palette = (byte2 & 0x03) + 0x04;
					fg_priority = (byte2 & 0x20) == 0;

					// If pixel is not transparent, we render it, and dont
					// bother checking the rest because the earlier sprites
					// in the list are higher priority
					if (fg_pixel != 0)
					{
						if (i == 0) // Is this sprite zero?
						{
							m_sprite_zero_rendered = true;
						}

						break;
					}
				}
			}
		}
	}

	// Now we have a background pixel and a foreground pixel. They need
	// to be combined. It is possible for sprites to go behind background
	// tiles that are not "transparent", yet another neat trick of the PPU
	// that adds complexity for us poor emulator developers...

	uint8_t pixel = 0x00;   // The FINAL Pixel...
	uint8_t palette = 0x00; // The FINAL Palette...

	if (bg_pixel == 0 && fg_pixel == 0)
	{
		// The background pixel is transparent
		// The foreground pixel is transparent
		// No winner, draw "background" colour
		pixel = 0x00;
		palette = 0x00;
	}
	else if (bg_pixel == 0 && fg_pixel > 0)
	{
		// The background pixel is transparent
		// The foreground pixel is visible
		// Foreground wins!
		pixel = fg_pixel;
		palette = fg_palette;
	}
	else if (bg_pixel > 0 && fg_pixel == 0)
	{
		// The background pixel is visible
		// The foreground pixel is transparent
		// Background wins!
		pixel = bg_pixel;
		palette = bg_palette;
	}
	else if (bg_pixel > 0 && fg_pixel > 0)
	{
		// The background pixel is visible
		// The foreground pixel is visible
		// Hmmm...
		if (fg_priority)
		{
			// Foreground cheats its way to victory!
			pixel = fg_pixel;
			palette = fg_palette;
		}
		else
		{
			// Background is considered more important!
			pixel = bg_pixel;
			palette = bg_palette;
		}

		// Sprite Zero Hit detection
		if (m_possible_sprite_zero_hit && m_sprite_zero_rendered)
		{
			// Sprite zero is a collision between foreground and background
			// so they must both be enabled
			if (m_reg_ppu_mask.Bits.m_show_bkgrnd & m_reg_ppu_mask.Bits.m_show_sprites)
			{
				// The left edge of the screen has specific switches to control
				// its appearance. This is used to smooth inconsistencies when
				// scrolling (since sprites x coord must be >= 0)
				if (!(m_reg_ppu_mask.Bits.m_show_bkgrnd_leftmost | m_reg_ppu_mask.Bits.m_show_sprites_leftmost))
				{
					if (cycles >= 9 && cycles < 258)
					{
						m_reg_status.Bits.m_sprite_zero_hit = 1;
					}
				}
				else
				{
					if (cycles >= 1 && cycles < 258)
					{
						m_reg_status.Bits.m_sprite_zero_hit = 1;
					}
				}
			}
		}
	}

	//sprScreen.SetPixel(cycle - 1, scanline, GetColourFromPaletteRam(palette, pixel));
	if (cycles >= 0 && cycles < 256 && scanlines < 240 && scanlines >= 0)
	{
		const int index = (cycles) + (scanlines * 256);
		assert(index >= 0 && index < 256 * 240);
		assert(pixel >= 0 && pixel < 4);
		static int pixel_counter[4] = { 0 };
		pixel_counter[pixel]++;
		//m_frame_buffer_data[index] = COLOR_PALETTE[pixel];
		const int nes_palette_index = memory.PPUReadByte(0x3F00 + (palette << 2) + pixel) & 0x3F;
		assert(nes_palette_index >= 0 && nes_palette_index < 64);
		static int counter[64] = { 0 };
		counter[nes_palette_index]++;
		m_frame_buffer_data[index] = PaletteColors[nes_palette_index];
	}
		
	// Advance renderer - it never stops, it's relentless
	cycles++;
	if (IsRenderingEnabled())
	{
		if (cycles == 260 && scanlines < 240)
		{
			//cart->GetMapper()->scanline();
		}
	}

	if (cycles >= 341)
	{
		cycles = 0;
		scanlines++;
		if (scanlines >= 261)
		{
			scanlines = -1;
			m_frame_ready = true;
			m_even_frame = !m_even_frame;
		}
	}
#if 0
		// nametable
		const uint8_t bkgrnd_pattern_table_id = GetControlRegister().Bits.m_bkgrnd_pattern_addr;
		const uint8_t sprite_pattern_table_id = GetControlRegister().Bits.m_sprite_pattern_addr;
		const uint8_t* oam = NESConsole::GetInstance()->GetMemory().GetOAM();
		const uint16_t attribute_memory_start = nametable_addr + 960;
		const uint16_t attribute_memory_end = nametable_addr + 1024;
		const int sprite_size = GetControlRegister().Bits.m_sprite_size == 1 ? 16 : 8;
		uint16_t tile_offset = bkgrnd_pattern_table_id == 0 ? 0 : 256;

		// scanline pixels
		for (int i = 0; i < 256; i += 8)
		{
			// fetch attribute byte for this pixel
			const uint8_t attribute_byte = GetAttributeByte(i, scanlines, nametable_addr);
			
			// get palette index out ofthe attribute byte
			const uint8_t palette_index = GetPaletteIndexFromAttributeByte(i, scanlines, attribute_byte);

			// get the tile_id for these 8 pixels
			const uint16_t nametable_index = nametable_addr + ((i / 8) + ((scanlines/8) * (256/8)));
			const uint8_t tile_id = memory.PPUReadByte(nametable_index);

			// get corresponding tile
			PatternTableTile* tile = m_pattern_table->GetTile(tile_id + tile_offset);
			
			// get corresponding row out of the tile
			const int row = scanlines % 8;
			uint32_t pixels[8] = { 0 };
			tile->GetTextureDataRow(row, pixels, FillData(palette_index));

			// copy row data into frame buffer
			memcpy(&m_frame_buffer_data[i + (scanlines * 256)], pixels, 8 * sizeof(uint32_t));

			static const int SPRITE_PER_SCANLINE = 8;
			uint8_t oam_indeces[SPRITE_PER_SCANLINE] = {0};
			uint8_t num_indeces = 0;
			for (int j = 0; j < 256 && num_indeces < SPRITE_PER_SCANLINE; j += 4)
			{
				const uint8_t byte0 = oam[j];
				// is this a valid sprite
				if (byte0 > 239)
				{
					continue;
				}
				
				// is any part of this sprite on this scanline
				if (byte0 + 1 >= scanlines && byte0 + 1 <= scanlines + sprite_size)
				{
					oam_indeces[num_indeces] = j;
					num_indeces++;
				}
			}

			for (int j = 0; j < num_indeces; ++j)
			{
				assert(oam_indeces[j] < 256);
				const uint8_t byte0 = oam[oam_indeces[j]];
				const uint8_t byte1 = oam[oam_indeces[j+1]];
				// https://wiki.nesdev.com/w/index.php/PPU_OAM#Byte_2
				// 76543210
				// ||||||||
				// ||||||++ - Palette(4 to 7) of sprite
				// |||++ + -- - Unimplemented
				// || +------Priority(0: in front of background; 1: behind background)
				// | +------ - Flip sprite horizontally
				// + --------Flip sprite vertically
				const uint8_t byte2 = oam[oam_indeces[j+2]];
				const uint8_t byte3 = oam[oam_indeces[j+3]];

				// TODO: check priority onbyte 2

				uint8_t tile_id = 0;
				uint16_t tile_offset = 0;
				if (sprite_size == 16)
				{
					tile_id = byte1 & 0xFE;
					tile_offset = (byte1 & 0x01) == 0 ? 0 : 256;
				}
				else
				{
					tile_id = byte1;
					tile_offset = sprite_pattern_table_id == 0 ? 0 : 256;
				}

				const uint8_t palette_index = (byte2 & 0x3) + 4;;
				const bool flip_horizontally = byte2 & (1 << 6);
				const bool flip_vertically = byte2 & (1 << 7);

				PatternTableTile* tile1 = m_pattern_table->GetTile(tile_id + tile_offset);
				PatternTableTile* tile2 = nullptr;
				if (sprite_size == 16)
				{
					tile2 = m_pattern_table->GetTile(tile_id + tile_offset + 1);
				}

				uint8_t y_pos = byte0 + 1;
				if (sprite_size == 8)
				{
					// get the row we need from the tile based on the scanline and y pos
					FillData fill_data;
					fill_data.m_flip_horizontally = flip_horizontally;
					fill_data.m_flip_vertically = flip_vertically;
					fill_data.m_palette_index = palette_index;

					const int row = scanlines - y_pos;
					uint32_t row_data[8] = { 0 };
					tile1->GetTextureDataRow(row, row_data, fill_data);
					memcpy(&m_frame_buffer_data[i + (scanlines * 256)], row_data, 8 * sizeof(uint32_t));
				}
				else
				{
					assert(false);
				}
				
			}
		}
#endif
}

Nametable::Quadrant PPU::GetQuadrantFromAttribyteByte(const int pixel_x, const int pixel_y, const uint8_t attribute_byte)
{
	Nametable::Quadrant quadrant = Nametable::Quadrant::BOTTOM_RIGHT;
	const int row = pixel_y / 8;
	const int col = pixel_x / 8;
	if ((row / 2) % 2 == 0 && (col / 2) % 2 == 0)
	{
		quadrant = Nametable::Quadrant::TOP_LEFT;
	}
	else if ((row / 2) % 2 == 1 && (col / 2) % 2 == 0)
	{
		quadrant = Nametable::Quadrant::BOTTOM_LEFT;
	}
	else if ((row / 2) % 2 == 0 && (col / 2) % 2 == 1)
	{
		quadrant = Nametable::Quadrant::TOP_RIGHT;
	}
	return quadrant;
}

void PPU::CoarseXIncrement()
{
	// https://wiki.nesdev.com/w/index.php/PPU_scrolling#Coarse_X_increment
	if (IsRenderingEnabled())
	{
		if (m_current_vram.Bits.m_coarse_x == 31)
		{
			m_current_vram.Bits.m_coarse_x = 0;
			m_current_vram.Bits.m_nametable_x = ~m_current_vram.Bits.m_nametable_x;
		}
		else
		{
			m_current_vram.Bits.m_coarse_x += 1;
		}
	}
}

void PPU::YIncrement()
{
	// https://wiki.nesdev.com/w/index.php/PPU_scrolling#Y_increment
	if (IsRenderingEnabled())
	{
		// if fine y goes past 7, we need to 
		// increment coarse y
		if (m_current_vram.Bits.m_fine_y < 7)
		{
			m_current_vram.Bits.m_fine_y++;
		}
		else
		{
			// reset fine y to 0 on a coarse increment
			m_current_vram.Bits.m_fine_y = 0;
			// Row 29 is the last row of tiles in a 
			// nametable. To wrap to the next nametable when 
			// incrementing coarse Y from 29, the vertical nametable is 
			// switched by toggling bit 11, and coarse Y wraps to row 0.
			if (m_current_vram.Bits.m_coarse_y == 29)
			{
				m_current_vram.Bits.m_coarse_y = 0;
				m_current_vram.Bits.m_nametable_y = ~m_current_vram.Bits.m_nametable_y;
			}
			else if (m_current_vram.Bits.m_coarse_y == 31)
			{
				// coarse y wrap around
				m_current_vram.Bits.m_coarse_y = 0;
			}
			else
			{
				m_current_vram.Bits.m_coarse_y += 1;
			}
		}
		}
}

void PPU::ExtractTileAddress()
{
	// https://wiki.nesdev.com/w/index.php/PPU_scrolling#Explanation
	// The high 5 bits of the X and Y scroll settings sent to $2005, when combined with 
	// the 2 nametable select bits sent to $2000, make a 12 bit address for the next 
	// tile to be fetched within the nametable address space $2000-2FFF
	const uint16_t extracted_addr = m_current_vram.Register & 0x0FFF;
	const uint16_t nametable_addr = 0x2000 | extracted_addr;
	m_background_tile_addr = nametable_addr;
}

void PPU::TransferLatchesToShiftRegisters()
{
	// https://wiki.nesdev.com/w/index.php/PPU_rendering#Preface
	// Every 8 cycles/shifts, new data is loaded into these registers.
	// https://wiki.nesdev.com/w/index.php/PPU_rendering#Cycles_1-256
	// The data fetched from these accesses is placed into internal latches, 
	// and then fed to the appropriate shift registers when it's time to do so (every 8 cycles).
	m_bg_shifter_pattern_lo = (m_bg_shifter_pattern_lo & 0xFF00) | m_bg_next_tile_lsb;
	m_bg_shifter_pattern_hi = (m_bg_shifter_pattern_hi & 0xFF00) | m_bg_next_tile_msb;
	m_bg_shifter_attrib_lo = (m_bg_shifter_attrib_lo & 0xFF00) | ((m_bg_next_tile_attrib & 0b01) ? 0xFF : 0x00);
	m_bg_shifter_attrib_hi = (m_bg_shifter_attrib_hi & 0xFF00) | ((m_bg_next_tile_attrib & 0b10) ? 0xFF : 0x00);
}

uint8_t PPU::GetPaletteIndexFromAttributeByte(
	const int pixel_x, 
	const int pixel_y, 
	const uint8_t attribute_byte
)
{
	// https://wiki.nesdev.com/w/index.php/PPU_attribute_tables
	uint8_t palette_index = 0;
	uint8_t shift = 0;
	const Nametable::Quadrant quadrant = 
		GetQuadrantFromAttribyteByte(pixel_x, pixel_y, attribute_byte);
	switch (quadrant)
	{
	case Nametable::Quadrant::TOP_LEFT:
		shift = 0;
		break;
	case Nametable::Quadrant::TOP_RIGHT:
		shift = 2;
		break;
	case Nametable::Quadrant::BOTTOM_RIGHT:
		shift = 6;
		break;
	case Nametable::Quadrant::BOTTOM_LEFT:
		shift = 4;
		break;
	default:
		assert(false);
	}
	palette_index = (attribute_byte >> shift) & 0x03;
	return palette_index;
}

uint8_t PPU::GetAttributeByte(
	const int pixel_x, 
	const int pixel_y, 
	const uint16_t nametable_addr
)
{
	const int y_offset = 8 * (pixel_y / 32);
	const int x_offset = pixel_x / 32;
	const int byte_index = x_offset + y_offset;
	assert(byte_index >= 0 && byte_index < 64);
	return 0;
}

void PPU::SetOAMDMA(const uint8_t value)
{
	// https://wiki.nesdev.com/w/index.php/PPU_registers#OAM_DMA_.28.244014.29_.3E_write
	// Writing $XX will upload 256 bytes of data from CPU page $XX00-$XXFF 
	// to the internal PPU OAM. The CPU is suspended during the transfer, 
	// which will take 513 or 514 cycles after the $4014 write tick. 
	// (1 dummy read cycle while waiting for writes to complete, +1 if on 
	// an odd CPU cycle, then 256 alternating read/write cycles.)
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint16_t start_address = value;
	start_address = start_address << 8;
	
	uint16_t end_address = value;
	end_address = end_address << 8;
	end_address |= 0x00FF;
	for (uint16_t i = start_address; i <= end_address; ++i)
	{
		memory.PPUWriteOAM(i - start_address, memory.CPUReadByte(i));
	}
}

void PPU::ResetFrameReady()
{
	m_frame_ready = false;
}

uint8_t PPU::GetOAMData()
{
	return NESConsole::GetInstance()->GetMemory().PPUReadOAM(m_reg_oam_addr);
}

void PPU::SetOAMData(const uint8_t value)
{
	// https://wiki.nesdev.com/w/index.php/PPU_registers#OAMDATA
	NESConsole::GetInstance()->GetMemory().PPUWriteOAM(m_reg_oam_addr, value);
	m_reg_oam_addr += 1;
}
