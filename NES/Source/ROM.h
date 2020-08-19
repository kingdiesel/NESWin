#ifndef NES_ROM_H
#define NES_ROM_H

#include <string>

struct iNESHeader
{
	int prg_rom_size_16 = 0;
	int chr_rom_size_8 = 0;
	uint8_t flags_6 = 0;
	uint8_t flags_7 = 0;
	int prg_ram_size_8 = 0;
	uint8_t flags_9 = 0;
	uint8_t flags_10 = 0;
	uint8_t raw_data[16] = {0x00};
};

class ROM
{
public:
	void Load(const std::string &path);

	uint8_t GetByte(const uint16_t position) const;

	uint16_t GetShort(const uint16_t position) const;

	void GetTile(uint16_t position, uint8_t *tile_data, int tile_data_size) const;

	void Reset();

	const iNESHeader &GetHeaderData() const;

private:
	uint16_t GetMappedPosition(const uint16_t position) const;

	uint8_t *m_rom_buffer = nullptr;
	uint8_t *m_prg_buffer = nullptr;
	uint8_t *m_chr_buffer = nullptr;
	int m_prg_size = 0;
	int m_chr_size = 0;
	iNESHeader m_header_data;
};


#endif //NES_ROM_H
