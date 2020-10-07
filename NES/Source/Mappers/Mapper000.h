#pragma once
#include "IMapper.h"

// https://wiki.nesdev.com/w/index.php/NROM
class Mapper000 : public IMapper
{
public:
	Mapper000(const iNESHeader& header) :
		IMapper(header)
	{}
	virtual bool CPUReadByte(const uint16_t position, uint16_t& mapped_position, uint8_t& value) override;
	virtual bool CPUWriteByte(const uint16_t position, uint16_t& mapped_position, const uint8_t value) override;

	virtual bool PPUReadByte(const uint16_t position, uint16_t& mapped_position, uint8_t& value) override;
	virtual bool PPUWriteByte(const uint16_t position, uint16_t& mapped_position, const uint8_t value) override;
private:

};