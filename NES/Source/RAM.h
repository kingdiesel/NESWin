#ifndef NES_RAM_H
#define NES_RAM_H
#include <cstdint>

class RAM
{
public:
	RAM();

	uint8_t GetByte(const uint16_t position) const;

	uint16_t GetShort(const uint16_t position) const;

	void SetByte(const uint16_t position, uint8_t value);

	void Reset();
private:
	uint8_t *m_ram_buffer = nullptr;
};


#endif //NES_RAM_H
