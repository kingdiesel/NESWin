#include "Nametable.h"
#include "Source/NESConsole.h"
#include "Source/Globals.h"
#include "PatternTable.h"
#include "Source/PatternTableTile.h"
#include <assert.h>
#include "SDL.h"

Nametable::Nametable(const uint16_t address, PatternTable* pattern_table) :
	m_base_address(address),
	m_pattern_table(pattern_table)
{
	// https://wiki.nesdev.com/w/index.php/PPU_nametables
	m_texture_nametable_data = new uint32_t[NES_RES];
	m_attribute_table_data = new uint8_t[ATTR_BYTES];
	memset(m_texture_nametable_data, 0x0, NES_RES * sizeof(uint32_t));
	memset(m_attribute_table_data, 0x0, ATTR_BYTES);
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
	const uint16_t attribute_memory_end = attribute_memory_start + ATTR_BYTES;
	for (uint16_t i = attribute_memory_start; i < attribute_memory_end; ++i)
	{
		const int attribute_index = i - attribute_memory_start;
		assert(attribute_index >= 0 && attribute_index < ATTR_BYTES);
		m_attribute_table_data[attribute_index] = memory.PPUReadByte(i);
	}

	// https://wiki.nesdev.com/w/index.php/PPU_nametables
	for (uint16_t i = m_base_address; i < attribute_memory_start; ++i)
	{
		const int nametable_row = (i - m_base_address) / 32;
		const int nametable_col = (i - m_base_address) % 32;
		const int attribute_row = nametable_row / 4;
		const int attribute_col = nametable_col / 4;
		const int attribute_index = (attribute_row * 8) + attribute_col;
		assert(attribute_index >= 0 && attribute_index < ATTR_BYTES);
		uint8_t attribute_byte = m_attribute_table_data[attribute_index];
		const int attribute_sub_row = nametable_row / 2;
		const int attribute_sub_col = nametable_col / 2;;
		const uint8_t palette_index = GetPaletteIndexFromAttributeByte(attribute_sub_row, attribute_sub_col, attribute_byte);
		assert(palette_index >= 0 && palette_index < 4);

		uint8_t tile_id = memory.PPUReadByte(i);
		uint16_t tile_offset = pattern_table_id == 0 ? 0 : 256;
		PatternTableTile* tile = m_pattern_table->GetTile(tile_id + tile_offset);
		tile->FillTextureData(FillData(palette_index));
		const uint32_t* tile_texture_data = tile->GetTextureTileData();
		for (int j = 0; j < 8; ++j)
		{
			const int address = (8 * nametable_row * 256 + nametable_col * 8) + (j * 256);
			assert(address >= 0 && address < NES_RES);
			assert(j * 8 >= 0 && j * 8 < PatternTableTile::TILE_TEXTURE_SIZE);
			// TODO: why
			//if (address + 8 * sizeof(uint32_t) <= NES_RES)
			{
				memcpy(&m_texture_nametable_data[address], &tile_texture_data[j * 8], 8 * sizeof(uint32_t));
			}
		}
	}

	SDL_UpdateTexture(
		m_nametable_texture,
		nullptr,
		m_texture_nametable_data,
		256 * sizeof(Uint32)
	);
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
	if (row  % 2 == 0 && col % 2 == 0)
	{
		quadrant = Nametable::Quadrant::TOP_LEFT;
	}
	else if (row % 2 == 1 && col % 2 == 0)
	{
		quadrant = Nametable::Quadrant::BOTTOM_LEFT;
	}
	else if (row % 2 == 0 && col % 2 == 1)
	{
		quadrant = Nametable::Quadrant::TOP_RIGHT;
	}
	return quadrant;
}
