//
//
#include <cstring>
#include <assert.h>
#include "PatternTableTile.h"
#include "SDL.h"

PatternTableTile::PatternTableTile()
{
	memset(m_tile_data, 0x00, TILE_SIZE);
	memset(m_texture_tile_data, 0x00, sizeof(m_texture_tile_data));
	m_tile_texture = nullptr;
}

PatternTableTile::~PatternTableTile()
{
	if (m_tile_texture != nullptr)
	{
		SDL_DestroyTexture(m_tile_texture);
	}
}

uint8_t *PatternTableTile::GetTileData()
{
	return m_tile_data;
}

const uint8_t *PatternTableTile::GetTileData() const
{
	return m_tile_data;
}

SDL_Texture *PatternTableTile::GetTileTexture()
{
	return m_tile_texture;
}

void PatternTableTile::CreateTextureFromTileData(SDL_Renderer *renderer)
{
	assert(m_tile_texture == nullptr);
	m_tile_texture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			8,
			8
	);

	for (int row = 0; row < 8; ++row)
	{
		uint8_t plane_zero_row = m_tile_data[row];
		uint8_t plane_one_row = m_tile_data[row + 8];
		for (int bit = 7; bit >= 0; --bit)
		{
			uint8_t shifted_row_zero = (plane_zero_row >> bit) & (uint8_t) 0x01;
			uint8_t shifted_row_one = (plane_one_row >> bit) & (uint8_t) 0x01;
			uint8_t color_bit = shifted_row_zero + shifted_row_one;
			assert(color_bit < 4);
			m_texture_tile_data[row * 8 + (7 - bit)] = color_bit == 0 ? 0xFF000000 : 0xFF00FFFF;
		}
	}

	SDL_UpdateTexture(m_tile_texture, nullptr, m_texture_tile_data, 8 * sizeof(Uint32));
}