#include <assert.h>
#include <iostream>
#include <iomanip>
#include "Memory.h"
#include "NESConsole.h"
#include "Mappers/IMapper.h"
const int ppu_ram_size = 16 * 1024;
Memory::Memory()
{
	
	// $0000-$07FF	$0800	2KB internal RAM (0-2047)
	// https://wiki.nesdev.com/w/index.php/CPU_memory_map
	m_cpu_ram_buffer = new uint8_t[2 * 1024];

	// The NES has 2KB of RAM dedicated to the PPU
	// https://wiki.nesdev.com/w/index.php/PPU_memory_map
	m_ppu_ram_buffer = new uint8_t[ppu_ram_size];
	
	memset(m_cpu_ram_buffer, 0x00, 2 * 1024);
	memset(m_ppu_ram_buffer, 0x00, ppu_ram_size);
	memset(m_palette_buffer, 0x00, 32);
	memset(m_primary_oam, 0x00, 256);
	memset(m_secondary_oam, 0x00, 64);
#if olc
	memset(tblName, 0x00, 2048);
#endif
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
		assert((position & 0x07FF) < (2 * 1024));
		return m_cpu_ram_buffer[position & 0x07FF];
	}
	else if (position >= 0x2000 && position <= 0x3FFF)
	{
		PPU& ppu = NESConsole::GetInstance()->GetPPU();
		const uint16_t mirrored_position = position & 0x2007;
		switch (mirrored_position)
		{
			case 0x2000:
				return 0;
			case 0x2001:
				return 0;
			case 0x2002:
			{
				// https://wiki.nesdev.com/w/index.php/PPU_scrolling#.242002_read
				// https://wiki.nesdev.com/w/index.php/PPU_registers#PPUSTATUS
				uint8_t register_value = ppu.GetStatusRegister().Register;
				if (!GetReadyOnlyMode())
				{
					// Reading the status register will clear bit 7 mentioned 
					// above and also the address latch used by PPUSCROLL 
					// and PPUADDR. It does not clear the sprite 0 hit 
					// or overflow bit.

					// reset latch
					ppu.ResetWriteToggle();

					// clear vblank flag
					ppu.SetStatusRegister(register_value & 0x80);
				}
				

				// return cached value with those bits still set?
				return (register_value & 0xE0) | (ppu.GetData() & 0x1F);
			}
			case 0x2003:
				return 0;
			case 0x2004:
				return ppu.GetOAMData();
			case 0x2005:
				return 0;
			case 0x2006:
				return 0;
			case 0x2007:
			{
				// https://wiki.nesdev.com/w/index.php/PPU_registers#PPUDATA
				uint8_t data = ppu.GetData();
				if (!GetReadyOnlyMode())
				{
					PPUScrollRegister& current_vram = ppu.GetCurrentVram();
					ppu.SetData(PPUReadByte(current_vram.Register));
					if (current_vram.Register >= 0x3F00)
					{
						data = ppu.GetData();
					}

					current_vram.Register += (ppu.GetControlRegister().Bits.m_vram_add ? 32 : 1);
				}
				
				return data;
			}
			default:
				assert(false);
		}
	}
	else if (position >= 0x4000 && position <= 0x4017)
	{
		if (position == 0x4016)
		{
			bool button_state = false;
			// https://wiki.nesdev.com/w/index.php/Controller_reading
			// https://wiki.nesdev.com/w/index.php/Controller_reading_code
			switch (m_controller_poll)
			{
			case 0:
				button_state = NESConsole::GetInstance()->GetAPressed();
				break;
			case 1:
				button_state = NESConsole::GetInstance()->GetBPressed();
				break;
			case 2:
				button_state = NESConsole::GetInstance()->GetSelectPressed();
				break;
			case 3:
				button_state = NESConsole::GetInstance()->GetStartPressed();
				break;
			case 4:
				button_state = NESConsole::GetInstance()->GetUpPressed();
				break;
			case 5:
				button_state = NESConsole::GetInstance()->GetDownPressed();
				break;
			case 6:
				button_state = NESConsole::GetInstance()->GetLeftPressed();
				break;
			case 7:
				button_state = NESConsole::GetInstance()->GetRightPressed();
				break;
			}

			if (!GetReadyOnlyMode())
			{
				m_controller_poll++;
			}
			return button_state;
		}
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
		return m_rom.CPUReadByte(position);
	}
	std::cout << "Unknown memory location: 0x" << std::uppercase << std::hex << std::setw(4) << std::setfill('0')
				<< position << std::endl;
	assert(false);
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

void Memory::ClearSecondaryOAM()
{
	// we should clear to 0xFF hear because 0xFF means
	memset(m_secondary_oam, 0xFF, 64);
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
		assert((position & 0x07FF) < (2 * 1024));
		m_cpu_ram_buffer[position & 0x07FF] = value;
	}
	else if (position >= 0x2000 && position <= 0x3FFF)
	{
		PPU& ppu = NESConsole::GetInstance()->GetPPU();
		const uint16_t mirrored_position = position & 0x2007;
		switch (mirrored_position)
		{
		case 0x2000:
		{
			PPUScrollRegister& temp_vram = ppu.GetTempVram();
			ppu.SetControlRegister(value);
			temp_vram.Bits.m_nametable_x = ppu.GetControlRegister().Bits.m_nametable_x;
			temp_vram.Bits.m_nametable_y = ppu.GetControlRegister().Bits.m_nametable_y;
		}
		break;
		case 0x2001:
			ppu.SetMaskRegister(value);
			break;
		case 0x2002:
			break;
		case 0x2003:
			ppu.SetOAMAddress(value);
			break;
		case 0x2004:
			ppu.SetOAMData(value);
			break;
		case 0x2005:
		{
			// https://wiki.nesdev.com/w/index.php/PPU_scrolling#.242005_first_write_.28w_is_0.29
			PPUScrollRegister& temp_vram = ppu.GetTempVram();
			if (ppu.GetWriteToggle() == 0)
			{
				ppu.SetFineX(value & 0x07);
				temp_vram.Bits.m_coarse_x = value >> 3;
				ppu.SetWriteToggle(1);
			}
			else
			{
				temp_vram.Bits.m_fine_y = value & 0x07;
				temp_vram.Bits.m_coarse_y = value >> 3;
				ppu.SetWriteToggle(0);
			}
		}
		break;
		case 0x2006:
		{
			// https://wiki.nesdev.com/w/index.php/PPU_registers#PPUADDR
			PPUScrollRegister& temp_vram = ppu.GetTempVram();
			if (ppu.GetWriteToggle() == 0)
			{
				temp_vram.Register = (uint16_t)((value & 0x3F) << 8) | (temp_vram.Register & 0x00FF);
				ppu.SetWriteToggle(1);
			}
			else
			{
				temp_vram.Register = (temp_vram.Register & 0xFF00) | value;
				ppu.MoveTempVramToCurrent();
				ppu.SetWriteToggle(0);

			}
		}
		break;
		case 0x2007:
		{
			// https://wiki.nesdev.com/w/index.php/PPU_registers#PPUDATA
			PPUScrollRegister& current_vram = ppu.GetCurrentVram();
			PPUWriteByte(current_vram.Register, value);
			current_vram.Register += (ppu.GetControlRegister().Bits.m_vram_add ? 32 : 1);
		}
		break;
		default:
			assert(false);
		}
	}
	else if (position >= 0x4000 && position <= 0x4017)
	{
		// TODO: write to APU 
		if (position == 0x4016)
		{
			// https://wiki.nesdev.com/w/index.php/Controller_reading
			if (value == 1)
			{
				// start poll
				m_controller_poll = 0;
			}
		}
		else if (position == 0x4014)
		{
			// https://wiki.nesdev.com/w/index.php/PPU_registers#OAM_DMA_.28.244014.29_.3E_write
			NESConsole::GetInstance()->SetDmaPage(value);
			NESConsole::GetInstance()->SetDmaAddr(0x00);
			NESConsole::GetInstance()->SetDmaTransfer(true);
		}
	}
	else if (position >= 0x4018 && position <= 0x401F)
	{
		// TODO: write to mystery APU
		//assert(false);
	}
	else if (position >= 0x4020 && position <= 0xFFFF)
	{
		m_rom.CPUWriteByte(position, value);
	}
	else
	{
		std::cout << "Unknown memory location: 0x" << std::uppercase << std::hex << std::setw(4) << std::setfill('0')
			<< position << std::endl;
		assert(false);
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

uint8_t Memory::PPUReadByte(uint16_t position) const
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
	position &= 0x3FFF;
	if (position >= 0x0000 && position <= 0x1FFF)
	{
		return m_rom.PPUReadByte(position);
	}
	else if (position >= 0x2000 && position <= 0x3EFF)
	{
#if olc
		uint16_t addr = position;
		addr &= 0x0FFF;
		uint8_t data = 0x00;
		//  0: horizontal (vertical arrangement)
		//	1: vertical (horizontal arrangement)
		if (GetROM().GetHeaderData().m_flags_6.Bits.m_mirroring == 0)
		{
			if (addr >= 0x0000 && addr <= 0x03FF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0400 && addr <= 0x07FF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0800 && addr <= 0x0BFF)
				data = tblName[1][addr & 0x03FF];
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				data = tblName[1][addr & 0x03FF];
			return data;
		}
		else
		{
			if (addr >= 0x0000 && addr <= 0x03FF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0400 && addr <= 0x07FF)
				data = tblName[1][addr & 0x03FF];
			if (addr >= 0x0800 && addr <= 0x0BFF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				data = tblName[1][addr & 0x03FF];
			return data;
		}
#else
		uint16_t mirrored_position = GetMirroredPosition(position & 0x2FFF);
		assert(mirrored_position >= 0 && mirrored_position < ppu_ram_size);
		return m_ppu_ram_buffer[mirrored_position];
#endif
	}
	else if (position >= 0x3F00 && position <= 0x3FFF)
	{
#if olc
		PPU& ppu = NESConsole::GetInstance()->GetPPU();
		position &= 0x001F;
		if (position == 0x0010) position = 0x0000;
		if (position == 0x0014) position = 0x0004;
		if (position == 0x0018) position = 0x0008;
		if (position == 0x001C) position = 0x000C;
		return m_palette_buffer[position] & (ppu.GetMaskRegister().Bits.m_grayscale ? 0x30 : 0x3F);
#else
		PPU& ppu = NESConsole::GetInstance()->GetPPU();
		// https://wiki.nesdev.com/w/index.php/PPU_palettes
		uint16_t mirrored_position = position & 0x3F1F;
		if (mirrored_position == 0x3F10)
		{
			mirrored_position = 0x3F00;
		}
		else if (mirrored_position == 0x3F14)
		{
			mirrored_position = 0x3F04;
		}
		else if (mirrored_position == 0x3F18)
		{
			mirrored_position = 0x3F08;
		}
		else if (mirrored_position == 0x3F1C)
		{
			mirrored_position = 0x3F0C;
		}
		const int shifted_down = mirrored_position - 0x3F00;
		assert(shifted_down >= 0 && shifted_down < 32);
		return m_palette_buffer[shifted_down];
#endif
		
	}
	else
	{
		std::cout << "Unknown memory location: 0x" << std::uppercase << std::hex << std::setw(4) << std::setfill('0')
			<< position << std::endl;
		assert(false);
		exit(1);
	}
	return 0;
}

void Memory::PPUWriteByte(uint16_t position, uint8_t value)
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
	position &= 0x3FFF;
	if (position >= 0x0000 && position <= 0x1FFF)
	{
		m_rom.PPUWriteByte(position, value);
	}
	else if (position >= 0x2000 && position <= 0x3EFF)
	{
#if olc
		uint16_t addr = position;
		addr &= 0x0FFF;
		if (GetROM().GetHeaderData().m_flags_6.Bits.m_mirroring == 0)
		{
			if (addr >= 0x0000 && addr <= 0x03FF)
				tblName[0][addr & 0x03FF] = value;
			if (addr >= 0x0400 && addr <= 0x07FF)
				tblName[0][addr & 0x03FF] = value;
			if (addr >= 0x0800 && addr <= 0x0BFF)
				tblName[1][addr & 0x03FF] = value;
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				tblName[1][addr & 0x03FF] = value;
		}
		else
		{
			if (addr >= 0x0000 && addr <= 0x03FF)
				tblName[0][addr & 0x03FF] = value;
			if (addr >= 0x0400 && addr <= 0x07FF)
				tblName[1][addr & 0x03FF] = value;
			if (addr >= 0x0800 && addr <= 0x0BFF)
				tblName[0][addr & 0x03FF] = value;
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				tblName[1][addr & 0x03FF] = value;
		}
#else
		uint16_t mirrored_position = GetMirroredPosition(position & 0x2FFF);
		assert(mirrored_position >=0 && mirrored_position < ppu_ram_size);
		m_ppu_ram_buffer[mirrored_position] = value;
#endif
	}
	else if (position >= 0x3F00 && position <= 0x3FFF)
	{
#if olc
		position &= 0x001F;
		if (position == 0x0010) position = 0x0000;
		if (position == 0x0014) position = 0x0004;
		if (position == 0x0018) position = 0x0008;
		if (position == 0x001C) position = 0x000C;
		m_palette_buffer[position] = value;
#else
		// https://wiki.nesdev.com/w/index.php/PPU_palettes
		uint16_t mirrored_position = position & 0x3F1F;
		if (mirrored_position == 0x3F10)
		{
			mirrored_position = 0x3F00;
		}
		else if (mirrored_position == 0x3F14)
		{
			mirrored_position = 0x3F04;
		}
		else if (mirrored_position == 0x3F18)
		{
			mirrored_position = 0x3F08;
		}
		else if (mirrored_position == 0x3F1C)
		{
			mirrored_position = 0x3F0C;
		}
		const int shifted_down = mirrored_position - 0x3F00;
		assert(shifted_down >= 0 && shifted_down < 32);
		m_palette_buffer[shifted_down] = value;
#endif
	}
	else
	{
		std::cout << "Unknown memory location: 0x" << std::uppercase << std::hex << std::setw(4) << std::setfill('0')
			<< position << std::endl;
		assert(false);
		exit(1);
	}
}

uint16_t Memory::GetMirroredPosition(const uint16_t position) const
{
	uint16_t mirrored_position = position;
	// https://wiki.nesdev.com/w/index.php/Mirroring#Nametable_Mirroring
	// https://wiki.nesdev.com/w/index.php/INES#Flags_6
	//  0: horizontal (vertical arrangement)
	//	1: vertical (horizontal arrangement)
	if (m_rom.GetMapper()->GetMirrorMode() == MirrorMode::Horizontal)
	{
		if (mirrored_position >= 0x2400 && mirrored_position < 0x2800)
		{
			mirrored_position &= 0x23FF;
		}
		else if (mirrored_position >= 0x2C00 && mirrored_position < 0x2FFF)
		{
			mirrored_position &= 0x2BFF;
		}
	}
	else if (m_rom.GetMapper()->GetMirrorMode() == MirrorMode::Vertical)
	{
		if (mirrored_position >= 0x2800 && mirrored_position < 0x2C00)
		{
			mirrored_position &= 0x23FF;
		}
		else if (mirrored_position >= 0x2C00 && mirrored_position < 0x2FFF)
		{
			mirrored_position &= 0x27FF;
		}
	}
	return mirrored_position;
}

void Memory::PPUWriteOAM(const uint8_t index, const uint8_t value)
{
	assert(index >= 0 && index < 256);
	m_primary_oam[index] = value;
}

uint8_t Memory::PPUReadOAM(const uint8_t index)
{
	assert(index >= 0 && index < 256);
	return m_primary_oam[index];
}

void Memory::Reset()
{
	memset(m_cpu_ram_buffer, 0x00, 2 * 1024);
	memset(m_ppu_ram_buffer, 0x00, ppu_ram_size);
	m_rom.Reset();
}