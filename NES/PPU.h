#pragma once
#include <cstdint>

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

class PPU
{
public:
	void PowerUp();
	void Reset();
	void ResetWriteToggle() { m_write_toggle = 0; }

	uint8_t GetOAMDMA()
	{
		return m_oam_dma;
	}

	void SetOAMDMA(const uint8_t value)
	{
		m_oam_dma = value;
	}

	uint8_t GetData()
	{
		return m_ppu_data;
	}

	void SetData(const uint8_t value);
	

	uint8_t GetAddress()
	{
		return m_ppu_addr;
	}

	void SetAddress(const uint8_t value)
	{
		// https://wiki.nesdev.com/w/index.php/PPU_registers#PPUADDR
		if (m_write_toggle == 0)
		{
			m_write_toggle = 1;
			m_ppu_addr = value;
			const uint16_t high_address = m_ppu_addr << 8;
			// clear high bits
			m_ppu_full_addr &= 0x00FF;
			// set high bits
			m_ppu_full_addr |= high_address;
			
		}
		else
		{
			m_write_toggle = 0;
			m_ppu_addr = value;
			// clear low bits
			m_ppu_full_addr &= 0xFF00;
			// set low bits
			m_ppu_full_addr |= m_ppu_addr;
		}
		
	}

	uint8_t GetScroll()
	{
		return m_ppu_scroll;
	}

	void SetScroll(const uint8_t value)
	{
		// https://wiki.nesdev.com/w/index.php/PPU_registers#Scroll_.28.242005.29_.3E.3E_write_x2
		if (m_write_toggle == 0)
		{
			m_write_toggle = 1;
			m_ppu_scroll = value;
			const uint16_t high_address = m_ppu_scroll << 8;
			// clear high bits
			m_ppu_full_scroll &= 0x00FF;
			// set high bits
			m_ppu_full_scroll |= high_address;

		}
		else
		{
			m_write_toggle = 0;
			m_ppu_scroll = value;
			// clear low bits
			m_ppu_full_scroll &= 0xFF00;
			// set low bits
			m_ppu_full_scroll |= m_ppu_addr;
		}
	}

	uint8_t GetOAMData()
	{
		return m_reg_oam_data;
	}

	void SetOAMData(const uint8_t value)
	{
		m_reg_oam_data = value;
	}
	
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

	PPUMaskRegister GetMaskRegister()
	{
		return m_reg_ppu_mask;
	}

	void SetMaskRegister(const uint8_t value)
	{
		m_reg_ppu_mask.Register = value;
	}
	
	PPUControlRegister& GetControlRegister()
	{
		return m_reg_ppu_control;
	}

	void SetControlRegister(const uint8_t value)
	{
		m_reg_ppu_control.Register = value;
	}
private:
	// https://wiki.nesdev.com/w/index.php/PPU_registers
	PPUControlRegister m_reg_ppu_control;
	PPUMaskRegister m_reg_ppu_mask;
	PPUStatusRegister m_reg_status;
	uint8_t m_reg_oam_addr = 0x00;
	uint8_t m_reg_oam_data = 0x00;
	uint8_t m_ppu_scroll = 0x00;
	uint16_t m_ppu_full_scroll = 0x0000;
	uint8_t m_ppu_addr = 0x00;
	uint16_t m_ppu_full_addr = 0x0000;
	uint8_t m_ppu_data = 0x00;
	uint8_t m_oam_dma = 0x00;
	// https://wiki.nesdev.com/w/index.php/PPU_registers#PPUADDR
	// determines if writing high bit or low bit
	// to 0x2005 and 0x2006
	uint8_t m_write_toggle = 0x00;
};

