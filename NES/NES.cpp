// NES.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Source/NESConsole.h"
#include "Source/PatternTableTile.h"
#include "SDL.h"
#include <iomanip>

int main(int argv, char** args)
{	
	//NESConsole::GetInstance()->RunTests();
	//return 0;

	//NESConsole console;
	NESConsole::GetInstance()->LoadROM(
		"C:/Users/aspiv/source/repos/NES/NES/TestRoms/nestest.nes"
		//"C:/Users/aspiv/source/repos/NES/NES/TestRoms/color_test.nes"
		//"C:/Users/aspiv/source/repos/NES/NES/TestRoms/dk.nes"
	);

	//NESConsole::GetInstance()->RunROM();
	//const int nes_resolution_x = 256;
	//const int nes_resolution_y = 240;
	const int nes_resolution_x = 256;
	const int nes_resolution_y = 512;
	const int fps = 60;
	const int screen_scale = 1;
	const int sdl_wait = static_cast<int>(1000.0f / (float)fps);
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(
		"Pattern Table",
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

	Uint32 format = SDL_PIXELFORMAT_ARGB8888;
	SDL_PixelFormat* mapping_format = SDL_AllocFormat(format);
	const ROM& rom = NESConsole::GetInstance()->GetROM();

	// Palletes
	/*
		// https://wiki.nesdev.com/w/index.php/PPU_palettes
		$3F00		Universal background color (16128)
		$3F01-$3F03	Background palette 0
		$3F05-$3F07	Background palette 1
		$3F09-$3F0B	Background palette 2
		$3F0D-$3F0F	Background palette 3
		$3F11-$3F13	Sprite palette 0
		$3F15-$3F17	Sprite palette 1
		$3F19-$3F1B	Sprite palette 2
		$3F1D-$3F1F	Sprite palette 3

		Each palette has three colors
		Each color takes up one byte

		// https://en.wikipedia.org/wiki/8-bit_color
		Bit    7  6  5  4  3  2  1  0
		Data   R  R  R  G  G  G  B  B
	*/

	// Pattern tables
	const int num_tiles = rom.GetHeaderData().chr_rom_size_8 * 8 * 1024;
	std::vector<PatternTableTile*> tiles;
	tiles.resize(num_tiles);

	//  $0000-$0FFF, nicknamed "left" 0 - 4095
	//	$1000-$1FFF, nicknamed "right" 4096 - 8191

	// this loops over the whole range (0-8191), out of laziness
	// "left" and "right" will actually be displayed as "top"
	// and "bottom"
	int tile_count = 0;
	for (int i = 0; i < num_tiles; i += 16, ++tile_count)
	{
		tiles[tile_count] = new PatternTableTile();
		for (int j = 0; j < 16; ++j)
		{
			tiles[tile_count]->GetTileData()[j] = NESConsole::GetInstance()->GetMemory().PPUReadByte(i + j);
		}
		tiles[tile_count]->CreateTextureFromTileData(renderer);
	}

	returnCode = SDL_SetRenderDrawColor(
		renderer,
		0x00,
		0x00,
		0x00,
		0xFF
	);

	NESConsole::GetInstance()->GetCPU().PowerUp();
	NESConsole::GetInstance()->GetCPU().SetLoggingEnabled(true);
	NESConsole::GetInstance()->GetCPU().SetRegisterProgramCounter(
		NESConsole::GetInstance()->GetMemory().CPUReadShort(0xFFFC)
	);

	bool quit = false;
	bool paused = false;
	SDL_Event event;
	while (!quit)
	{
		const Uint32 frame_start = SDL_GetTicks();
		bool n_pressed = false;
		bool v_pressed = false;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_n)
				{
					n_pressed = true;
				}
				else if (event.key.keysym.sym == SDLK_p)
				{
					paused = !paused;
				}
				else if (event.key.keysym.sym == SDLK_v)
				{
					v_pressed = true;
				}
				break;
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
			SDL_RenderCopy(
				renderer, 
				tiles[i]->GetTileTexture(), 
				nullptr, 
				&dest_rect
			);
		}
		if (v_pressed)
		{
			PPUStatusRegister status = NESConsole::GetInstance()->GetPPU().GetStatusRegister();
			status.Bits.m_vertical_blank_started = 
				status.Bits.m_vertical_blank_started == 1 ? 0 : 1;
			NESConsole::GetInstance()->GetPPU().SetStatusRegister(status.Register);
			v_pressed = false;
		}
		if (n_pressed)
		{
			n_pressed = false;
			for (int i = 0x3F00; i <= 0x3F1f; ++i)
			{
				uint8_t color = NESConsole::GetInstance()->GetMemory().PPUReadByte(i);
				std::cout << "color: 0x" << std::uppercase << std::hex << std::setw(4) << std::setfill('0')
					<< static_cast<uint16_t>(color) << std::endl;
			}
		}
		for (int i = 0; i < 600 && !paused; ++i)
		{
			NESConsole::GetInstance()->GetCPU().ExecuteInstruction();
		}

		SDL_RenderPresent(renderer);

		const Uint32 frame_time = SDL_GetTicks() - frame_start;
		if (frame_time < sdl_wait)
		{
			SDL_Delay(sdl_wait - frame_time);
		}
	}

	SDL_FreeFormat(mapping_format);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return 0;
}