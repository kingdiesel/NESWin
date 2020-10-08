#include "Nametable.h"
#include "Source/NESConsole.h"
#include "PatternTable.h"
#include "Source/PatternTableTile.h"
#include <assert.h>
#include "SDL.h"

Nametable::Nametable(const uint16_t address, PatternTable* pattern_table) :
	m_base_address(address),
	m_pattern_table(pattern_table)
{
	// https://wiki.nesdev.com/w/index.php/PPU_nametables
	m_texture_nametable_data = new uint32_t[256*240];
	m_attribute_table_data = new uint8_t[64];
	memset(m_texture_nametable_data, 0x0, 256 * 240 * sizeof(uint32_t));
	memset(m_attribute_table_data, 0x0, 64);
}

void Nametable::Initialize(SDL_Renderer* renderer)
{
	assert(m_nametable_texture == nullptr);
	m_nametable_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC,
		256,
		240
	);
}

void Nametable::Run()
{
	const Memory& memory = NESConsole::GetInstance()->GetMemory();
	const PPU& ppu = NESConsole::GetInstance()->GetPPU();
	const PPUControlRegister& control_register = ppu.GetControlRegister();
	const uint8_t pattern_table_id = control_register.Bits.m_bkgrnd_pattern_addr;

	// fill attribute memory
	const uint16_t attribute_memory_start = m_base_address + 960;
	const uint16_t attribute_memory_end = m_base_address + 1024;
	for (uint16_t i = attribute_memory_start; i < attribute_memory_end; ++i)
	{
		m_attribute_table_data[i - attribute_memory_start] = memory.PPUReadByte(i);
	}


	// https://wiki.nesdev.com/w/index.php/PPU_nametables
	for (uint16_t i = m_base_address; i < attribute_memory_start; ++i)
	{
		int row = (i - m_base_address) / 32;
		int col = (i - m_base_address) % 32;
		uint8_t tile_id = memory.PPUReadByte(i);
		uint16_t tile_offset = pattern_table_id == 0 ? 0 : 256;
		PatternTableTile* tile = m_pattern_table->GetTile(tile_id + tile_offset);
		uint8_t attribute_byte = GetAttributeByte(row, col);
		uint8_t palette_index = GetPaletteIndexFromAttributeByte(row, col, attribute_byte);
		tile->FillTextureData(FillData(palette_index));
		const uint32_t* tile_texture_data = tile->GetTextureTileData();
		for (int i = 0; i < 8; ++i)
		{
			const int address = (8 * row * 256 + col * 8) + (i * 256);
			assert(address >= 0 && address < 256 * 240);
			memcpy(&m_texture_nametable_data[address], &tile_texture_data[i*8], 8*sizeof(uint32_t));
		}
	}

	SDL_UpdateTexture(
		m_nametable_texture,
		nullptr,
		m_texture_nametable_data,
		256 * sizeof(Uint32)
	);
}

uint8_t Nametable::GetAttributeIndex(
	const int row,
	const int col,
	int& out_row,
	int& out_col
)
{
	// https://wiki.nesdev.com/w/index.php/PPU_attribute_tables
	out_row = 8 * (row / 4);
	out_col = col / 4;
	const int attribute_index = out_row + out_col;
	return attribute_index;
}

uint8_t Nametable::GetAttributeByte(const int row, const int col)
{
	int out_row = 0;
	int out_col = 0;
	const int attribute_index = GetAttributeIndex(row, col, out_row, out_col);
	assert(attribute_index >= 0 && attribute_index < 64);
	return m_attribute_table_data[attribute_index];
}

uint8_t Nametable::GetPaletteIndexFromAttributeByte(
	const int row, 
	const int col, 
	const uint8_t attribute_byte
)
{
	// https://wiki.nesdev.com/w/index.php/PPU_attribute_tables
	uint8_t palette_index = 0;
	uint8_t shift = 0;
	Nametable::Quadrant quadrant = 
		GetQuadrantFromAttributeByte(row, col, attribute_byte);
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

Nametable::Quadrant Nametable::GetQuadrantFromAttributeByte(
	const int row, 
	const int col, 
	const uint8_t attribute_byte
)
{
	// https://wiki.nesdev.com/w/index.php/PPU_attribute_tables
	Nametable::Quadrant quadrant = Nametable::Quadrant::BOTTOM_RIGHT;
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
