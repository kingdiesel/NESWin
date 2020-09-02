#include "PPU.h"
#include "Source/NESConsole.h"

void PPU::PowerUp()
{
	// https://wiki.nesdev.com/w/index.php/PPU_power_up_state
}

void PPU::Reset()
{
	// https://wiki.nesdev.com/w/index.php/PPU_power_up_state
}

void PPU::Run()
{
	m_even_frame = !m_even_frame;
	const bool show_background = GetMaskRegister().Bits.m_show_bkgrnd;
	const bool show_sprites = GetMaskRegister().Bits.m_show_sprites;
	const bool rendering_disabled = !(show_background && show_sprites);
	// The PPU renders 262 scanlines per frame. Each scanline lasts for 
	// 341 PPU clock cycles (113.667 CPU clock cycles; 1 CPU cycle = 3 PPU cycles), 
	// with each clock cycle producing one pixel.

	if (cycles > 340) 
	{
		cycles -= 341;
		++scanlines;
	}

	if (scanlines >=0 && scanlines <= 239)
	{ 
		//  drawing

	}
	else if (scanlines == 241 && cycles == 1) 
	{
		m_reg_status.Bits.m_vertical_blank_started = 1;
		// if we are Generate an NMI at the start of the vertical blanking interval
		if (GetControlRegister().Bits.m_generate_nmi == 1)
		{
			m_cpu_nmi = true;
		}
	}
	else if (scanlines == 261 && cycles == 1) 
	{    
		m_reg_status.Bits.m_vertical_blank_started = 0;
		m_cpu_nmi = false;
		scanlines = 0;
		SetFrameReady(true);
	}
	cycles++;
}

void PPU::SetOAMDMA(const uint8_t value)
{
	// https://wiki.nesdev.com/w/index.php/PPU_registers#OAM_DMA_.28.244014.29_.3E_write
	// Writing $XX will upload 256 bytes of data from CPU page $XX00-$XXFF 
	// to the internal PPU OAM. The CPU is suspended during the transfer, 
	// which will take 513 or 514 cycles after the $4014 write tick. 
	// (1 dummy read cycle while waiting for writes to complete, +1 if on 
	// an odd CPU cycle, then 256 alternating read/write cycles.)
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint16_t start_address = value;
	start_address = start_address << 8;
	
	uint16_t end_address = value;
	end_address = end_address << 8;
	end_address |= 0x00FF;
	for (uint16_t i = start_address; i <= end_address; ++i)
	{
		memory.PPUWriteOAM(i - start_address, memory.CPUReadByte(i));
	}
}

uint8_t PPU::GetOAMData()
{
	return NESConsole::GetInstance()->GetMemory().PPUReadOAM(m_reg_oam_addr);
}

void PPU::SetOAMData(const uint8_t value)
{
	// https://wiki.nesdev.com/w/index.php/PPU_registers#OAMDATA
	NESConsole::GetInstance()->GetMemory().PPUWriteOAM(m_reg_oam_addr, value);
	m_reg_oam_addr += 1;
}

void PPU::SetData(const uint8_t value)
{
	// https://wiki.nesdev.com/w/index.php/PPU_registers#PPUDATA
	m_ppu_data = value;
	NESConsole::GetInstance()->GetMemory().PPUWriteByte(m_ppu_full_addr, value);
	if (GetControlRegister().Bits.m_vram_add == 0)
	{
		m_ppu_full_addr += 1;
	}
	else
	{
		m_ppu_full_addr += 32;
	}
}
