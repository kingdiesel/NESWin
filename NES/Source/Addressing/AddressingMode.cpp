//
//
#include <assert.h>
#include "AddressingMode.h"
#include "../CPU.h"
#include "../Memory.h"
#include <iomanip>

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AbsoluteAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	uint16_t destination = GetMemoryByte(cpu, memory);
	uint8_t value = GetMemoryByteValue(cpu, memory);
	std::ostringstream os;
	os << "$" << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << std::right << destination;
	os << " = " << std::setw(2) << std::setfill('0') << std::right << (int) value;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << os.str();
	out_string.append(spacing.str());
}

uint16_t AbsoluteAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	return memory.GetShort(cpu.GetRegisterProgramCounter() + 1);
}

uint8_t AbsoluteAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	return memory.GetByte(GetMemoryByte(cpu, memory));
}

uint16_t AbsoluteAddressingStrategy::GetSetAddress(const CPU &cpu, const Memory &memory) const
{
	return GetMemoryByte(cpu, memory);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AbsoluteYAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	uint16_t memory_address = memory.GetShort(cpu.GetRegisterProgramCounterPlus(1));
	uint16_t memory_offset = memory_address + cpu.GetRegisterY();
	uint8_t value = memory.GetByte(memory_offset);

	std::ostringstream os;
	os << "$" << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << std::right << memory_address;
	os << std::setw(0) << ",Y @ ";
	os << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << std::right << memory_offset;
	os << std::setw(0) << " = ";
	os << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << std::right << (int) value;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << os.str();
	out_string.append(spacing.str());
}

uint16_t AbsoluteYAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	uint16_t memory_address = memory.GetShort(cpu.GetRegisterProgramCounterPlus(1));
	memory_address += cpu.GetRegisterY();
	return memory_address;
}

uint8_t AbsoluteYAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	uint16_t memory_address = memory.GetShort(cpu.GetRegisterProgramCounterPlus(1));
	uint16_t memory_offset = memory_address + cpu.GetRegisterY();
	if (memory_address / 256 != memory_offset / 256)
	{
		m_addressing_cycles = 1;
	}
	else
	{
		m_addressing_cycles = 0;
	}
	return memory.GetByte(memory_offset);
}

uint16_t AbsoluteYAddressingStrategy::GetSetAddress(const CPU &cpu, const Memory &memory) const
{
	return GetMemoryByte(cpu, memory);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AbsoluteXAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	uint16_t memory_address = memory.GetShort(cpu.GetRegisterProgramCounterPlus(1));
	uint16_t memory_offset = memory_address + cpu.GetRegisterX();
	uint8_t value = memory.GetByte(memory_offset);

	std::ostringstream os;
	os << "$" << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << std::right << memory_address;
	os << std::setw(0) << ",X @ ";
	os << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << std::right << memory_offset;
	os << std::setw(0) << " = ";
	os << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << std::right << (int) value;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << os.str();
	out_string.append(spacing.str());
}

uint16_t AbsoluteXAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	uint16_t memory_address = memory.GetShort(cpu.GetRegisterProgramCounterPlus(1));
	memory_address += cpu.GetRegisterX();
	return memory_address;
}

uint8_t AbsoluteXAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	uint16_t memory_address = memory.GetShort(cpu.GetRegisterProgramCounterPlus(1));
	uint16_t memory_offset = memory_address + cpu.GetRegisterX();
	if (memory_address / 256 != memory_offset / 256)
	{
		m_addressing_cycles = 1;
	}
	else
	{
		m_addressing_cycles = 0;
	}
	return memory.GetByte(memory_offset);
}

uint16_t AbsoluteXAddressingStrategy::GetSetAddress(const CPU &cpu, const Memory &memory) const
{
	return GetMemoryByte(cpu, memory);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RelativeAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	uint16_t destination = GetMemoryByteValue(cpu, memory);
	std::ostringstream os;
	os << std::hex << "$" << std::uppercase << std::setw(27) << std::left << (int) destination;
	out_string.append(os.str());
}

uint8_t RelativeAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	return memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
}

uint16_t RelativeAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	uint8_t data_byte = GetMemoryByte(cpu, memory);
	uint16_t destination = cpu.GetRegisterProgramCounterPlus(2) + (uint16_t) data_byte;
	return destination;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void JMPAbsoluteAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	std::ostringstream os;
	os << std::hex << "$" << std::uppercase << std::setw(27) << std::left << GetMemoryByte(cpu, memory);
	out_string.append(os.str());
}

uint16_t JMPAbsoluteAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	return memory.GetShort(cpu.GetRegisterProgramCounterPlus(1));
}

uint8_t JMPAbsoluteAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	return memory.GetByte(GetMemoryByte(cpu, memory));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ZeroPageAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	uint8_t data_byte = GetMemoryByte(cpu, memory);
	uint8_t data_value = GetMemoryByteValue(cpu, memory);
	std::ostringstream os;
	os << "$" << std::uppercase << std::setw(2) << std::right << std::setfill('0') << std::hex << (int) data_byte
	   << " = " << std::setw(2) << (int) data_value;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << os.str();
	out_string.append(spacing.str());
}

uint8_t ZeroPageAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	return memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
}

uint8_t ZeroPageAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	uint8_t zero_page_address = GetMemoryByte(cpu, memory);
	uint8_t zero_page_value = memory.GetByte((uint16_t) zero_page_address);
	return zero_page_value;
}

uint16_t ZeroPageAddressingStrategy::GetSetAddress(const CPU &cpu, const Memory &memory) const
{
	return GetMemoryByte(cpu, memory);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ZeroPageXAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	uint8_t memory_address = memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
	uint8_t memory_offset = memory_address + cpu.GetRegisterX();
	uint8_t memory_value = memory.GetByte((uint16_t) memory_offset);

	std::ostringstream os;
	os << "$" << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << std::right << (int) memory_address;
	os << std::setw(0) << ",X @ ";
	os << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << std::right << (int) memory_offset;
	os << std::setw(0) << " = ";
	os << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << std::right << (int) memory_value;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << os.str();
	out_string.append(spacing.str());
}

uint8_t ZeroPageXAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	uint8_t memory_address = memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
	uint8_t memory_offset = memory_address + cpu.GetRegisterX();
	return memory_offset;
}

uint8_t ZeroPageXAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	uint8_t memory_address = memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
	uint8_t memory_offset = memory_address + cpu.GetRegisterX();
	uint8_t memory_value = memory.GetByte((uint16_t) memory_offset);
	return memory_value;
}

uint16_t ZeroPageXAddressingStrategy::GetSetAddress(const CPU &cpu, const Memory &memory) const
{
	return GetMemoryByte(cpu, memory);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ZeroPageYAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	uint8_t memory_address = memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
	uint8_t memory_offset = memory_address + cpu.GetRegisterY();
	uint8_t memory_value = memory.GetByte((uint16_t) memory_offset);

	std::ostringstream os;
	os << "$" << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << std::right << (int) memory_address;
	os << std::setw(0) << ",Y @ ";
	os << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << std::right << (int) memory_offset;
	os << std::setw(0) << " = ";
	os << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << std::right << (int) memory_value;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << os.str();
	out_string.append(spacing.str());
}

uint8_t ZeroPageYAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	uint8_t memory_address = memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
	uint8_t memory_offset = memory_address + cpu.GetRegisterY();
	return memory_offset;
}

uint8_t ZeroPageYAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	uint8_t memory_address = memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
	uint8_t memory_offset = memory_address + cpu.GetRegisterY();
	uint8_t memory_value = memory.GetByte((uint16_t) memory_offset);
	return memory_value;
}

uint16_t ZeroPageYAddressingStrategy::GetSetAddress(const CPU &cpu, const Memory &memory) const
{
	return GetMemoryByte(cpu, memory);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IndexedIndirectAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	uint8_t lsb_location = GetMemoryByte(cpu, memory);
	std::ostringstream lsbstream;
	lsbstream << "($";
	lsbstream << std::uppercase << std::hex << std::right << std::setfill('0') << std::setw(2) << (int) lsb_location;
	lsbstream << std::setw(0) << std::setfill(' ') << ",X)";
	lsb_location += cpu.GetRegisterX();
	uint8_t msb_location = lsb_location + (uint8_t) 1;
	uint16_t lsb = memory.GetByte(lsb_location);
	uint16_t msb = memory.GetByte(msb_location);
	uint16_t full_address = (msb << 8) | lsb;
	uint8_t value_at_address = memory.GetByte(full_address);

	lsbstream << " @ " << std::right << std::setfill('0') << std::setw(2) << (int) lsb_location;
	lsbstream << std::setfill(' ') << std::setw(0) << " = ";
	lsbstream << std::setfill('0') << std::setw(4) << full_address;
	lsbstream << std::setfill(' ') << std::setw(0) << " = ";
	lsbstream << std::setfill('0') << std::setw(2) << (int) value_at_address;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << lsbstream.str();

	out_string.append(spacing.str());
}

uint8_t IndexedIndirectAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	return memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
}

uint8_t IndexedIndirectAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	uint16_t full_address = GetValueAddress(cpu, memory);
	return memory.GetByte(full_address);
}

uint16_t IndexedIndirectAddressingStrategy::GetValueAddress(const CPU &cpu, const Memory &memory) const
{
	uint8_t lsb_location = GetMemoryByte(cpu, memory);
	lsb_location += cpu.GetRegisterX();
	uint8_t msb_location = lsb_location + (uint8_t) 1;
	uint16_t lsb = memory.GetByte((uint16_t) lsb_location);
	uint16_t msb = memory.GetByte((uint16_t) msb_location);
	uint16_t full_address = (msb << 8) | lsb;
	return full_address;
}

uint16_t IndexedIndirectAddressingStrategy::GetSetAddress(const CPU &cpu, const Memory &memory) const
{
	return GetValueAddress(cpu, memory);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IndirectIndexedAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	uint8_t lsb_location = GetMemoryByte(cpu, memory);
	std::ostringstream lsbstream;
	lsbstream << "($";
	lsbstream << std::uppercase << std::hex << std::right << std::setfill('0') << std::setw(2) << (int) lsb_location;
	lsbstream << std::setw(0) << std::setfill(' ') << "),Y";
	uint8_t msb_location = lsb_location + (uint8_t) 1;
	uint16_t lsb = memory.GetByte(lsb_location);
	uint16_t msb = memory.GetByte(msb_location);
	uint16_t full_address = (msb << 8) | lsb;

	lsbstream << std::setfill(' ') << std::setw(0) << " = ";
	lsbstream << std::setfill('0') << std::setw(4) << full_address;

	full_address += cpu.GetRegisterY();
	uint8_t value_at_address = memory.GetByte(full_address);
	lsbstream << " @ " << std::right << std::setfill('0') << std::setw(4) << (int) full_address;
	lsbstream << std::setfill(' ') << std::setw(0) << " = ";
	lsbstream << std::setfill('0') << std::setw(2) << (int) value_at_address;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << lsbstream.str();

	out_string.append(spacing.str());
}

uint8_t IndirectIndexedAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	return memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
}

uint8_t IndirectIndexedAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	uint16_t full_address = GetValueAddress(cpu, memory);
	return memory.GetByte(full_address);
}

uint16_t IndirectIndexedAddressingStrategy::GetValueAddress(const CPU &cpu, const Memory &memory) const
{
	uint8_t lsb_location = GetMemoryByte(cpu, memory);
	uint8_t msb_location = (uint8_t) lsb_location + (uint8_t) 1;
	uint16_t lsb = memory.GetByte(lsb_location);
	uint16_t msb = memory.GetByte(msb_location);
	uint16_t full_address = (msb << 8) | lsb;
	if (full_address / 256 != (full_address + cpu.GetRegisterY()) / 256)
	{
		m_addressing_cycles = 1;
	}
	else
	{
		m_addressing_cycles = 0;
	}
	full_address += cpu.GetRegisterY();
	return full_address;
}

uint16_t IndirectIndexedAddressingStrategy::GetSetAddress(const CPU &cpu, const Memory &memory) const
{
	return GetValueAddress(cpu, memory);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccumulatorAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	std::ostringstream os;
	os << std::setw(28) << std::left << "A";
	out_string.append(os.str());
}

uint8_t AccumulatorAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	assert(false);
	return 0;
}

uint8_t AccumulatorAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	return cpu.GetRegisterA();
}

void AccumulatorAddressingStrategy::SetMemoryByteValue(CPU &cpu, Memory &memory, uint8_t value) const
{
	cpu.SetRegisterA(value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ImmediateAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	std::ostringstream os;
	os << std::hex << "#$" << std::uppercase << std::setw(2) << std::setfill('0') << (int) GetMemoryByte(cpu, memory);
	os << std::setfill(' ') << std::setw(24) << " ";
	out_string.append(os.str());
}

uint8_t ImmediateAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	return memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
}

uint8_t ImmediateAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	return GetMemoryByte(cpu, memory);
}

uint16_t ImmediateAddressingStrategy::GetSetAddress(const CPU &cpu, const Memory &memory) const
{
	return GetMemoryByte(cpu, memory);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IndirectAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	uint16_t value = GetMemoryByte(cpu, memory);
	uint16_t full_address = GetMemoryByteValue(cpu, memory);
	std::ostringstream os;
	os << std::right << std::hex << "($" << std::uppercase << std::setw(4) << std::setfill('0') << value << ")";
	os << std::setfill(' ') << std::setw(0) << " = ";
	os << std::right << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << full_address;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << os.str();

	out_string.append(spacing.str());
}

uint16_t IndirectAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	return memory.GetShort(cpu.GetRegisterProgramCounterPlus(1));
}

uint16_t IndirectAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	uint16_t value = GetMemoryByte(cpu, memory);
	uint8_t lsb = memory.GetByte(value);
	uint16_t msb = 0x0000;
	if ((value & 0x00FF) == 0xFF)
	{
		msb = memory.GetByte(value & 0xFF00);
	}
	else
	{
		msb = memory.GetByte(value + (uint16_t) 1);
	}
	uint16_t full_address = (msb << 8) | lsb;
	return full_address;
}

uint16_t IndirectAddressingStrategy::GetSetAddress(const CPU &cpu, const Memory &memory) const
{
	return GetMemoryByte(cpu, memory);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ImpliedAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	std::ostringstream os;
	os << std::uppercase << std::setw(28) << std::left << " ";
	out_string.append(os.str());
}