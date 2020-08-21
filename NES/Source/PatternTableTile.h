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

	unsigned char *GetTileData();

	const unsigned char *GetTileData() const;

	struct SDL_Texture *GetTileTexture();

	void CreateTextureFromTileData(struct SDL_Renderer *renderer);

	static const int TILE_SIZE = 16;
private:
	PatternTableTile(const PatternTableTile &other)
	{}
	unsigned char m_tile_data[TILE_SIZE];
	uint32_t m_texture_tile_data[64];

	struct SDL_Texture *m_tile_texture = nullptr;
};

#endif //NES_PATTERNTABLETILE_H
