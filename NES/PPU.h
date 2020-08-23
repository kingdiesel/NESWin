#pragma once
#include <cstdint>
class PPU
{
	union PPUControlRegister
	{
		struct Bits {
			uint8_t m_base_nametable_addr : 2;
			uint8_t m_vram_add : 1;
			uint8_t m_sprite_pattern_addr : 1;
			uint8_t m_bkgrnd_pattern_addr : 1;
			uint8_t m_sprite_size : 1;
			uint8_t m_ppu_master_slave : 1;
			uint8_t m_generate_nmi : 1;

		} Bits;
		uint8_t Register = 0x00;
	};

	union PPUMaskRegister
	{
		struct Bits {
			uint8_t m_grayscale : 1;
			uint8_t m_show_bkgrnd_leftmost : 1;
			uint8_t m_show_sprites_leftmost : 1;
			uint8_t m_show_bkgrnd : 1;
			uint8_t m_show_sprites : 1;
			uint8_t m_emphasize_red : 1;
			uint8_t m_emphasize_green : 1;
			uint8_t m_emphasize_blue : 1;

		} Bits;
		uint8_t Register = 0x00;
	};

	union PPUStatusRegister
	{
		struct Bits {
			uint8_t m_lsb_previous_write : 5;
			uint8_t m_sprite_overflow : 1;
			uint8_t m_sprite_zero_hit : 1;
			uint8_t m_vertical_blank_started : 1;

		} Bits;
		uint8_t Register = 0x00;
	};

	PPUControlRegister m_reg_ppu_control;
	PPUMaskRegister m_reg_ppu_mask;
	PPUStatusRegister m_reg_status;
	uint8_t m_reg_oam_addr = 0x00;
	uint8_t m_reg_oam_data = 0x00;
	uint8_t m_ppu_scroll = 0x00;
	uint8_t m_ppu_addr = 0x00;
	uint8_t m_ppu_data = 0x00;
	uint8_t m_oam_dma = 0x00;
};

