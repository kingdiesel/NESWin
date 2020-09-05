//
//
#include <cstring>
#include <assert.h>
#include "PatternTableTile.h"
#include "SDL.h"
#include "NESConsole.h"
#include "../Palette.h"

PatternTableTile::PatternTableTile()
{
	memset(m_tile_data, 0x00, TILE_SIZE);
	memset(m_texture_tile_data, 0x00, sizeof(m_texture_tile_data));
}

PatternTableTile::~PatternTableTile()
{
}

uint8_t *PatternTableTile::GetTileData()
{
	return m_tile_data;
}

const uint8_t *PatternTableTile::GetTileData() const
{
	return m_tile_data;
}

void PatternTableTile::FillTextureData(bool use_palette)
{
	// https://lospec.com/palette-list/2-bit-grayscale
	uint32_t COLOR_PALETTE[4] =
	{
		0x000000,
		0x676767,
		0xb6b6b6,
		0xffffff,
	};

	if (use_palette)
	{
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		for (int i = 0x3F00; i <= 0x3F03; ++i)
		{
			uint8_t color = memory.PPUReadByte(0x3F00);
			uint32_t palette_color = PaletteColors[color];
			COLOR_PALETTE[i & 0x00FF] = palette_color;
		}
	}

	for (int row = 0; row < 8; ++row)
	{
		uint8_t plane_zero_row = m_tile_data[row];
		uint8_t plane_one_row = m_tile_data[row + 8];
		for (int bit = 7; bit >= 0; --bit)
		{
			uint8_t shifted_row_zero = (plane_zero_row >> bit) & (uint8_t)0x01;
			uint8_t shifted_row_one = (plane_one_row >> bit) & (uint8_t)0x01;
			uint8_t color_bit = shifted_row_zero + shifted_row_one;
			assert(color_bit < 4);
			m_texture_tile_data[row * 8 + (7 - bit)] = COLOR_PALETTE[color_bit];
		}
	}
}
