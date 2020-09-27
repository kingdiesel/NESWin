#include "Sprites.h"
#include "Source/NESConsole.h"
#include "PatternTable.h"
#include "Source/PatternTableTile.h"
#include <assert.h>
#include "SDL.h"

Sprites::Sprites(class PatternTable* pattern_table):
	m_pattern_table(pattern_table)
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
	memset(m_texture_sprites_data, 0x0, 256 * 240 * sizeof(uint32_t));
	const uint8_t* oam = NESConsole::GetInstance()->GetMemory().GetPrimaryOAM();
	PPU& ppu = NESConsole::GetInstance()->GetPPU();
	const PPUControlRegister& control_register = ppu.GetControlRegister();
	const uint8_t pattern_table_id = control_register.Bits.m_sprite_pattern_addr;
	for (int i = 0; i < 256; i +=4)
	{
		// https://wiki.nesdev.com/w/index.php/PPU_OAM#Byte_0
		// Y position of top of sprite
		const uint8_t byte0 = oam[i];
		if (byte0 > 239)
		{
			continue;
		}
		// https://wiki.nesdev.com/w/index.php/PPU_OAM#Byte_1
		// 76543210
		// ||||||||
		// |||||||+-Bank($0000 or $1000) of tiles
		// ++++++ + --Tile number of top of sprite(0 to 254; bottom half gets the next tile)
		const uint8_t byte1 = oam[i+1];
		const bool is8x16sprite = ppu.GetControlRegister().Bits.m_sprite_size == 1;
		uint8_t tile_id = 0;
		uint16_t tile_offset = 0;
		GetTileDataFromByte1(byte1, is8x16sprite, pattern_table_id, tile_id, tile_offset);

		// https://wiki.nesdev.com/w/index.php/PPU_OAM#Byte_2
		// 76543210
		// ||||||||
		// ||||||++ - Palette(4 to 7) of sprite
		// |||++ + -- - Unimplemented
		// || +------Priority(0: in front of background; 1: behind background)
		// | +------ - Flip sprite horizontally
		// + --------Flip sprite vertically
		const uint8_t byte2 = oam[i+2];
		const uint8_t palette_index = GetPaletteIndexFromByte2(byte2);
		bool flip_horizontally = false;
		bool flip_vertically = false;
		GetFlipFromByte2(byte2, flip_horizontally, flip_vertically);

		// https://wiki.nesdev.com/w/index.php/PPU_OAM#Byte_3
		// X position of left side of sprite.
		const uint8_t byte3 = oam[i+3];
		PatternTableTile* tile1 = m_pattern_table->GetTile(tile_id + tile_offset);
		PatternTableTile* tile2 = nullptr;
		if (is8x16sprite)
		{
			tile2 = m_pattern_table->GetTile(tile_id + tile_offset + 1);
		}
		FillData fill_data(palette_index, flip_horizontally, flip_vertically);
		tile1->FillTextureData(fill_data);
		if (tile2 != nullptr)
		{
			tile2->FillTextureData(fill_data);
		}

		uint8_t y_pos = byte0 + 1;
		const uint8_t x_pos = byte3;
		if (is8x16sprite)
		{
			uint32_t* top_tile = tile1->GetTextureTileData();
			uint32_t* bottom_tile = tile2->GetTextureTileData();
			if (flip_vertically)
			{
				top_tile = tile2->GetTextureTileData();
				bottom_tile = tile1->GetTextureTileData();
			}

			for (int i = 0; i < 8; ++i)
			{
				const int address = (y_pos * 256 + x_pos) + (i * 256);
				assert(address >= 0 && address < 256 * 240);
				memcpy(&m_texture_sprites_data[address], &top_tile[i * 8], 8 * sizeof(uint32_t));
			}

			y_pos += 8;
			for (int i = 0; i < 8; ++i)
			{
				const int address = (y_pos * 256 + x_pos) + (i * 256);
				assert(address >= 0 && address < 256 * 240);
				memcpy(&m_texture_sprites_data[address], &bottom_tile[i * 8], 8 * sizeof(uint32_t));
			}
		}
		else
		{
			const uint32_t* tile_texture_data = tile1->GetTextureTileData();
			for (int i = 0; i < 8; ++i)
			{
				const int address = (y_pos * 256 + x_pos) + (i * 256);
				if (address >= 256 * 240)
				{
					continue;
				}
				assert(address >= 0 && address < 256 * 240);
				memcpy(&m_texture_sprites_data[address], &tile_texture_data[i * 8], 8 * sizeof(uint32_t));
			}
		}
	}

	SDL_UpdateTexture(
		m_sprites_texture,
		nullptr,
		m_texture_sprites_data,
		256 * sizeof(Uint32)
	);
}

void Sprites::GetTileDataFromByte1(
	const uint8_t byte1,
	const bool is8x16sprite,
	const uint8_t pattern_table_id,
	uint8_t& tile_id,
	uint16_t& tile_offset
)
{
	// https://wiki.nesdev.com/w/index.php/PPU_OAM#Byte_1
	// 76543210
	// ||||||||
	// |||||||+-Bank($0000 or $1000) of tiles
	// ++++++ + --Tile number of top of sprite(0 to 254; bottom half gets the next tile)
	if (is8x16sprite)
	{
		tile_id = byte1 & 0xFE;
		tile_offset = (byte1 & 0x01) == 0 ? 0 : 256;
	}
	else
	{
		tile_id = byte1;
		tile_offset = pattern_table_id == 0 ? 0 : 256;
	}
}

uint8_t Sprites::GetPriorityFromByte2(const uint8_t byte2)
{
	// https://wiki.nesdev.com/w/index.php/PPU_OAM#Byte_2
	// 76543210
	// ||||||||
	// ||||||++ - Palette(4 to 7) of sprite
	// |||++ + -- - Unimplemented
	// || +------Priority(0: in front of background; 1: behind background)
	// | +------ - Flip sprite horizontally
	// + --------Flip sprite vertically
	return (byte2 & 0x20) == 0 ? 0 : 1;
}

uint8_t Sprites::GetPaletteIndexFromByte2(const uint8_t byte2)
{
	// https://wiki.nesdev.com/w/index.php/PPU_OAM#Byte_2
	// 76543210
	// ||||||||
	// ||||||++ - Palette(4 to 7) of sprite
	// |||++ + -- - Unimplemented
	// || +------Priority(0: in front of background; 1: behind background)
	// | +------ - Flip sprite horizontally
	// + --------Flip sprite vertically
	const uint8_t palette_bits = (byte2 & 0x3) + 4;
	return palette_bits;
}

void Sprites::GetFlipFromByte2(const uint8_t byte2, bool& flip_horizontally, bool& flip_vertically)
{
	// https://wiki.nesdev.com/w/index.php/PPU_OAM#Byte_2
	// 76543210
	// ||||||||
	// ||||||++ - Palette(4 to 7) of sprite
	// |||++ + -- - Unimplemented
	// || +------Priority(0: in front of background; 1: behind background)
	// | +------ - Flip sprite horizontally
	// + --------Flip sprite vertically
	flip_horizontally = byte2 & (1 << 6);
	flip_vertically = byte2 & (1 << 7);
}
