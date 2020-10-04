#pragma once
#include "IMapper.h"

// https://wiki.nesdev.com/w/index.php/NROM
class Mapper001 : public IMapper
{
public:
	Mapper001(const iNESHeader& header) :
		IMapper(header)
	{}
	virtual bool CPUReadByte(const uint16_t position, uint16_t& mapped_position) override;
	virtual bool CPUWriteByte(const uint16_t position, uint16_t& mapped_position, const uint8_t value) override;

	virtual bool PPUReadByte(const uint16_t position, uint16_t& mapped_position) override;
	virtual bool PPUWriteByte(const uint16_t position, uint16_t& mapped_position, const uint8_t value) override;
private:
	// https://wiki.nesdev.com/w/index.php/MMC1#Registers
	uint8_t m_shift_register = 0x00;
	int m_shift_register_write_count = 0;
};