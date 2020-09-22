#ifndef NES_MEMORY_H
#define NES_MEMORY_H

#include "ROM.h"

class Memory
{
public:
	Memory();
	
	uint8_t CPUReadByte(const uint16_t position) const;
	void CPUWriteByte(const uint16_t position, uint8_t value);
	
	uint8_t PPUReadByte(uint16_t position) const;
	const uint8_t* PPUGetRawPtr(const uint16_t position) const;
	void PPUWriteByte(uint16_t position, uint8_t value);

	void PPUWriteOAM(const uint8_t index, const uint8_t value);
	uint8_t PPUReadOAM(const uint8_t index);
	
	// helper function when we read two bytes in a row
	// to construct a memory address
	uint16_t CPUReadShort(const uint16_t position) const;
	uint8_t* GetPrimaryOAM() { return m_primary_oam; }
	uint8_t* GetSeconaryOAM() { return m_secondary_oam; }
	void ClearSecondaryOAM();
	
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
	//	https://wiki.nesdev.com/w/index.php/PPU_OAM
	uint8_t m_primary_oam[256];
	uint8_t m_secondary_oam[64];

	mutable uint8_t m_controller_poll = 0;
};


#endif //NES_MEMORY_H
