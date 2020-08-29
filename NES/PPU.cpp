#include "PPU.h"
#include "Source/NESConsole.h"

void PPU::PowerUp()
{
	// https://wiki.nesdev.com/w/index.php/PPU_power_up_state
}

void PPU::Reset()
{
	// https://wiki.nesdev.com/w/index.php/PPU_power_up_state
}

void PPU::SetData(const uint8_t value)
{
	// https://wiki.nesdev.com/w/index.php/PPU_registers#PPUDATA
	m_ppu_data = value;
	NESConsole::GetInstance()->GetMemory().PPUWriteByte(m_ppu_full_addr, value);
	if (GetControlRegister().Bits.m_vram_add == 0)
	{
		m_ppu_full_addr += 1;
	}
	else
	{
		m_ppu_full_addr += 32;
	}
}
