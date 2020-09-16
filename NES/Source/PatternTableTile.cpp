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
	m_console = NESConsole::GetInstance().get();
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

void PatternTableTile::FillTextureData(const FillData& _data)
{
	// https://lospec.com/palette-list/2-bit-grayscale
	uint32_t COLOR_PALETTE[4] =
	{
		0x000000,
		0xffffff,
		0xb6b6b6,
		0x676767,
	};

	int start_address = 0;
	switch (_data.m_palette_index)
	{
	case 0:
		start_address = 0x3F01;
		break;
	case 1:
		start_address = 0x3F05;
		break;
	case 2:
		start_address = 0x3F09;
		break;
	case 3:
		start_address = 0x3F0D;
		break;
	case 4:
		start_address = 0x3F11;
		break;
	case 5:
		start_address = 0x3F15;
		break;
	case 6:
		start_address = 0x3F19;
		break;
	case 7:
		start_address = 0x3F1D;
		break;
	default:
		break;
	}

	Memory& memory = m_console->GetMemory();
	for (int i = 1; i <= 3 && _data.m_palette_index != -1; ++i)
	{
		uint8_t color = memory.PPUReadByte(start_address + i - 1);
		uint32_t palette_color = PaletteColors[color];
		COLOR_PALETTE[i] = palette_color;
	}

	// https://wiki.nesdev.com/w/index.php/PPU_pattern_tables
	for (int row = 0; row < 8; ++row)
	{
		uint8_t plane_zero_row = m_tile_data[row];
		uint8_t plane_one_row = m_tile_data[row + 8];
		for (int bit = 7; bit >= 0; --bit)
		{
			uint8_t shifted_row_zero = (plane_zero_row >> bit) & 0x01;
			uint8_t shifted_row_one = (plane_one_row >> bit) & 0x01;
			uint8_t color_bit = 0;
			switch (shifted_row_zero)
			{
			case 0:
				switch (shifted_row_one)
				{
				case 0:
					color_bit = 0;
					break;
				case 1:
					color_bit = 2;
					break;
				}
				break;
			case 1:
				switch (shifted_row_one)
				{
				case 0:
					color_bit = 1;
					break;
				case 1:
					color_bit = 3;
					break;
				}
				break;
			}
			assert(color_bit < 4);
			if (_data.m_flip_horizontally)
			{
				m_texture_tile_data[(row << 3) + bit] = COLOR_PALETTE[color_bit];
			}
			else if (_data.m_flip_vertically)
			{
				m_texture_tile_data[((7 - row) << 3) + (7 - bit)] = COLOR_PALETTE[color_bit];
			}
			else
			{
				m_texture_tile_data[(row << 3) + (7 - bit)] = COLOR_PALETTE[color_bit];
			}
		}
	}
}

void PatternTableTile::GetTextureDataRow(int row, uint32_t* row_data, const FillData& _data)
{
	// https://lospec.com/palette-list/2-bit-grayscale
	static uint32_t COLOR_PALETTE[4] =
	{
		0x000000,
		0xffffff,
		0xb6b6b6,
		0x676767,
	};

	int start_address = 0;
	switch (_data.m_palette_index)
	{
	case 0:
		start_address = 0x3F01;
		break;
	case 1:
		start_address = 0x3F05;
		break;
	case 2:
		start_address = 0x3F09;
		break;
	case 3:
		start_address = 0x3F0D;
		break;
	case 4:
		start_address = 0x3F11;
		break;
	case 5:
		start_address = 0x3F15;
		break;
	case 6:
		start_address = 0x3F19;
		break;
	case 7:
		start_address = 0x3F1D;
		break;
	default:
		break;
	}

	Memory& memory = m_console->GetMemory();
	for (int i = 1; i <= 3 && _data.m_palette_index != -1; ++i)
	{
		uint8_t color = memory.PPUReadByte(start_address + i - 1);
		uint32_t palette_color = PaletteColors[color];
		COLOR_PALETTE[i] = palette_color;
	}

	if (_data.m_flip_vertically)
	{
		row = 7 - row;
	}
	uint8_t plane_zero_row = m_tile_data[row];
	uint8_t plane_one_row = m_tile_data[row + 8];
	for (int bit = 7; bit >= 0; --bit)
	{
		uint8_t shifted_row_zero = (plane_zero_row >> bit) & 0x01;
		uint8_t shifted_row_one = (plane_one_row >> bit) & 0x01;
		uint8_t color_bit = 0;
		switch (shifted_row_zero)
		{
		case 0:
			switch (shifted_row_one)
			{
			case 0:
				color_bit = 0;
				break;
			case 1:
				color_bit = 2;
				break;
			}
			break;
		case 1:
			switch (shifted_row_one)
			{
			case 0:
				color_bit = 1;
				break;
			case 1:
				color_bit = 3;
				break;
			}
			break;
		}
		assert(color_bit < 4);
		if (_data.m_flip_horizontally)
		{
			row_data[bit] = COLOR_PALETTE[color_bit];
		}
		else
		{
			row_data[7 - bit] = COLOR_PALETTE[color_bit];;
		}
	}
}
