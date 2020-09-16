//
//

#ifndef NES_PATTERNTABLETILE_H
#define NES_PATTERNTABLETILE_H

#include <cstdint>

struct FillData
{
	FillData() = default;
	FillData(const int palette_index) :
		m_palette_index(palette_index)
	{}
	FillData(
		const int palette_index,
		const bool flip_horizontally,
		const bool flip_vertically) :
		m_palette_index(palette_index),
		m_flip_horizontally(flip_horizontally),
		m_flip_vertically(flip_vertically)
	{}
	int m_palette_index = -1;
	bool m_flip_horizontally = false;
	bool m_flip_vertically = false;
};

class PatternTableTile
{
public:
	PatternTableTile();

	~PatternTableTile();

	uint8_t *GetTileData();

	const uint8_t *GetTileData() const;

	uint32_t* GetTextureTileData() { return m_texture_tile_data; }
	const uint32_t* GetTextureTileData() const { return m_texture_tile_data; }

	void FillTextureData(const FillData& _data);
	void GetTextureDataRow(int row, uint32_t* row_data, const FillData& _data);
	static const int TILE_SIZE = 16;
private:
	PatternTableTile(const PatternTableTile &other)
	{}
	uint8_t m_tile_data[TILE_SIZE];
	uint32_t m_texture_tile_data[64];
	class NESConsole* m_console = nullptr;
};

#endif //NES_PATTERNTABLETILE_H
