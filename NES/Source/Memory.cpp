#include <assert.h>
#include <iostream>
#include <iomanip>
#include "Memory.h"
#include "NESConsole.h"

Memory::Memory()
{
	/*
		$0000-$07FF	$0800	2KB internal RAM (0-2047)
	*/
	m_cpu_ram_buffer = new uint8_t[2 * 1024];
	memset(m_cpu_ram_buffer, 0x00, 2 * 1024);
}

uint8_t Memory::CPUReadByte(const uint16_t position) const
{
	/*
		$0000-$07FF	$0800	2KB internal RAM (0-2047)
		$0800-$0FFF	$0800	Mirrors of $0000-$07FF (2048-4095)
		$1000-$17FF	$0800
		$1800-$1FFF	$0800
		$2000-$2007	$0008	NES PPU registers
		$2008-$3FFF	$1FF8	Mirrors of $2000-2007 (repeats every 8 bytes)
		$4000-$4017	$0018	NES APU and I/O registers
		$4018-$401F	$0008	APU and I/O functionality that is normally disabled. See CPU Test Mode.
		$4020-$FFFF	$BFE0	Cartridge space: PRG ROM, PRG RAM, and mapper registers (See Note)
	*/
	if (position <= 0x1FFF)
	{
		return m_cpu_ram_buffer[position & 0x07FF];
	}
	else if (position >= 0x2000 && position <= 0x3FFF)
	{
		PPU& ppu = NESConsole::GetInstance()->GetPPU();
		const uint16_t mirrored_position = position & 0x2007;
		switch (mirrored_position)
		{
			case 0x2000:
				return ppu.GetControlRegister().Register;
			case 0x2001:
				return ppu.GetMaskRegister().Register;
			case 0x2002:
				return ppu.GetStatusRegister().Register;
			case 0x2003:
				return ppu.GetOAMAddress();
			case 0x2004:
				return ppu.GetOAMData();
			case 0x2005:
				return ppu.GetScroll();
			case 0x2006:
				return ppu.GetAddress();
			case 0x2007:
				return ppu.GetData();
			default:
				assert(false);
		}
	}
	else if (position >= 0x4000 && position <= 0x4017)
	{
		// TODO: read from APU 
		assert(false);
	}
	else if (position >= 0x4018 && position <= 0x401F)
	{
		// TODO: read from mystery APU
		assert(false);
	}
	else if (position >= 0x4020 && position <= 0xFFFF)
	{
		return m_rom.GetByte(position);
	}
	std::cout << "Unknown memory location: 0x" << std::uppercase << std::hex << std::setw(4) << std::setfill('0')
				<< position << std::endl;
	exit(1);
	return 0;
}

uint16_t Memory::CPUReadShort(const uint16_t position) const
{
	uint16_t return_value = 0;
	const uint8_t low = CPUReadByte(position);
	const uint8_t high = CPUReadByte(position + 1);
	return_value = high << 8;
	return_value |= low;
	return return_value;
}

void Memory::CPUWriteByte(const uint16_t position, uint8_t value)
{
/*
	$0000-$07FF	$0800	2KB internal RAM (0-2047)
	$0800-$0FFF	$0800	Mirrors of $0000-$07FF (2048-4095)
	$1000-$17FF	$0800
	$1800-$1FFF	$0800
	$2000-$2007	$0008	NES PPU registers
	$2008-$3FFF	$1FF8	Mirrors of $2000-2007 (repeats every 8 bytes)
	$4000-$4017	$0018	NES APU and I/O registers
	$4018-$401F	$0008	APU and I/O functionality that is normally disabled. See CPU Test Mode.
	$4020-$FFFF	$BFE0	Cartridge space: PRG ROM, PRG RAM, and mapper registers (See Note)
*/
	if (position <= 0x1FFF)
	{
		m_cpu_ram_buffer[position & 0x07FF] = value;
	}
	else if (position >= 0x2000 && position <= 0x3FFF)
	{
		PPU& ppu = NESConsole::GetInstance()->GetPPU();
		const uint16_t mirrored_position = position & 0x2007;
		switch (mirrored_position)
		{
		case 0x2000:
			ppu.SetControlRegister(value);
		case 0x2001:
			ppu.SetMaskRegister(value);
		case 0x2002:
			ppu.SetStatusRegister(value);
		case 0x2003:
			ppu.SetOAMAddress(value);
		case 0x2004:
			ppu.SetOAMData(value);
		case 0x2005:
			ppu.SetScroll(value);
		case 0x2006:
			ppu.SetAddress(value);
		case 0x2007:
			ppu.SetData(value);
		default:
			assert(false);
		}
	}
	else if (position >= 0x4000 && position <= 0x4017)
	{
		// TODO: write to APU 
		assert(false);
	}
	else if (position >= 0x4018 && position <= 0x401F)
	{
		// TODO: write to mystery APU
		assert(false);
	}
	else if (position >= 0x4020 && position <= 0xFFFF)
	{
		// TODO: no writing to ROM
		assert(false);
	}
	else
	{
		std::cout << "Unknown memory location: 0x" << std::uppercase << std::hex << std::setw(4) << std::setfill('0')
			<< position << std::endl;
		exit(1);
	}
}

void Memory::Reset()
{
	memset(m_cpu_ram_buffer, 0x00, 2 * 1024);
	m_rom.Reset();
}