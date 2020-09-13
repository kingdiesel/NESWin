#pragma once
#include <cstdint>

class Sprites
{
public:
	Sprites(class PatternTable* pattern_table);
	void Run();
	void Initialize(struct SDL_Renderer* renderer);
	struct SDL_Texture* GetTexture() { return m_sprites_texture; }
private:
	uint8_t GetPaletteIndexFromByte2(const uint8_t byte2);
	void GetFlipFromByte2(
		const uint8_t byte2,
		bool& flip_horizontally,
		bool& flip_vertically
	);
	struct SDL_Texture* m_sprites_texture = nullptr;
	uint32_t* m_texture_sprites_data = nullptr;
	class PatternTable* m_pattern_table;
};

