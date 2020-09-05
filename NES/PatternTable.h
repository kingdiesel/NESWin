#pragma once
class PatternTable
{
public:
	PatternTable() = default;

	void Initialize(struct SDL_Renderer* renderer);
	struct SDL_Texture* GetTexture() { return m_pattern_table_texture; }
	const class PatternTableTile* GetTile(int index) const;
private:
	struct SDL_Texture* m_pattern_table_texture = nullptr;
	class PatternTableTile* m_tiles = nullptr;
};

