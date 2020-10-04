#include "ROM.h"
#include <fstream>
#include <cstring>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include "Mappers/Mapper000.h"

using namespace std;

void ROM::Load(const std::string &path)
{
	assert(m_rom_buffer == nullptr);
	ifstream input_stream;
	input_stream.open(path, ios::binary);
	input_stream.seekg(0, ios::end);
	const auto file_length = static_cast<int>(input_stream.tellg());
	input_stream.seekg(0, ios::beg);
	m_rom_buffer = new uint8_t[file_length];
	input_stream.read((char *) m_rom_buffer, file_length);
	input_stream.close();

	// https://wiki.nesdev.com/w/index.php/INES#iNES_file_format
	//	fill out header
	const int HEADER_SIZE = 16;
	std::memcpy(m_header_data.raw_data, m_rom_buffer, HEADER_SIZE);
	m_header_data.prg_rom_size_16 = (int) m_header_data.raw_data[4];
	m_header_data.chr_rom_size_8 = (int) m_header_data.raw_data[5];
	m_header_data.m_flags_6.Flag = m_header_data.raw_data[6];
	m_header_data.m_flags_7.Flag = m_header_data.raw_data[7];
	m_header_data.prg_ram_size_8 = (int) m_header_data.raw_data[8];
	m_header_data.flags_9 = m_header_data.raw_data[9];
	m_header_data.flags_10 = m_header_data.raw_data[10];

	//HEADER_SIZE + (16 * KB_SIZE * self.num_prg_blocks
	const int KB = 1024;
	m_prg_size = (16 * KB * m_header_data.prg_rom_size_16);
	m_prg_buffer = new uint8_t[m_prg_size];
	std::memcpy(m_prg_buffer, &m_rom_buffer[HEADER_SIZE], m_prg_size);
	m_chr_size = 8 * KB * m_header_data.chr_rom_size_8;
	// chr ram
	if (HasChrRam())
	{
		m_chr_size = 8 * KB;
	}
	m_chr_buffer = new uint8_t[m_chr_size];
	std::memcpy(m_chr_buffer, &m_rom_buffer[HEADER_SIZE + m_prg_size], m_chr_size);
	if (GetHeaderData().GetMapperNumber() == 0)
	{
		m_mapper = new Mapper000(m_header_data);
	}
	else
	{
		std::cout << "Unsupported mapper: " << GetHeaderData().GetMapperNumber() << std::endl;
		exit(1);
	}
}

void ROM::Reset()
{
	delete[] m_rom_buffer;
	m_rom_buffer = nullptr;
	delete[] m_prg_buffer;
	m_prg_buffer = nullptr;
	delete[] m_chr_buffer;
	m_chr_buffer = nullptr;
	memset(m_header_data.raw_data, 0, 16);
}

void ROM::PPUWriteByte(const uint16_t position, const uint8_t value)
{
	assert(position >= 0 && position < m_chr_size);
	m_chr_buffer[position] = value;
}

uint8_t ROM::PPUReadByte(const uint16_t position) const
{
	assert(position >= 0 && position < m_chr_size);
	return m_chr_buffer[position];
}

uint8_t ROM::CPUReadByte(const uint16_t position) const
{
	assert(position >= 0 && position <= 0xFFFF);
	assert(m_prg_buffer != nullptr);
	// https://wiki.nesdev.com/w/index.php/NROM
	uint16_t mapped_position;
	m_mapper->CPUReadByte(position, mapped_position);
	if (mapped_position > m_prg_size)
	{
		// TODO: this needs to return whatever is on the bus from the last
		// read/write
		return 0xFF;
	}
	assert(mapped_position < m_prg_size);
	return m_prg_buffer[mapped_position];
}

const iNESHeader &ROM::GetHeaderData() const
{
	return m_header_data;
}