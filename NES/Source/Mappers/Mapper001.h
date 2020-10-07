#pragma once
#include "IMapper.h"

// https://wiki.nesdev.com/w/index.php/NROM
class Mapper001 : public IMapper
{
public:
	Mapper001(const iNESHeader& header);
	virtual bool CPUReadByte(const uint16_t position, uint16_t& mapped_position, uint8_t& value) override;
	virtual bool CPUWriteByte(const uint16_t position, uint16_t& mapped_position, const uint8_t value) override;

	virtual bool PPUReadByte(const uint16_t position, uint16_t& mapped_position, uint8_t& value) override;
	virtual bool PPUWriteByte(const uint16_t position, uint16_t& mapped_position, const uint8_t value) override;
	virtual MirrorMode GetMirrorMode() override;
private:
	// https://wiki.nesdev.com/w/index.php/MMC1#Registers
	uint8_t m_load_shift_register = 0x00;

	//4bit0
	//-----
	//CPPMM
	//|||||
	//|||++ - Mirroring(0: one - screen, lower bank; 1: one - screen, upper bank;
	//|||               2: vertical; 3: horizontal)
	//|++-- - PRG ROM bank mode(0, 1: switch 32 KB at $8000, ignoring low bit of bank number;
	//|			2: fix first bank at $8000 and switch 16 KB bank at $C000;
	//|			3: fix last bank at $C000 and switch 16 KB bank at $8000)
	//+ ---- - CHR ROM bank mode(0: switch 8 KB at a time; 1: switch two separate 4 KB banks)
	union ControlRegister
	{
		struct Bits {
			uint8_t m_mirroring : 2;
			uint8_t m_prg_rom_bank : 2;
			uint8_t m_chr_rom_bank : 1;
			uint8_t m_unused : 3;

		} Bits;
		uint8_t Register = 0x00;
	};

	// https://wiki.nesdev.com/w/index.php/MMC1#CHR_bank_0_.28internal.2C_.24A000-.24BFFF.29
	union ChrBankRegister
	{
		struct Bits {
			uint8_t m_chr_bank_select : 5;
			uint8_t m_unused : 3;

		} Bits;
		uint8_t Register = 0x00;
	};

	//4bit0
	//---- -
	//RPPPP
	//|||||
	//|++++ - Select 16 KB PRG ROM bank(low bit ignored in 32 KB mode)
	//+---- - PRG RAM chip enable(0: enabled; 1: disabled; ignored on MMC1A)
	union PrgBankRegister
	{
		struct Bits {
			uint8_t m_prg_rom_bank_select : 4;
			uint8_t m_prg_ram_enable : 1;
			uint8_t m_unused : 3;
		} Bits;
		uint8_t Register = 0x00;
	};

	ControlRegister m_control_register;
	//4bit0
	//---- -
	//CCCCC
	//|||||
	//++++ + -Select 4 KB or 8 KB CHR bank at PPU $0000(low bit ignored in 8 KB mode)
	ChrBankRegister m_chr_bank_0_register;

	//4bit0
	//---- -
	//CCCCC
	//|||||
	//++++ + -Select 4 KB CHR bank at PPU $1000(ignored in 8 KB mode)
	ChrBankRegister m_chr_bank_1_register;

	PrgBankRegister m_prg_bank_register;
	int m_load_shift_register_count = 0;
	
	uint8_t m_char_bank_selector_low_addr = 0x00;
	uint8_t m_char_bank_selector_high_addr = 0x00;
	uint8_t m_char_bank_selector_8 = 0x00;

	uint8_t m_prg_bank_selector_16_low_addr = 0x00;
	uint8_t m_prg_bank_selector_16_high_addr = 0x00;
	uint8_t m_prg_bank_select_32 = 0x00;

	uint8_t* m_prg_ram_bank = nullptr;
};