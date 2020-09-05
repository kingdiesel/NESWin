// NES.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Source/NESConsole.h"
#include "Source/PatternTableTile.h"
#include "PatternTable.h"
#include "Palette.h"
#include "SDL.h"
#include <iomanip>
#include "Nametable.h"

int main(int argv, char** args)
{	
	//NESConsole::GetInstance()->RunNesTestTiming();
	//NESConsole::GetInstance()->RunNesTest();
	//return 0;

	//NESConsole console;
	NESConsole::GetInstance()->LoadROM(
		//"C:/Users/aspiv/source/repos/NES/NES/TestRoms/nestest.nes"
		//"C:/Users/aspiv/source/repos/NES/NES/TestRoms/color_test.nes"
		"C:/Users/aspiv/source/repos/NES/NES/TestRoms/dk.nes"
	);

	//NESConsole::GetInstance()->RunROM();
	const int screen_scale = 1;
	const int nes_resolution_x = 256;
	const int nes_resolution_y = 240;
	const int pattern_render_area_x = 128;
	const int pattern_render_area_y = 256;
	const int palette_height = 16;
	const int window_x = pattern_render_area_x + nes_resolution_x;
	const int window_y = pattern_render_area_y + palette_height;
	const int fps = 60;
	const int sdl_wait = static_cast<int>(1000.0f / (float)fps);
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(
		"NES",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		window_x * screen_scale,
		window_y * screen_scale,
		0
	);

	renderer = SDL_CreateRenderer(
		window, 
		-1, 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	int return_code = SDL_RenderSetLogicalSize(
		renderer, 
		window_x * screen_scale,
		window_y * screen_scale
	);

	Uint32 format = SDL_PIXELFORMAT_ARGB8888;
	SDL_PixelFormat* mapping_format = SDL_AllocFormat(format);
	const ROM& rom = NESConsole::GetInstance()->GetROM();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	PPU& ppu = NESConsole::GetInstance()->GetPPU();
	// Pattern tables
	//const int num_tiles = rom.GetHeaderData().chr_rom_size_8 * 8 * 1024;
	PatternTable pattern_table;
	pattern_table.Initialize(renderer);
	Nametable name_table(0x2000, &pattern_table);
	name_table.Initialize(renderer);
	//  $0000-$0FFF, nicknamed "left" 0 - 4095
	//	$1000-$1FFF, nicknamed "right" 4096 - 8191

	// this loops over the whole range (0-8191), out of laziness
	// "left" and "right" will actually be displayed as "top"
	// and "bottom"
	
	return_code = SDL_SetRenderDrawColor(
		renderer,
		0x00,
		0x00,
		0x00,
		0xFF
	);

	NESConsole::GetInstance()->PowerUp();

	bool quit = false;
	bool paused = false;
	SDL_Event event;
	while (!quit)
	{
		const Uint32 frame_start = SDL_GetTicks();
		bool n_pressed = false;
		bool v_pressed = false;
		bool l_pressed = false;
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
				else if (event.key.keysym.sym == SDLK_l)
				{
					l_pressed = true;
				}
				else if (event.key.keysym.sym == SDLK_v)
				{
					v_pressed = true;
				}
				else if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					exit(1);
				}
				break;
			case SDL_QUIT:
				quit = true;
				break;
			}
		}

		//	bug if frame takes too long to render, need double buffer
		SDL_RenderClear(renderer);

		SDL_Rect pattern_rect;
		pattern_rect.x = 0;
		pattern_rect.y = 0;
		pattern_rect.h = 256;
		pattern_rect.w = 128;
		SDL_RenderCopy(
			renderer,
			pattern_table.GetTexture(),
			nullptr,
			&pattern_rect
		);
		
		for (int i = 0x3F00; i <= 0x3F1f; ++i)
		{
			static const int num_palettes = 0x3F1F - 0x3F00;
			SDL_Rect palette_rect;
			palette_rect.y = pattern_render_area_y;
			palette_rect.h = palette_height;
			palette_rect.w = pattern_render_area_x / num_palettes;
			palette_rect.x = (i - 0x3F00) * palette_rect.w;
			
			uint8_t color = memory.PPUReadByte(i);
			uint32_t palette_color = PaletteColors[color];

			const uint8_t r = (palette_color & 0xFF0000) >> 16;
			const uint8_t g = (palette_color & 0x00FF00) >> 8;
			const uint8_t b = (palette_color & 0x0000FF);
			const uint8_t a = 0xFF;
			return_code = SDL_SetRenderDrawColor(renderer, r, g, b, a);
			SDL_RenderFillRect(renderer, &palette_rect);
		}

		if (l_pressed)
		{
			const bool logging_enabled = NESConsole::GetInstance()->GetCPU().GetLoggingEnabled();
			NESConsole::GetInstance()->GetCPU().SetLoggingEnabled(!logging_enabled);
			l_pressed = false;
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
				uint8_t color = memory.PPUReadByte(i);
				std::cout << "color: 0x" << std::uppercase << std::hex << std::setw(4) << std::setfill('0')
					<< static_cast<uint16_t>(color);// << std::endl;
				std::cout << "(0x" << std::uppercase << std::hex << std::setw(4) << std::setfill('0')
					<< PaletteColors[color] << ")" << std::endl;
			}
		}
		
		NESConsole::GetInstance()->Run();
		name_table.Run();

		SDL_Rect name_rect;
		name_rect.x = pattern_render_area_x;
		name_rect.y = 0;
		name_rect.h = nes_resolution_y;
		name_rect.w = nes_resolution_x;
		SDL_RenderCopy(
			renderer,
			name_table.GetTexture(),
			nullptr,
			&name_rect
		);

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