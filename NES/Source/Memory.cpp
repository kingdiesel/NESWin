#include <assert.h>
#include <iostream>
#include <iomanip>
#include "Memory.h"
#include "NESConsole.h"

Memory::Memory()
{
	
	// $0000-$07FF	$0800	2KB internal RAM (0-2047)
	// https://wiki.nesdev.com/w/index.php/CPU_memory_map
	m_cpu_ram_buffer = new uint8_t[2 * 1024];

	// The NES has 2KB of RAM dedicated to the PPU
	// https://wiki.nesdev.com/w/index.php/PPU_memory_map
	m_ppu_ram_buffer = new uint8_t[2 * 1024];
	
	memset(m_cpu_ram_buffer, 0x00, 2 * 1024);
	memset(m_cpu_ram_buffer, 0x00, 2 * 1024);
	memset(m_palette_buffer, 0x00, 32);
	memset(m_object_attribute_memory, 0x00, 256);
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
			{
				// https://wiki.nesdev.com/w/index.php/PPU_scrolling#.242002_read
				// https://wiki.nesdev.com/w/index.php/PPU_registers#PPUSTATUS
				// Reading the status register will clear bit 7 mentioned 
				// above and also the address latch used by PPUSCROLL 
				// and PPUADDR. It does not clear the sprite 0 hit 
				// or overflow bit.
				const uint8_t register_value = ppu.GetStatusRegister().Register;

				// reset latch
				ppu.ResetWriteToggle();

				// clear vblank flag
				ppu.SetStatusRegister(register_value & 0x80);

				// return cached value with those bits still set?
				return register_value;
			}
			case 0x2003:
				return ppu.GetOAMAddress();
			case 0x2004:
				return ppu.GetOAMData();
			case 0x2005:
				// TODO: how does this work with latching
				return ppu.GetScroll();
			case 0x2006:
				// TODO: how does this work with latching
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
		return 0;
	}
	else if (position >= 0x4018 && position <= 0x401F)
	{
		// TODO: read from mystery APU
		return 0;
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
			break;
		case 0x2001:
			ppu.SetMaskRegister(value);
			break;
		case 0x2002:
			ppu.SetStatusRegister(value);
			break;
		case 0x2003:
			ppu.SetOAMAddress(value);
			break;
		case 0x2004:
			ppu.SetOAMData(value);
			break;
		case 0x2005:
			ppu.SetScroll(value);
			break;
		case 0x2006:
			ppu.SetAddress(value);
			break;
		case 0x2007:
			ppu.SetData(value);
			break;
		default:
			assert(false);
		}
	}
	else if (position == 0x4014)
	{
		PPU& ppu = NESConsole::GetInstance()->GetPPU();
		ppu.SetOAMDMA(value);
		// TODO: if this occurs, need to copy stuff
		// to sprite memory
		// https://wiki.nesdev.com/w/index.php/PPU_registers#OAM_DMA_.28.244014.29_.3E_write
	}
	else if (position >= 0x4000 && position <= 0x4017)
	{
		// TODO: write to APU 
		//assert(false);
	}
	else if (position >= 0x4018 && position <= 0x401F)
	{
		// TODO: write to mystery APU
		//assert(false);
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

bool IsBackgroundFallthrough(const uint16_t position)
{
	if (position == 0x3F04
		|| position == 0x3F08
		|| position == 0x3F0C
		|| position == 0x3F10
		|| position == 0x3F14
		|| position == 0x3F18
		|| position == 0x3F1C
		)
	{
		return true;
	}
	return false;
}

uint8_t Memory::PPUReadByte(const uint16_t position) const
{
	/*
		$0000-$0FFF	$1000	Pattern table 0
		$1000-$1FFF	$1000	Pattern table 1
		$2000-$23FF	$0400	Nametable 0
		$2400-$27FF	$0400	Nametable 1
		$2800-$2BFF	$0400	Nametable 2
		$2C00-$2FFF	$0400	Nametable 3
		$3000-$3EFF	$0F00	Mirrors of $2000-$2EFF
		$3F00-$3F1F	$0020	Palette RAM indexes
		$3F20-$3FFF	$00E0	Mirrors of $3F00-$3F1F
	*/
	if (position >= 0x0000 && position <= 0x1FFF)
	{
		return m_rom.GetChrRom()[position];
	}
	else if (position >= 0x2000 && position <= 0x3EFF)
	{
		const uint16_t mirrored_position = position & 0x2FFF;
		return m_ppu_ram_buffer[mirrored_position];
	}
	else if (position >= 0x3F00 && position <= 0x3FFF)
	{
		// https://wiki.nesdev.com/w/index.php/PPU_palettes
		const uint16_t mirrored_position = position & 0x3F1F;
		if (IsBackgroundFallthrough(mirrored_position))
		{
			return m_palette_buffer[0];
		}
		const int shifted_down = mirrored_position - 0x3F00;
		assert(shifted_down < 32);
		return m_palette_buffer[shifted_down];
	}
	else
	{
		std::cout << "Unknown memory location: 0x" << std::uppercase << std::hex << std::setw(4) << std::setfill('0')
			<< position << std::endl;
		exit(1);
	}
	return 0;
}

void Memory::PPUWriteByte(const uint16_t position, uint8_t value)
{
	/*
		$0000-$0FFF	$1000	Pattern table 0
		$1000-$1FFF	$1000	Pattern table 1
		$2000-$23FF	$0400	Nametable 0
		$2400-$27FF	$0400	Nametable 1
		$2800-$2BFF	$0400	Nametable 2
		$2C00-$2FFF	$0400	Nametable 3
		$3000-$3EFF	$0F00	Mirrors of $2000-$2EFF
		$3F00-$3F1F	$0020	Palette RAM indexes
		$3F20-$3FFF	$00E0	Mirrors of $3F00-$3F1F
	*/
	if (position >= 0x0000 && position <= 0x1FFF)
	{
		// we're probably not writing to chr rom..yet
		assert(false);
	}
	else if (position >= 0x2000 && position <= 0x3EFF)
	{
		const uint16_t mirrored_position = position & 0x2FFF;
		m_ppu_ram_buffer[mirrored_position] = value;
	}
	else if (position >= 0x3F00 && position <= 0x3FFF)
	{
		// https://wiki.nesdev.com/w/index.php/PPU_palettes
		const uint16_t mirrored_position = position & 0x3F1F;
		if (IsBackgroundFallthrough(mirrored_position))
		{
			// is writing to the fallthrough byte supported?
			// if so -- remove this assert
			//assert(false);
		}
		const int shifted_down = mirrored_position - 0x3F00;
		m_palette_buffer[shifted_down] = value;
	}
	else
	{
		std::cout << "Unknown memory location: 0x" << std::uppercase << std::hex << std::setw(4) << std::setfill('0')
			<< position << std::endl;
		exit(1);
	}
}

void Memory::PPUWriteOAM(const uint8_t index, const uint8_t value)
{
	m_object_attribute_memory[index] = value;
}

uint8_t Memory::PPUReadOAM(const uint8_t index)
{
	return m_object_attribute_memory[index];
}

void Memory::Reset()
{
	memset(m_cpu_ram_buffer, 0x00, 2 * 1024);
	memset(m_ppu_ram_buffer, 0x00, 2 * 1024);
	m_rom.Reset();
}