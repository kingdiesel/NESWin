#include <cstring>
#include <assert.h>
#include "RAM.h"

RAM::RAM()
{
	m_ram_buffer = new unsigned char[2 * 1024];
	memset(m_ram_buffer, 0x00, 2 * 1024);
}

void RAM::Reset()
{
	memset(m_ram_buffer, 0x00, 2 * 1024);
}

unsigned char RAM::GetByte(const unsigned short position) const
{
	assert(position >= 0 && position <= 0x07FF);
	return m_ram_buffer[position];
}

unsigned short RAM::GetShort(const unsigned short position) const
{
	assert(position >= 0 && position <= 0x07FF);
	unsigned short return_value = 0x0000;
	return_value = m_ram_buffer[position + 1];
	return_value = return_value << 8;
	return_value |= m_ram_buffer[position];
	return return_value;
}

void RAM::SetByte(const unsigned short position, unsigned char value)
{
	assert(position >= 0 && position <= 0x07FF);
	m_ram_buffer[position] = value;
}