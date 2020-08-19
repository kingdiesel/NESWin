#ifndef NES_RAM_H
#define NES_RAM_H


class RAM
{
public:
	RAM();

	unsigned char GetByte(const unsigned short position) const;

	unsigned short GetShort(const unsigned short position) const;

	void SetByte(const unsigned short position, unsigned char value);

	void Reset();
private:
	unsigned char *m_ram_buffer = nullptr;
};


#endif //NES_RAM_H
