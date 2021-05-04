#pragma once

#include <string>

struct iNESHeader
{
	int prg_rom_size_16 = 0;
	int chr_rom_size_8 = 0;
	
	//	https://wiki.nesdev.com/w/index.php/INES#Flags_6
	union Flag6
	{
		struct Flag6Bits {
			uint8_t m_mirroring : 1;
			uint8_t m_battery : 1;
			uint8_t m_trainer : 1;
			uint8_t m_ignore_mirroring : 1;
			uint8_t m_lower_mapper : 4;

		} Bits;
		uint8_t Flag = 0x00;
	};

	//	https://wiki.nesdev.com/w/index.php/INES#Flags_7
	union Flag7
	{
		struct Flag7Bits {
			uint8_t m_unisystem : 1;
			uint8_t m_playchoice : 1;
			uint8_t m_nes_20 : 2;
			uint8_t m_upper_mapper : 4;

		} Bits;
		uint8_t Flag = 0x00;
	};

	Flag6 m_flags_6;
	Flag7 m_flags_7;
	uint8_t prg_ram_size_8 = 0;
	uint8_t flags_9 = 0;
	uint8_t flags_10 = 0;
	uint8_t raw_data[16] = {0x00};

	int GetMapperNumber() const
	{
		return m_flags_6.Bits.m_lower_mapper | m_flags_7.Bits.m_upper_mapper;
	}
};



class ROM
{
public:
	void Load(const std::string &path);

	uint8_t CPUReadByte(const uint16_t position) const;
	uint8_t PPUReadByte(const uint16_t position) const;
	void PPUWriteByte(const uint16_t position, const uint8_t value);
	void CPUWriteByte(const uint16_t position, const uint8_t value);
	void Reset();
	bool HasChrRam() const { return m_header_data.chr_rom_size_8 == 0; }
	int GetMapperNumber() const { return m_header_data.GetMapperNumber(); }
	const iNESHeader &GetHeaderData() const;
	class IMapper* GetMapper() { return m_mapper; }
	class IMapper* GetMapper() const { return m_mapper; }

private:
	class IMapper* m_mapper = nullptr;
	uint8_t *m_rom_buffer = nullptr;
	uint8_t *m_prg_buffer = nullptr;
	uint8_t *m_chr_buffer = nullptr;
	int m_prg_size = 0;
	int m_chr_size = 0;
	iNESHeader m_header_data;
};
