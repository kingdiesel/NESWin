#include "Mapper001.h"
#include "../BitMask.h"
#include <cassert>

Mapper001::Mapper001(const iNESHeader& header) :
	IMapper(header)
{
	constexpr int ram_bank_size = 8 * 1024;
	m_prg_ram_bank = new uint8_t[ram_bank_size];
	memset(m_prg_ram_bank, 0x00, ram_bank_size);
}

bool Mapper001::CPUReadByte(const uint16_t position, uint16_t& mapped_position, uint8_t& value)
{
	// CPU $6000-$7FFF: 8 KB PRG RAM bank, (optional)
	if (position >= 0x6000 && position <= 0x7FFF)
	{
		mapped_position = 0xFFFF;
		value = m_prg_ram_bank[position & ADDR_8K_MAX];
		return true;
	}
	if (position >= 0x8000)
	{
		if (m_control_register.Bits.m_prg_rom_bank >=2 )
		{
			// 16K Mode
			if (position >= 0x8000 && position <= 0xBFFF)
			{
				mapped_position = m_prg_bank_selector_16_low_addr * 0x4000 + (position & 0x3FFF);
				return true;
			}

			if (position >= 0xC000 && position <= 0xFFFF)
			{
				mapped_position = m_prg_bank_selector_16_high_addr * 0x4000 + (position & 0x3FFF);
				return true;
			}
		}
		else
		{
			mapped_position = m_prg_bank_select_32 * 0x8000 + (position & 0x7FFF);
			return true;
		}
	}

	return false;
}

bool Mapper001::CPUWriteByte(const uint16_t position, uint16_t& mapped_position, const uint8_t value)
{
	// CPU $6000-$7FFF: 8 KB PRG RAM bank, (optional)
	if (position >= 0x6000 && position <= 0x7FFF)
	{
		mapped_position = 0xFFFF;
		m_prg_ram_bank[position & ADDR_8K_MAX] = value;
		return true;
	}

	// https://wiki.nesdev.com/w/index.php/MMC1#Registers
	if (position >= 0x8000)
	{
		//	7  bit  0
		//	---- ----
		//	Rxxx xxxD
		//	|       |
		//	|       +-Data bit to be shifted into shift register, LSB first
		//	+ -------- - 1: Reset shift register and write Control with(Control OR $0C),
		//	locking PRG ROM at $C000 - $FFFF to the last bank.
		
		// Writing a value with bit 7 set ($80 through $FF) 
		// to any address in $8000-$FFFF clears the shift 
		// register to its initial state
		if (Bit7Set(value))
		{
			m_load_shift_register = 0x00;
			m_load_shift_register_count = 0;
			// Reset shift register and write Control with (Control OR $0C),
			m_control_register.Register |= BIT_CONTROL_RESET;
		}
		else
		{
			// To change a register's value, the CPU writes five 
			// times with bit 7 clear and a bit of the desired value in bit 0.
			// lsb is written first
			m_load_shift_register >>= 1;
			m_load_shift_register |= (value & 0x01) << 4;
			m_load_shift_register_count++;

			// On the fifth write, the MMC1 copies bit 0 and the shift register 
			// contents into an internal register selected by bits 14 and 13 of the 
			// address, and then it clears the shift register.
			if (m_load_shift_register_count == 5)
			{
				uint16_t target_register = position >> 13;
				target_register &= BIT_0_1;

				// Control (internal, $8000-$9FFF)
				if (target_register == 0)
				{
					m_control_register.Register = m_load_shift_register & BIT_0_1_2_3_4;
				}
				// CHR bank 0 (internal, $A000-$BFFF)
				else if (target_register == 1)
				{
					m_chr_bank_0_register.Register = m_load_shift_register & BIT_0_1_2_3_4;
					if (m_control_register.Bits.m_chr_rom_bank == 1)
					{
						m_char_bank_selector_low_addr = m_chr_bank_0_register.Register;
					}
					else
					{
						m_char_bank_selector_8 = m_chr_bank_0_register.Register & 0x1E;
					}
				}
				// CHR bank 1 (internal, $C000-$DFFF)
				else if (target_register == 2)
				{
					m_chr_bank_1_register.Register = m_load_shift_register & BIT_0_1_2_3_4;
					if (m_control_register.Bits.m_chr_rom_bank == 1)
					{
						m_char_bank_selector_high_addr = m_chr_bank_1_register.Register;
					}
				}
				// PRG bank (internal, $E000-$FFFF)
				else if (target_register == 3)
				{
					m_prg_bank_register.Register = m_load_shift_register & BIT_0_1_2_3_4;

					switch (m_control_register.Bits.m_prg_rom_bank)
					{
					case 0:
					case 1:
						m_prg_bank_select_32 = m_prg_bank_register.Bits.m_prg_rom_bank_select >> 1;
						break;
					case 2:
						m_prg_bank_selector_16_low_addr = 0;
						m_prg_bank_selector_16_high_addr = m_prg_bank_register.Bits.m_prg_rom_bank_select;
						break;
					case 3:
						m_prg_bank_selector_16_low_addr = m_prg_bank_register.Bits.m_prg_rom_bank_select;
						m_prg_bank_selector_16_high_addr = m_header_data.prg_rom_size_16 - 1;
						break;
					}
				}

				// reset
				m_load_shift_register = 0x00;
				m_load_shift_register_count = 0;
			}
		}
	}
	return false;
}

MirrorMode Mapper001::GetMirrorMode()
{
	// (0: one-screen, lower bank; 1: one-screen, upper bank;2: vertical; 3: horizontal)
	if (m_control_register.Bits.m_mirroring == 0)
	{
		return MirrorMode::OneScreenLower;
	}
	else if (m_control_register.Bits.m_mirroring == 1)
	{
		return MirrorMode::OneScreenUpper;
	}
	else if (m_control_register.Bits.m_mirroring == 2)
	{
		return MirrorMode::Vertical;
	}
	return MirrorMode::Horizontal;
}

bool Mapper001::PPUReadByte(const uint16_t position, uint16_t& mapped_position, uint8_t& value)
{
	if (position >= 0x0000 && position <= 0x1FFF)
	{
		if (m_header_data.chr_rom_size_8 == 0)
		{
			mapped_position = position;
			return true;
		}
		else
		{
			if (m_control_register.Bits.m_chr_rom_bank == 1)
			{
				if (position >= 0x0000 && position <= 0x0FFF)
				{
					mapped_position = m_char_bank_selector_low_addr * 0x1000 + (position & 0x0FFF);
					return true;
				}

				if (position >= 0x1000 && position <= 0x1FFF)
				{
					mapped_position = m_char_bank_selector_high_addr * 0x1000 + (position & 0x0FFF);
					return true;
				}
			}
			else
			{
				mapped_position = m_char_bank_selector_8 * 0x2000 + (position & 0x1FFF);
				return true;
			}
		}
	}
	return false;
}

bool Mapper001::PPUWriteByte(const uint16_t position, uint16_t& mapped_position, const uint8_t value)
{
	// PPU $0000-$0FFF: 4 KB switchable CHR bank
	// PPU $1000 - $1FFF: 4 KB switchable CHR bank
	if (position >= 0x0000 && position <= 0x1FFF)
	{
		if (m_header_data.chr_rom_size_8 == 0)
		{
			mapped_position = position;
			return true;
		}
	}
	return false;
}