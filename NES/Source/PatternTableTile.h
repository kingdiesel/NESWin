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
	const uint32_t* GetTextureTileData() const { return m_texture_tile_data; }

	void FillTextureData(int palette = -1);

	static const int TILE_SIZE = 16;
private:
	PatternTableTile(const PatternTableTile &other)
	{}
	uint8_t m_tile_data[TILE_SIZE];
	uint32_t m_texture_tile_data[64];
	class NESConsole* m_console = nullptr;
};

#endif //NES_PATTERNTABLETILE_H
