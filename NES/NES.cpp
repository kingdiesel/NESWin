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
#include "Sprites.h"

int main(int argv, char** args)
{	
	//NESConsole::GetInstance()->RunNesTestTiming();
	//NESConsole::GetInstance()->RunNesTest();
	//return 0;

	//NESConsole console;
	NESConsole::GetInstance()->Initialize();
	//NESConsole::GetInstance()->GetCPU().SetLoggingEnabled(true);
	NESConsole::GetInstance()->LoadROM(
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/cpu.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/nesstress.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/nestest.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/power_up_palette.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/sprite_ram.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/palette_ram.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/cpu_timing_test.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/cpu_interrupts.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/branch_basics.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/backward_branch.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/forward_branch.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/cpu_dummy_reads.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/instr_misc.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/color_test.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/zelda.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/dk.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/shadowgate.nes"
		"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/smb.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/arkanoid.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/skydestroyer.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/1942.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/iceclimber.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/excitebike.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/defender.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/loderunner.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/bomberman.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/excitebike.nes"
		//"C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/balloonfight.nes"
	);

#define debug true
	const int screen_scale = debug ? 1 : 2;
	const int nes_resolution_x = 256;
	const int nes_resolution_y = 240;
	const int pattern_render_area_x = 128;
	const int pattern_render_area_y = 256;
	const int palette_height = 16;
	const int window_x = debug ? pattern_render_area_x + nes_resolution_x + nes_resolution_x + nes_resolution_x : nes_resolution_x;
	const int window_y = debug ? pattern_render_area_y + nes_resolution_y : nes_resolution_y;
	const int fps = 60;
	const int sdl_wait = static_cast<int>(1000.0f / (float)fps);
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* frame_buffer = nullptr;

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

	frame_buffer = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC,
		256,
		240
	);

	Uint32 format = SDL_PIXELFORMAT_RGB888;
	SDL_PixelFormat* mapping_format = SDL_AllocFormat(format);
	const ROM& rom = NESConsole::GetInstance()->GetROM();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	PPU& ppu = NESConsole::GetInstance()->GetPPU();
	// Pattern tables
	//const int num_tiles = rom.GetHeaderData().chr_rom_size_8 * 8 * 1024;
#if debug
	PatternTable pattern_table;
	pattern_table.Initialize(renderer);
	
	Sprites sprites(&pattern_table);
	sprites.Initialize(renderer);
	
	const int NUM_NAMETABLES = 4;
	Nametable** nametables = new Nametable*[NUM_NAMETABLES];
	for (int i = 0; i < NUM_NAMETABLES; ++i)
	{
		nametables[i] = new Nametable(0x2000 + (1024 * i), &pattern_table);
		nametables[i]->Initialize(renderer);
	}
#endif
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
	int show_lines = 0;
	int show_nametable = 0;
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
				else if (event.key.keysym.sym == SDLK_DOWN)
				{
					NESConsole::GetInstance()->SetDownPressed(true);
				}
				else if (event.key.keysym.sym == SDLK_UP)
				{
					NESConsole::GetInstance()->SetUpPressed(true);
				}
				else if (event.key.keysym.sym == SDLK_LEFT)
				{
					NESConsole::GetInstance()->SetLeftPressed(true);
				}
				else if (event.key.keysym.sym == SDLK_RIGHT)
				{
					NESConsole::GetInstance()->SetRightPressed(true);
				}
				else if (event.key.keysym.sym == SDLK_RETURN)
				{
					NESConsole::GetInstance()->SetStartPressed(true);
				}
				else if (event.key.keysym.sym == SDLK_z)
				{
					NESConsole::GetInstance()->SetAPressed(true);
				}
				else if (event.key.keysym.sym == SDLK_x)
				{
					NESConsole::GetInstance()->SetBPressed(true);
				}
				else if (event.key.keysym.sym == SDLK_SPACE)
				{
					NESConsole::GetInstance()->SetSelectPressed(true);
				}
				else if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					NESConsole::GetInstance()->FlushLog();
					exit(1);
				}
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_DOWN)
				{
					NESConsole::GetInstance()->SetDownPressed(false);
				}
				else if (event.key.keysym.sym == SDLK_UP)
				{
					NESConsole::GetInstance()->SetUpPressed(false);
				}
				else if (event.key.keysym.sym == SDLK_LEFT)
				{
					NESConsole::GetInstance()->SetLeftPressed(false);
				}
				else if (event.key.keysym.sym == SDLK_RIGHT)
				{
					NESConsole::GetInstance()->SetRightPressed(false);
				}
				else if (event.key.keysym.sym == SDLK_RETURN)
				{
					NESConsole::GetInstance()->SetStartPressed(false);
				}
				else if (event.key.keysym.sym == SDLK_z)
				{
					NESConsole::GetInstance()->SetAPressed(false);
				}
				else if (event.key.keysym.sym == SDLK_x)
				{
					NESConsole::GetInstance()->SetBPressed(false);
				}
				else if (event.key.keysym.sym == SDLK_SPACE)
				{
					NESConsole::GetInstance()->SetSelectPressed(false);
				}
				break;
			case SDL_QUIT:
				quit = true;
				break;
			}
		}

		//	bug if frame takes too long to render, need double buffer
		SDL_RenderClear(renderer);
#if debug
		{
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

			for (int i = 0x3F00; i <= 0x3F1F; ++i)
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
		}
#endif

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
			show_lines++;
		}
		
		NESConsole::GetInstance()->Run();
		const uint32_t* frame_buffer_data = NESConsole::GetInstance()->GetPPU().GetFrameBufferData();
		SDL_UpdateTexture(
			frame_buffer,
			nullptr,
			frame_buffer_data,
			256 * sizeof(Uint32)
		);
#if debug
		{
			for (int i = 0; i < NUM_NAMETABLES; ++i)
			{
				SDL_Rect name_rect;
				nametables[i]->Run();
				name_rect.x = pattern_render_area_x + (nes_resolution_x * (i % 2));
				name_rect.y = nes_resolution_y * (i / 2);
				name_rect.h = nes_resolution_y;
				name_rect.w = nes_resolution_x;
				SDL_RenderCopy(
					renderer,
					nametables[i]->GetTexture(),
					nullptr,
					&name_rect
				);
			}

			// presence of chr ram indicates taht this memory is filled 
			// and managed at run time, so it too needs an update call
			// every frame
			if (NESConsole::GetInstance()->GetROM().HasChrRam())
			{
				pattern_table.Run();
			}

			sprites.Run();
			SDL_Rect sprites_rect;
			sprites_rect.x = pattern_render_area_x + (nes_resolution_x * 2);
			sprites_rect.y = 0;
			sprites_rect.h = 240;
			sprites_rect.w = 256;
			SDL_RenderCopy(
				renderer,
				sprites.GetTexture(),
				nullptr,
				&sprites_rect
			);
		}
#endif
		SDL_Rect frame_buffer_rect;
		frame_buffer_rect.x = debug ? pattern_render_area_x + (nes_resolution_x * 2) :0;
		frame_buffer_rect.y = debug ? 240 : 0;
		frame_buffer_rect.h = debug ? 240 : window_y * screen_scale;
		frame_buffer_rect.w = debug ? 256 : window_x * screen_scale;
		SDL_RenderCopy(
			renderer,
			frame_buffer,
			nullptr,
			&frame_buffer_rect
		);
		

		bool draw_lines = false;
		int max_x = 8;
		int max_y = 8;
		if (show_lines % 3 == 1)
		{
			draw_lines = true;
		}
		else if (show_lines % 3 == 2)
		{
			draw_lines = true;
			max_x = 32;
			max_y = 30;
		}
		else
		{
			draw_lines = false;
		}

		if (debug && draw_lines)
		{
			int grid_width = 256 / max_x;
			int grid_height = 256 / max_y;
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			for (int i = 0; i < max_x; ++i)
			{
				SDL_RenderDrawLine(
					renderer,
					(nes_resolution_x * 2) + pattern_render_area_x + (i * grid_width),
					nes_resolution_y,
					(nes_resolution_x * 2) + pattern_render_area_x + (i * grid_width),
					nes_resolution_y * 2
				);
			}

			for (int i = 0; i < max_y; ++i)
			{
				SDL_RenderDrawLine(
					renderer,
					(nes_resolution_x * 2) + pattern_render_area_x,
					nes_resolution_y + i * grid_height,
					(nes_resolution_x * 2) + pattern_render_area_x + 256,
					nes_resolution_y + i * grid_height
				);
			}
		}
		
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
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