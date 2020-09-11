#pragma once
#include <cstdint>

class Sprites
{
public:
	Sprites();
	void Run();
	void Initialize(struct SDL_Renderer* renderer);
	struct SDL_Texture* GetTexture() { return m_sprites_texture; }
private:
	struct SDL_Texture* m_sprites_texture = nullptr;
	uint32_t* m_texture_sprites_data = nullptr;
};

