// NES.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Source/NESConsole.h"
#include "Source/PatternTableTile.h"
#include "SDL.h"

int main(int argv, char** args)
{
	
	//NESConsole::GetInstance()->RunTests();

	//NESConsole console;
	NESConsole::GetInstance()->LoadROM("C:/Users/aspiv/source/repos/NES/NES/TestRoms/nestest.nes");

	//console.RunROM();
	const int nes_resolution_x = 256;
	const int nes_resolution_y = 240;
	const int fps = 60;
	const int screen_scale = 1;
	const int sdl_wait = static_cast<int>(1000.0f / (float)fps);
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(
		"NES",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		nes_resolution_x * screen_scale,
		nes_resolution_y * screen_scale,
		0
	);

	renderer = SDL_CreateRenderer(
		window, 
		-1, 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	int returnCode = SDL_RenderSetLogicalSize(
		renderer, 
		nes_resolution_x * screen_scale, 
		nes_resolution_y * screen_scale
	);

	returnCode = SDL_SetRenderDrawColor(
		renderer, 
		0x00, 
		0x00, 
		0x00, 
		0xFF
	);

	SDL_Texture* pattern_table_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		nes_resolution_x,
		nes_resolution_y
	);

	Uint32 format = SDL_PIXELFORMAT_ARGB8888;
	SDL_PixelFormat* mapping_format = SDL_AllocFormat(format);

	const ROM& rom = NESConsole::GetInstance()->GetROM();
	const int num_tiles = rom.GetHeaderData().chr_rom_size_8 * 8 * 1024;
	std::vector<PatternTableTile*> tiles;
	tiles.resize(num_tiles);
	int tile_count = 0;
	for (int i = 0; i < num_tiles; i += 16, ++tile_count)
	{
		tiles[tile_count] = new PatternTableTile();
		rom.GetTile(i, tiles[tile_count]->GetTileData(), PatternTableTile::TILE_SIZE);
		tiles[tile_count]->CreateTextureFromTileData(renderer);
	}

	bool quit = false;
	SDL_Event event;
	while (!quit)
	{
		const Uint32 frame_start = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			}
		}

		//	bug if frame takes too long to render, need double buffer
		SDL_RenderClear(renderer);
		for (int i = 0; i < tile_count; ++i)
		{
			SDL_Rect dest_rect;
			dest_rect.x = (i * 16) % nes_resolution_x;
			dest_rect.y = ((i * 16) / nes_resolution_x) * 16;
			dest_rect.h = 16;
			dest_rect.w = 16;
			SDL_RenderCopy(renderer, tiles[i]->GetTileTexture(), nullptr, &dest_rect);
		}

		SDL_RenderPresent(renderer);

		const Uint32 frame_time = SDL_GetTicks() - frame_start;
		if (frame_time < sdl_wait)
		{
			SDL_Delay(sdl_wait - frame_time);
		}
	}

	SDL_FreeFormat(mapping_format);
	SDL_DestroyTexture(pattern_table_texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
