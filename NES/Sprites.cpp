#include "Sprites.h"
#include "Source/NESConsole.h"
#include <assert.h>
#include "SDL.h"

Sprites::Sprites()
{
	m_texture_sprites_data = new uint32_t[256 * 240];
	memset(m_texture_sprites_data, 0x0, 256 * 240 * sizeof(uint32_t));
}

void Sprites::Initialize(SDL_Renderer* renderer)
{
	assert(m_sprites_texture == nullptr);
	m_sprites_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC,
		256,
		240
	);
}

void Sprites::Run()
{
	SDL_UpdateTexture(
		m_sprites_texture,
		nullptr,
		m_texture_sprites_data,
		256 * sizeof(Uint32)
	);
}