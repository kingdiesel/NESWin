#include "PatternTable.h"
#include "Source/PatternTableTile.h"
#include "Source/NESConsole.h"
#include <assert.h>
#include "SDL.h"
static const int TILE_COUNT = 8192 / 16;
void PatternTable::Initialize(struct SDL_Renderer* renderer)
{
	assert(m_pattern_table_texture == nullptr);
	assert(m_tiles == nullptr);
	m_tiles = new PatternTableTile[TILE_COUNT];

	m_pattern_table_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC,
		128,
		256
	);

	Memory& memory = NESConsole::GetInstance()->GetMemory();

	SDL_Rect dest_rect;
	for (int i = 0; i < 8192; i += 16)
	{
		int tile_count = i / 16;
		for (int j = 0; j < PatternTableTile::TILE_SIZE; ++j)
		{
			m_tiles[tile_count].GetTileData()[j] = memory.PPUReadByte(i + j);
		}
		m_tiles[tile_count].FillTextureData();
		uint32_t* texture_tile_data = m_tiles[tile_count].GetTextureTileData();
		dest_rect.x = (tile_count * 8) % 128;
		dest_rect.y = ((tile_count * 8) / 128) * 8;
		dest_rect.h = 8;
		dest_rect.w = 8;
		SDL_UpdateTexture(
			m_pattern_table_texture,
			&dest_rect,
			texture_tile_data,
			8 * sizeof(Uint32)
		);
	}
}

PatternTableTile* PatternTable::GetTile(int index)
{
	assert(index >= 0 && index < TILE_COUNT);
	return &m_tiles[index];
}
