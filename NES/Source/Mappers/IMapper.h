#pragma once
#include <cstdint>
#include "../ROM.h"

enum class MirrorMode
{
	Horizontal,
	Vertical,
	OneScreenLower,
	OneScreenUpper
};

class IMapper
{
public:
	IMapper(const iNESHeader& header) :
		m_header_data(header)
	{}
	virtual bool CPUReadByte(const uint16_t position, uint16_t& mapped_position, uint8_t& value) = 0;
	virtual bool CPUWriteByte(const uint16_t position, uint16_t& mapped_position, const uint8_t value) = 0;

	virtual bool PPUReadByte(const uint16_t position, uint16_t& mapped_position, uint8_t& value) = 0;
	virtual bool PPUWriteByte(const uint16_t position, uint16_t& mapped_position, const uint8_t value) = 0;

	virtual MirrorMode GetMirrorMode() = 0;
protected:
	iNESHeader m_header_data;
};