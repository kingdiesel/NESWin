#include "PPU.h"
#include "NESConsole.h"
#include "PatternTable.h"
#include "PatternTableTile.h"
#include <cassert>
#include "Palette.h"
#include <iostream>
#include "Sprites.h"

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
			switch ((cycles - 1) % 8)
			{
			case 0:
			{
				TransferLatchesToShiftRegisters();
				ExtractTileAddress();
			}
			break; 
			case 2:
				m_bg_next_tile_attrib = memory.PPUReadByte(0x23C0 | (m_current_vram.Bits.m_nametable_y << 11)
					| (m_current_vram.Bits.m_nametable_x << 10)
					| ((m_current_vram.Bits.m_coarse_y >> 2) << 3)
					| (m_current_vram.Bits.m_coarse_x >> 2));

				if (m_current_vram.Bits.m_coarse_y & 0x02)
				{
					m_bg_next_tile_attrib >>= 4;
				}
					
				if (m_current_vram.Bits.m_coarse_x & 0x02)
				{
					m_bg_next_tile_attrib >>= 2;
				}
				m_bg_next_tile_attrib &= 0x03;
				break;

			case 4:
			{
				const uint8_t nametable_tile = memory.PPUReadByte(m_background_tile_addr);
				m_bg_next_tile_lsb = memory.PPUReadByte(
					(GetControlRegister().Bits.m_bkgrnd_pattern_addr << 12)
					+ ((uint16_t)nametable_tile << 4)
					+ (m_current_vram.Bits.m_fine_y) + 0);
			}
			break;
			case 6:
			{
				const uint8_t nametable_tile = memory.PPUReadByte(m_background_tile_addr);
				m_bg_next_tile_msb = memory.PPUReadByte(
					(GetControlRegister().Bits.m_bkgrnd_pattern_addr << 12)
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
			const bool is8x16sprite = GetControlRegister().Bits.m_sprite_size == 1;
			bool flip_horizontally = false, flip_vertically = false;
			Sprites::GetFlipFromByte2(byte2, flip_horizontally, flip_vertically);
	
			const uint8_t pattern_table_id = GetControlRegister().Bits.m_sprite_pattern_addr;
			uint8_t tile_id = 0;
			uint16_t tile_offset = 0;
			Sprites::GetTileDataFromByte1(byte1, is8x16sprite, pattern_table_id, tile_id, tile_offset);
			// 8x8 sprite mode
			if (!is8x16sprite)
			{
				if (flip_vertically)
				{
					sprite_pattern_addr_lo = 
						(GetControlRegister().Bits.m_sprite_pattern_addr << 12)  // Which Pattern Table? 0KB or 4KB offset
						| (byte1 << 4) | (7 - (scanlines - byte0));
				}
				else
				{
					sprite_pattern_addr_lo = (GetControlRegister().Bits.m_sprite_pattern_addr << 12)
						| tile_id  << 4 | (scanlines - byte0);
				}
			}
			else
			{
				if (!flip_vertically)
				{
					if (scanlines - byte0 < 8)
					{
						sprite_pattern_addr_lo =
							((m_reg_ppu_control.Bits.m_sprite_pattern_addr & 0x01) << 12)
							| (tile_id << 4)
							| ((scanlines - byte0) & 0x07);
					}
					else
					{
						sprite_pattern_addr_lo =
							((m_reg_ppu_control.Bits.m_sprite_pattern_addr & 0x01) << 12)
							| ((tile_id + 1) << 4)
							| ((scanlines - byte0) & 0x07);
					}
				}
				else
				{
					if (scanlines - byte0 < 8)
					{
						sprite_pattern_addr_lo =
							((byte1 & 0x01) << 12)
							| ((tile_id + 1) << 4)
							| (7 - (scanlines - byte0) & 0x07);
					}
					else
					{
						sprite_pattern_addr_lo =
							((byte1 & 0x01) << 12)
							| (tile_id << 4)
							| (7 - (scanlines - byte0) & 0x07);
					}
				}
			}

			sprite_pattern_addr_hi = sprite_pattern_addr_lo + 8;
			sprite_pattern_bits_lo = memory.PPUReadByte(sprite_pattern_addr_lo);
			sprite_pattern_bits_hi = memory.PPUReadByte(sprite_pattern_addr_hi);
			if (flip_horizontally)
			{
				// https://stackoverflow.com/a/2602885
				auto flipbyte = [](uint8_t b)
				{
					b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
					b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
					b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
					return b;
				};
				sprite_pattern_bits_lo = flipbyte(sprite_pattern_bits_lo);
				sprite_pattern_bits_hi = flipbyte(sprite_pattern_bits_hi);
			}

			m_shifter_pattern_sprite_low[i] = sprite_pattern_bits_lo;
			m_shifter_pattern_sprite_high[i] = sprite_pattern_bits_hi;
		}
	}

	if (scanlines >= 241 && scanlines < 261)
	{
		if (scanlines == 241 && cycles == 1)
		{
			m_reg_status.Bits.m_vertical_blank_started = 1;

			if (m_reg_ppu_control.Bits.m_generate_nmi)
			{
				m_cpu_nmi = true;
			}
		}
	}

	// https://wiki.nesdev.com/w/index.php/PPU_rendering#Preface
	uint8_t bg_pixel = 0x00;
	uint8_t bg_palette = 0x00;
	if (m_reg_ppu_mask.Bits.m_show_bkgrnd)
	{
		if (m_reg_ppu_mask.Bits.m_show_bkgrnd_leftmost || (cycles >= 9))
		{
			uint16_t bit_mux = 0x8000 >> m_fine_x;
			uint8_t p0_pixel = (m_bg_shifter_pattern_lo & bit_mux) > 0;
			uint8_t p1_pixel = (m_bg_shifter_pattern_hi & bit_mux) > 0;
			bg_pixel = (p1_pixel << 1) | p0_pixel;
			uint8_t bg_pal0 = (m_bg_shifter_attrib_lo & bit_mux) > 0;
			uint8_t bg_pal1 = (m_bg_shifter_attrib_hi & bit_mux) > 0;
			bg_palette = (bg_pal1 << 1) | bg_pal0;
		}
	}

	uint8_t fg_pixel = 0x00;
	uint8_t fg_palette = 0x00;
	uint8_t fg_priority = 0x00;
	if (m_reg_ppu_mask.Bits.m_show_sprites)
	{
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
				if (byte3 == 0)
				{
					uint8_t fg_pixel_lo = (m_shifter_pattern_sprite_low[i] & 0x80) > 0;
					uint8_t fg_pixel_hi = (m_shifter_pattern_sprite_high[i] & 0x80) > 0;
					fg_pixel = (fg_pixel_hi << 1) | fg_pixel_lo;
					fg_palette = (byte2 & 0x03) + 0x04;
					fg_priority = (byte2 & 0x20) == 0;
					if (fg_pixel != 0)
					{
						if (i == 0)
						{
							m_sprite_zero_rendered = true;
						}

						break;
					}
				}
			}
		}
	}

	uint8_t pixel = 0x00;
	uint8_t palette = 0x00;

	if (bg_pixel == 0 && fg_pixel == 0)
	{
		pixel = 0x00;
		palette = 0x00;
	}
	else if (bg_pixel == 0 && fg_pixel > 0)
	{
		pixel = fg_pixel;
		palette = fg_palette;
	}
	else if (bg_pixel > 0 && fg_pixel == 0)
	{
		pixel = bg_pixel;
		palette = bg_palette;
	}
	else if (bg_pixel > 0 && fg_pixel > 0)
	{
		if (fg_priority)
		{
			pixel = fg_pixel;
			palette = fg_palette;
		}
		else
		{
			pixel = bg_pixel;
			palette = bg_palette;
		}

		if (m_possible_sprite_zero_hit && m_sprite_zero_rendered)
		{
			if (m_reg_ppu_mask.Bits.m_show_bkgrnd & m_reg_ppu_mask.Bits.m_show_sprites)
			{
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

	if (cycles >= 0 && cycles < 256 && scanlines < 240 && scanlines >= 0)
	{
		const int index = (cycles) + (scanlines * 256);
		assert(index >= 0 && index < 256 * 240);
		assert(pixel >= 0 && pixel < 4);
		static int pixel_counter[4] = { 0 };
		pixel_counter[pixel]++;
		const int nes_palette_index = memory.PPUReadByte(0x3F00 + (palette << 2) + pixel) & 0x3F;
		assert(nes_palette_index >= 0 && nes_palette_index < 64);
		static int counter[64] = { 0 };
		counter[nes_palette_index]++;
		m_frame_buffer_data[index] = PaletteColors[nes_palette_index];
	}
		
	cycles++;

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
		assert(i - start_address <= std::numeric_limits<uint8_t>::max());
		memory.PPUWriteOAM(static_cast<uint8_t>(i - start_address), memory.CPUReadByte(i));
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
