#include <cstring>
#include <assert.h>
#include "RAM.h"

RAM::RAM()
{
	m_ram_buffer = new uint8_t[2 * 1024];
	Reset();
}

void RAM::Reset()
{
	memset(m_ram_buffer, 0x00, 2 * 1024);
}

uint8_t RAM::GetByte(const uint16_t position) const
{
	assert(position >= 0 && position <= 0x07FF);
	return m_ram_buffer[position];
}

uint16_t RAM::GetShort(const uint16_t position) const
{
	assert(position >= 0 && position <= 0x07FF);
	uint16_t return_value = 0x0000;
	return_value = m_ram_buffer[position + 1];
	return_value = return_value << 8;
	return_value |= m_ram_buffer[position];
	return return_value;
}

void RAM::SetByte(const uint16_t position, uint8_t value)
{
	/*
		$0000-$07FF	$0800	2KB internal RAM
		$0800-$0FFF	$0800	Mirrors of $0000-$07FF
		$1000-$17FF	$0800
		$1800-$1FFF	$0800
		$2000-$2007	$0008	NES PPU registers
		$2008-$3FFF	$1FF8	Mirrors of $2000-2007 (repeats every 8 bytes)
		$4000-$4017	$0018	NES APU and I/O registers
		$4018-$401F	$0008	APU and I/O functionality that is normally disabled. See CPU Test Mode.
		$4020-$FFFF	$BFE0	Cartridge space: PRG ROM, PRG RAM, and mapper registers (See Note)
	*/
	assert(position >= 0 && position <= 0x07FF);
	m_ram_buffer[position] = value;
}