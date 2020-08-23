#include <assert.h>
#include <iostream>
#include <iomanip>
#include "Memory.h"

uint8_t CPUMemory::ReadByte(const uint16_t position) const
{
	/*
		$0000-$07FF	$0800	2KB internal RAM (0-2047)
		$0800-$0FFF	$0800	Mirrors of $0000-$07FF (2048-4095)
		$1000-$17FF	$0800
		$1800-$1FFF	$0800
		$2000-$2007	$0008	NES PPU registers
		$2008-$3FFF	$1FF8	Mirrors of $2000-2007 (repeats every 8 bytes)
		$4000-$4017	$0018	NES APU and I/O registers
		$4018-$401F	$0008	APU and I/O functionality that is normally disabled. See CPU Test Mode.
		$4020-$FFFF	$BFE0	Cartridge space: PRG ROM, PRG RAM, and mapper registers (See Note)
	*/
	if (position <= 0x1FFF)
	{
		return m_ram.GetByte(position & 0x07FF);
	}
	else if (position >= 0x2000 && position <= 0x3FFF)
	{
		// TODO: read from PPU
		assert(false);
	}
	else if (position >= 0x4000 && position <= 0x4017)
	{
		// TODO: read from APU 
		assert(false);
	}
	else if (position >= 0x4018 && position <= 0x401F)
	{
		// TODO: read from mystery APU
		assert(false);
	}
	else if (position >= 0x4020 && position <= 0xFFFF)
	{
		return m_rom.GetByte(position);
	}
	std::cout << "Unknown memory location: 0x" << std::uppercase << std::hex << std::setw(4) << std::setfill('0')
				<< position << std::endl;
	exit(1);
	return 0;
}

uint16_t CPUMemory::ReadShort(const uint16_t position) const
{
	uint16_t return_value = 0;
	const uint8_t low = ReadByte(position);
	const uint8_t high = ReadByte(position + 1);
	return_value = high << 8;
	return_value |= low;
	return return_value;
}

void CPUMemory::WriteByte(const uint16_t position, uint8_t value)
{
	assert(!(position <= 0xFFFF && position >= 0x4020));
	m_ram.SetByte(position, value);
}

void CPUMemory::Reset()
{
	m_ram.Reset();
	m_rom.Reset();
}