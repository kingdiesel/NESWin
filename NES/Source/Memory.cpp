#include <assert.h>
#include <iostream>
#include <iomanip>
#include "Memory.h"

uint8_t Memory::GetByte(const uint16_t position) const
{
	if (position <= 0x07FF)
	{
		return m_ram.GetByte(position);
	}
	else if (position <= 0xFFFF && position >= 0x4020)
	{
		return m_rom.GetByte(position);
	}
	else
	{
		std::cout << "Unkonown memory location: 0x" << std::uppercase << std::hex << std::setw(4) << std::setfill('0')
				  << position << std::endl;
		exit(1);
	}
	assert(false);
}

uint16_t Memory::GetShort(const uint16_t position) const
{
	if (position <= 0x07FF)
	{
		return m_ram.GetByte(position);
	}
	else if (position <= 0xFFFF && position >= 0x4020)
	{
		return m_rom.GetShort(position);
	}
	assert(false);
	return 0;
}

void Memory::SetByte(const uint16_t position, uint8_t value)
{
	assert(!(position <= 0xFFFF && position >= 0x4020));
	m_ram.SetByte(position, value);
}

void Memory::Reset()
{
	m_ram.Reset();
	m_rom.Reset();
}