#include "Mapper000.h"
#include <cassert>
#pragma warning( disable : 4100 )

bool Mapper000::CPUReadByte(const uint16_t position, uint16_t& mapped_position, uint8_t& value)
{
	//	CPU $6000 - $7FFF: 
	//		Family Basic only : PRG RAM, mirrored as necessary to fill entire 8 KiB window, 
	//		write protectable with an external switch
	//CPU $8000 - $BFFF : 
	//		First 16 KB of ROM.
	//CPU $C000 - $FFFF : 
	//		Last 16 KB of ROM(NROM - 256) or mirror of $8000 - $BFFF(NROM - 128).
	if (position >= 0x8000 && position <= 0xFFFF)
	{
		mapped_position = position & (m_header_data.prg_rom_size_16 > 1 ? 0x7FFF : 0x3FFF);
		return true;
	}
	return false;
}

bool Mapper000::CPUWriteByte(const uint16_t position, uint16_t& mapped_position, const uint8_t value)
{
	uint8_t dummy = 0x00;
	return CPUReadByte(position, mapped_position, dummy);
}

bool Mapper000::PPUReadByte(const uint16_t position, uint16_t& mapped_position, uint8_t& value)
{
	mapped_position = position;
	return true;
}

bool Mapper000::PPUWriteByte(const uint16_t position, uint16_t& mapped_position, const uint8_t value)
{
	return false;
}

MirrorMode Mapper000::GetMirrorMode()
{
	if (m_header_data.m_flags_6.Bits.m_mirroring == 0)
	{
		return MirrorMode::Horizontal;
	}
	return MirrorMode::Vertical;
}