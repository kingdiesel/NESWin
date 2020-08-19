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
	assert(position >= 0 && position <= 0x07FF);
	m_ram_buffer[position] = value;
}