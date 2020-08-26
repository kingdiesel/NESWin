#ifndef NES_MEMORY_H
#define NES_MEMORY_H

#include "ROM.h"

class Memory
{
public:
	Memory();
	
	uint8_t CPUReadByte(const uint16_t position) const;
	void CPUWriteByte(const uint16_t position, uint8_t value);
	
	uint8_t PPUReadByte(const uint16_t position) const;
	void PPUWriteByte(const uint16_t position, uint8_t value);
	
	// helper function when we read two bytes in a row
	// to construct a memory address
	uint16_t CPUReadShort(const uint16_t position) const;
	
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
	uint8_t* m_ppu_ram_buffer = nullptr;
	uint8_t m_palette_buffer[32];
	uint8_t m_object_attribute_memory[256];
};


#endif //NES_MEMORY_H
