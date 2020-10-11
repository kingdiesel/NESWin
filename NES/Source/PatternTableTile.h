#pragma once
#include <memory>
#include <cstdint>
#include "NESConsole.h"

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
	static constexpr int TILE_SIZE = 16;
	static constexpr int TILE_TEXTURE_SIZE = 64;
private:
	uint8_t m_tile_data[TILE_SIZE];
	uint32_t m_texture_tile_data[TILE_TEXTURE_SIZE];
	std::shared_ptr<NESConsole> m_console;
};