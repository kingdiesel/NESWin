#ifndef NES_MEMORY_H
#define NES_MEMORY_H

#include "ROM.h"
#include "RAM.h"

class Memory
{
public:
	unsigned char GetByte(const unsigned short position) const;

	unsigned short GetShort(const unsigned short position) const;

	void SetByte(const unsigned short position, unsigned char value);

	ROM &GetROM()
	{
		return m_rom;
	}

	const ROM &GetROM() const
	{
		return m_rom;
	}

	void Reset();

private:
	ROM m_rom;
	RAM m_ram;
};


#endif //NES_MEMORY_H
