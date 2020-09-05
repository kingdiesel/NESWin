#include "Nametable.h"
#include "Source/NESConsole.h"
#include "PatternTable.h"
#include "Source/PatternTableTile.h"
#include <assert.h>
#include "SDL.h"

Nametable::Nametable(const uint16_t address, PatternTable* pattern_table) :
	m_base_address(address),
	m_pattern_table(pattern_table)
{
	// https://wiki.nesdev.com/w/index.php/PPU_nametables
	m_texture_nametable_data = new uint32_t[256*240];
	m_attribute_table_data = new uint8_t[64];

}

void Nametable::Initialize(SDL_Renderer* renderer)
{
	assert(m_nametable_texture == nullptr);
	//m_nametable_texture = SDL_CreateTexture(
	//	renderer,
	//	SDL_PIXELFORMAT_ARGB8888,
	//	SDL_TEXTUREACCESS_STREAMING,
	//	256,
	//	240
	//);
	m_nametable_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC,
		256,
		240
	);
}

void Nametable::Run()
{
	const Memory& memory = NESConsole::GetInstance()->GetMemory();
	const PPU& ppu = NESConsole::GetInstance()->GetPPU();
	const PPUControlRegister& control_register = ppu.GetControlRegister();
	const uint8_t pattern_table_id = control_register.Bits.m_bkgrnd_pattern_addr;

	// fill attribute memory
	const uint16_t attribute_memory_start = m_base_address + 960;
	const uint16_t attribute_memory_end = m_base_address + 1024;
	for (uint16_t i = attribute_memory_start; i < attribute_memory_end; ++i)
	{
		m_attribute_table_data[i - attribute_memory_start] = memory.PPUReadByte(i);
	}

	//uint32_t* pixels = nullptr;
	//int pitch = 0;
	//int return_code = SDL_LockTexture(m_nametable_texture,
	//	nullptr,
	//	reinterpret_cast<void**>(&pixels),
	//	&pitch
	//);
	//int pixel_index = 0;
	SDL_Rect dest_rect;
	// https://wiki.nesdev.com/w/index.php/PPU_nametables
	for (uint16_t i = m_base_address; i < attribute_memory_start; ++i)
	{
		int row = (i - m_base_address) / 32;
		int col = (i - m_base_address) % 32;
		uint8_t tile_id = memory.PPUReadByte(i);
		uint16_t tile_offset = pattern_table_id == 0 ? 0 : 256;
		const PatternTableTile* tile = m_pattern_table->GetTile(tile_id + tile_offset);
		const uint32_t* tile_texture_data = tile->GetTextureTileData();
		//memcpy(&pixels[pixel_index], tile_texture_data, 64*4);
		dest_rect.x = col * 8;
		dest_rect.y = row * 8;
		dest_rect.h = 8;
		dest_rect.w = 8;
		SDL_UpdateTexture(
			m_nametable_texture,
			&dest_rect,
			tile_texture_data,
			8 * sizeof(Uint32)
		);
		//pixel_index += 64;
	}

	//SDL_UnlockTexture(m_nametable_texture);
}
