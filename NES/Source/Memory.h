#ifndef NES_MEMORY_H
#define NES_MEMORY_H

#include "ROM.h"

class Memory
{
public:
	Memory();
	uint8_t CPUReadByte(const uint16_t position) const;

	uint16_t CPUReadShort(const uint16_t position) const;

	void CPUWriteByte(const uint16_t position, uint8_t value);

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
	uint8_t* m_cpu_ram_buffer = nullptr;
};


#endif //NES_MEMORY_H
