#pragma once
#include <cstdint>

class Nametable
{
public:
	enum class Quadrant
	{
		TOP_LEFT,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT
	};
	Nametable(const uint16_t address, class PatternTable* pattern_table);
	void Initialize(struct SDL_Renderer* renderer);
	void Run();
	struct SDL_Texture* GetTexture() { return m_nametable_texture; }
private:
	uint8_t GetAttributeByte(const int row, const int col);
	uint8_t GetAttributeIndex(
		const int row,
		const int col,
		int& out_row,
		int& out_col
	);
	uint8_t GetPaletteIndexFromAttributeByte(
		const int row, 
		const int col, 
		const uint8_t attribute_byte
	);
	
	Quadrant GetQuadrantFromAttributeByte(
		const int row,
		const int col,
		const uint8_t attribute_byte
	);
	struct SDL_Texture* m_nametable_texture = nullptr;
	uint32_t* m_texture_nametable_data = nullptr;
	uint8_t* m_attribute_table_data = nullptr;
	uint16_t m_base_address = 0;
	class PatternTable* m_pattern_table;
};

