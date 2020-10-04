#include "Mapper001.h"
#include "../BitMask.h"
#include <cassert>

bool Mapper001::CPUReadByte(const uint16_t position, uint16_t& mapped_position)
{
	// https://wiki.nesdev.com/w/index.php/MMC1
	//	CPU $6000 - $7FFF: 
	//		8 KB PRG RAM bank, (optional)
	//	CPU $8000 - $BFFF : 
	//		16 KB PRG ROM bank, either switchable or fixed to the first bank
	//	CPU $C000 - $FFFF : 
	//		16 KB PRG ROM bank, either fixed to the last bank or switchable
	//	PPU $0000 - $0FFF : 
	//		4 KB switchable CHR bank
	//	PPU $1000 - $1FFF : 
	//		4 KB switchable CHR bank
	if (position >= 0x8000 && position <= 0xFFFF)
	{
		mapped_position = position & (m_header_data.prg_rom_size_16 > 1 ? 0x7FFF : 0x3FFF);
		return true;
	}
	return false;
}

bool Mapper001::CPUWriteByte(const uint16_t position, uint16_t& mapped_position, const uint8_t value)
{
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
			m_shift_register = 0x00;
			m_shift_register_write_count = 0;
			// TOOD: control gets reset here
		}
		else
		{
			// To change a register's value, the CPU writes five 
			// times with bit 7 clear and a bit of the desired value in bit 0.
			// lsb is written first 
			m_shift_register >>= 1;
			m_shift_register |= (value & 0x01) << 4;
			m_shift_register_write_count++;

			// On the fifth write, the MMC1 copies bit 0 and the shift register 
			// contents into an internal register selected by bits 14 and 13 of the 
			// address, and then it clears the shift register.
			if (m_shift_register_write_count == 5)
			{
				uint16_t target_register = position >> 13;
				target_register &= BIT0_1;
				// reset
				m_shift_register = 0x00;
				m_shift_register_write_count = 0;
			}
		}
	}
	return CPUReadByte(position, mapped_position);
}

bool Mapper001::PPUReadByte(const uint16_t position, uint16_t& mapped_position)
{
	return false;
}

bool Mapper001::PPUWriteByte(const uint16_t position, uint16_t& mapped_position, const uint8_t value)
{
	return false;
}