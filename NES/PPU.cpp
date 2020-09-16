#include "PPU.h"
#include "Source/NESConsole.h"
#include "PatternTable.h"
#include "Source/PatternTableTile.h"
#include <cassert>

PPU::PPU()
{
	m_frame_buffer_data = new uint32_t[256 * 240];
	memset(m_frame_buffer_data, 0x00, 256 * 240 * sizeof(uint32_t));
}

void PPU::PowerUp()
{
	// https://wiki.nesdev.com/w/index.php/PPU_power_up_state
}

void PPU::Reset()
{
	// https://wiki.nesdev.com/w/index.php/PPU_power_up_state
}

void PPU::Run()
{
	m_even_frame = !m_even_frame;
	const bool show_background = GetMaskRegister().Bits.m_show_bkgrnd;
	const bool show_sprites = GetMaskRegister().Bits.m_show_sprites;
	const uint8_t background_pattern_table = GetControlRegister().Bits.m_bkgrnd_pattern_addr;
	const bool rendering_disabled = !(show_background && show_sprites);
	const Memory& memory = NESConsole::GetInstance()->GetMemory();
	// The PPU renders 262 scanlines per frame. Each scanline lasts for 
	// 341 PPU clock cycles (113.667 CPU clock cycles; 1 CPU cycle = 3 PPU cycles), 
	// with each clock cycle producing one pixel.
	// (0 = $2000; 1 = $2400; 2 = $2800; 3 = $2C00)
	uint16_t nametable_addr = 0x2000;
	switch (GetControlRegister().Bits.m_base_nametable_addr)
	{
	case 1:
		nametable_addr = 0x2400;
		break;
	case 2:
		nametable_addr = 0x2800;
		break;
	case 3:
		nametable_addr = 0x2C00;
		break;
	case 0:
	default:
		break;
	}
	const uint16_t attribute_addr = nametable_addr + 960;

	if (cycles > 340) 
	{
		cycles -= 341;
		++scanlines;
	}

	if (scanlines >=0 && scanlines <= 239)
	{ 
		// nametable
		const uint8_t pattern_table_id = GetControlRegister().Bits.m_bkgrnd_pattern_addr;
		const uint16_t attribute_memory_start = nametable_addr + 960;
		const uint16_t attribute_memory_end = nametable_addr + 1024;
		uint16_t tile_offset = pattern_table_id == 0 ? 0 : 256;

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
		}

		// sprite


	}
	else if (scanlines == 241 && cycles == 1) 
	{
		m_reg_status.Bits.m_vertical_blank_started = 1;
		// if we are Generate an NMI at the start of the vertical blanking interval
		if (GetControlRegister().Bits.m_generate_nmi == 1)
		{
			m_cpu_nmi = true;
		}
	}
	else if (scanlines == 261 && cycles == 1) 
	{    
		m_reg_status.Bits.m_vertical_blank_started = 0;
		m_cpu_nmi = false;
		scanlines = 0;
		SetFrameReady(true);
	}
	cycles++;
}

Nametable::Quadrant PPU::GetQuadrantFromAttribyteByte(const int pixel_x, const int pixel_y, const uint8_t attribute_byte)
{
	Nametable::Quadrant quadrant = Nametable::Quadrant::BOTTOM_RIGHT;

	//if ((pixel_y / 2) % 2 == 0 && (pixel_x / 2) % 2 == 0)
	//{
	//	quadrant = Nametable::Quadrant::TOP_LEFT;
	//}
	return quadrant;
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

void PPU::SetData(const uint8_t value)
{
	// https://wiki.nesdev.com/w/index.php/PPU_registers#PPUDATA
	m_ppu_data = value;
	NESConsole::GetInstance()->GetMemory().PPUWriteByte(m_ppu_full_addr, value);
	if (GetControlRegister().Bits.m_vram_add == 0)
	{
		m_ppu_full_addr += 1;
	}
	else
	{
		m_ppu_full_addr += 32;
	}
}
