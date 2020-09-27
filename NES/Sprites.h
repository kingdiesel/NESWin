#pragma once
#include <cstdint>

class Sprites
{
public:
	Sprites(class PatternTable* pattern_table);
	void Run();
	void Initialize(struct SDL_Renderer* renderer);
	struct SDL_Texture* GetTexture() { return m_sprites_texture; }
	static void GetTileDataFromByte1(
		const uint8_t byte1,
		const bool is8x16sprite,
		const uint8_t pattern_table_id,
		uint8_t& tile_id,
		uint16_t& tile_offset
	);
	static uint8_t GetPriorityFromByte2(const uint8_t byte2);
	static uint8_t GetPaletteIndexFromByte2(const uint8_t byte2);
	static void GetFlipFromByte2(
		const uint8_t byte2,
		bool& flip_horizontally,
		bool& flip_vertically
	);
private:
	struct SDL_Texture* m_sprites_texture = nullptr;
	uint32_t* m_texture_sprites_data = nullptr;
	class PatternTable* m_pattern_table;
};

