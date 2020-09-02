//
//

#ifndef NES_PATTERNTABLETILE_H
#define NES_PATTERNTABLETILE_H

#include <cstdint>

class PatternTableTile
{
public:
	PatternTableTile();

	~PatternTableTile();

	uint8_t *GetTileData();

	const uint8_t *GetTileData() const;

	uint32_t* GetTextureTileData() { return m_texture_tile_data; }

	struct SDL_Texture *GetTileTexture();

	void CreateTextureFromTileData(struct SDL_Renderer *renderer);

	static const int TILE_SIZE = 16;
private:
	PatternTableTile(const PatternTableTile &other)
	{}
	uint8_t m_tile_data[TILE_SIZE];
	uint32_t m_texture_tile_data[64];

	struct SDL_Texture *m_tile_texture = nullptr;
};

#endif //NES_PATTERNTABLETILE_H
