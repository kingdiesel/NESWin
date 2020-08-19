#ifndef NES_MEMORY_H
#define NES_MEMORY_H

#include "ROM.h"
#include "RAM.h"

class Memory
{
public:
	uint8_t GetByte(const uint16_t position) const;

	uint16_t GetShort(const uint16_t position) const;

	void SetByte(const uint16_t position, uint8_t value);

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
