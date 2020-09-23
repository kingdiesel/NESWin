#pragma once
#include <cstdint>
#include <cassert>
#include "Nametable.h"

union PPUControlRegister
{
	struct Bits {
		uint8_t m_nametable_x : 1;
		uint8_t m_nametable_y : 1;
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

// https://wiki.nesdev.com/w/index.php/PPU_scrolling#PPU_internal_registers
union PPUScrollRegister
{
	struct
	{
		uint16_t m_coarse_x : 5;
		uint16_t m_coarse_y : 5;
		uint16_t m_nametable_x : 1;
		uint16_t m_nametable_y : 1;
		uint16_t m_fine_y : 3;
		uint16_t m_unused : 1;
	} Bits;
	uint16_t Register = 0x0000;
};

class PPU
{
public:
	PPU();
	void PowerUp();
	void Reset();
	void SetPatternTable(class PatternTable* pattern_table) { m_pattern_table = pattern_table; }
	void ResetWriteToggle() { m_write_toggle = 0; }
	const uint32_t* GetFrameBufferData() const { return m_frame_buffer_data; }
	void Run();

	void SetOAMDMA(const uint8_t value);

	void ResetNMIRequest() { SetNMIRequest(false); }
	void SetNMIRequest(const bool cpu_nmi) { m_cpu_nmi = cpu_nmi; }
	bool GetNMIRequest() { return m_cpu_nmi; }
	bool IsRenderingEnabled() const;

	uint8_t GetData()
	{
		return m_ppu_data;
	}

	void SetData(const uint8_t data)
	{
		m_ppu_data = data;
	}

	uint8_t GetWriteToggle() const { return m_write_toggle; }
	void SetWriteToggle(const uint8_t toggle) 
	{ 
		assert(toggle == 0 || toggle == 1); 
		m_write_toggle = toggle; 
	}

	void SetFineX(const uint8_t fine_x) { m_fine_x = fine_x; }

	
	bool GetFrameReady() const { return m_frame_ready; }
	void ResetFrameReady();

	uint8_t GetOAMData();
	void SetOAMData(const uint8_t value);
	
	uint8_t GetOAMAddress()
	{
		return m_reg_oam_addr;
	}

	void SetOAMAddress(const uint8_t value)
	{
		m_reg_oam_addr = value;
	}

	PPUStatusRegister GetStatusRegister()
	{
		return m_reg_status;
	}

	void SetStatusRegister(const uint8_t value)
	{
		m_reg_status.Register = value;
	}

	PPUMaskRegister GetMaskRegister() const
	{
		return m_reg_ppu_mask;
	}

	void SetMaskRegister(const uint8_t value)
	{
		m_reg_ppu_mask.Register = value;
	}
	
	const PPUControlRegister& GetControlRegister() const
	{
		return m_reg_ppu_control;
	}

	void SetControlRegister(const uint8_t value)
	{
		m_reg_ppu_control.Register = value;
	}

	PPUScrollRegister& GetTempVram() { return m_temp_vram; }
	PPUScrollRegister& GetCurrentVram() { return m_current_vram; }
	void MoveTempVramToCurrent() { m_current_vram = m_temp_vram; }
private:
	void SetFrameReady(const bool frame_ready) { m_frame_ready = frame_ready; }
	
	uint8_t GetPaletteIndexFromAttributeByte(
		const int pixel_x, 
		const int pixel_y, 
		const uint8_t attribute_byte
	);
	
	uint8_t GetAttributeByte(
		const int pixel_x, 
		const int pixel_y, 
		const uint16_t nametable_addr
	);

	Nametable::Quadrant GetQuadrantFromAttribyteByte(
		const int pixel_x,
		const int pixel_y,
		const uint8_t attribute_byte
	);

	// https://wiki.nesdev.com/w/index.php/PPU_registers
	PPUControlRegister m_reg_ppu_control;
	PPUMaskRegister m_reg_ppu_mask;
	PPUStatusRegister m_reg_status;
	uint8_t m_reg_oam_addr = 0x00;
	uint8_t m_ppu_data = 0x00;
	bool m_cpu_nmi = false;
	// https://wiki.nesdev.com/w/index.php/PPU_frame_timing#Even.2FOdd_Frames
	bool m_even_frame = true;
	// cycles spent on this scanline
	int cycles = 0;
	// scanlines rendered this frame
	int scanlines = 0;

	// https://wiki.nesdev.com/w/index.php/PPU_scrolling#PPU_internal_registers
	PPUScrollRegister m_temp_vram;
	PPUScrollRegister m_current_vram;
	uint8_t m_fine_x = 0x00;
	uint8_t m_write_toggle = 0x00;

	// more internal registers
	// https://wiki.nesdev.com/w/index.php/PPU_rendering#Preface
	// 2 16-bit shift registers - These contain the pattern table data 
	// for two tiles. Every 8 cycles, the data for the next tile is 
	// loaded into the upper 8 bits of this shift register. 
	// Meanwhile, the pixel to render is fetched from one of the lower 8 bits.
#if 0
	uint16_t m_shift_pattern_bkgd[2] = { 0 };
	uint8_t m_next_tile_high = 0;
	uint8_t m_next_tile_low = 0;
	uint8_t m_next_tile_id = 0;

	// 2 8 - bit shift registers - These contain the palette attributes 
	// for the lower 8 pixels of the 16 - bit shift register.These 
	// registers are fed by a latch which contains the palette attribute 
	// for the next tile.Every 8 cycles, the latch is loaded with the 
	// palette attribute for the next tile.
	uint16_t m_shift_attribute[2] = { 0 };
	uint8_t m_next_tile_attrubte = 0;
#endif
	uint8_t m_bg_next_tile_id = 0x00;
	uint8_t m_bg_next_tile_attrib = 0x00;
	uint8_t m_bg_next_tile_lsb = 0x00;
	uint8_t m_bg_next_tile_msb = 0x00;
	uint16_t m_bg_shifter_pattern_lo = 0x0000;
	uint16_t m_bg_shifter_pattern_hi = 0x0000;
	uint16_t m_bg_shifter_attrib_lo = 0x0000;
	uint16_t m_bg_shifter_attrib_hi = 0x0000;
	// 8 pairs of 8-bit shift registers - These contain the pattern table 
	// data for up to 8 sprites, to be rendered on the current scanline. 
	// Unused sprites are loaded with an all-transparent set of values.
	uint8_t m_shifter_pattern_sprite_low[8];
	uint8_t m_shifter_pattern_sprite_high[8];

	bool m_possible_sprite_zero_hit = false;
	bool m_sprite_zero_rendered = false;

	uint8_t m_active_sprites = 0;


	// ppu has "filled the frame buffer"
	bool m_frame_ready = false;

	uint32_t* m_frame_buffer_data = nullptr;

	class PatternTable* m_pattern_table = nullptr;
};

