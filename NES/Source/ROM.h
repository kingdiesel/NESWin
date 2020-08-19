#ifndef NES_ROM_H
#define NES_ROM_H

#include <string>

struct iNESHeader
{
	int prg_rom_size_16 = 0;
	int chr_rom_size_8 = 0;
	unsigned char flags_6 = 0;
	unsigned char flags_7 = 0;
	int prg_ram_size_8 = 0;
	unsigned char flags_9 = 0;
	unsigned char flags_10 = 0;
	unsigned char raw_data[16] = {0x00};
};

class ROM
{
public:
	void Load(const std::string &path);

	unsigned char GetByte(const unsigned short position) const;

	unsigned short GetShort(const unsigned short position) const;

	void GetTile(unsigned short position, unsigned char *tile_data, int tile_data_size) const;

	void Reset();

	const iNESHeader &GetHeaderData() const;

private:
	unsigned short GetMappedPosition(const unsigned short position) const;

	unsigned char *m_rom_buffer = nullptr;
	unsigned char *m_prg_buffer = nullptr;
	unsigned char *m_chr_buffer = nullptr;
	int m_prg_size = 0;
	int m_chr_size = 0;
	iNESHeader m_header_data;
};


#endif //NES_ROM_H
