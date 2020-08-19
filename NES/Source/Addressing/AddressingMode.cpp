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
	unsigned short destination = GetMemoryByte(cpu, memory);
	unsigned char value = GetMemoryByteValue(cpu, memory);
	std::ostringstream os;
	os << "$" << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << std::right << destination;
	os << " = " << std::setw(2) << std::setfill('0') << std::right << (int) value;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << os.str();
	out_string.append(spacing.str());
}

unsigned short AbsoluteAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	return memory.GetShort(cpu.GetRegisterProgramCounter() + 1);
}

unsigned char AbsoluteAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	return memory.GetByte(GetMemoryByte(cpu, memory));
}

unsigned short AbsoluteAddressingStrategy::GetSetAddress(const CPU &cpu, const Memory &memory) const
{
	return GetMemoryByte(cpu, memory);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AbsoluteYAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	unsigned short memory_address = memory.GetShort(cpu.GetRegisterProgramCounterPlus(1));
	unsigned short memory_offset = memory_address + cpu.GetRegisterY();
	unsigned char value = memory.GetByte(memory_offset);

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

unsigned short AbsoluteYAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	unsigned short memory_address = memory.GetShort(cpu.GetRegisterProgramCounterPlus(1));
	memory_address += cpu.GetRegisterY();
	return memory_address;
}

unsigned char AbsoluteYAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	unsigned short memory_address = memory.GetShort(cpu.GetRegisterProgramCounterPlus(1));
	unsigned short memory_offset = memory_address + cpu.GetRegisterY();
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

unsigned short AbsoluteYAddressingStrategy::GetSetAddress(const CPU &cpu, const Memory &memory) const
{
	return GetMemoryByte(cpu, memory);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AbsoluteXAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	unsigned short memory_address = memory.GetShort(cpu.GetRegisterProgramCounterPlus(1));
	unsigned short memory_offset = memory_address + cpu.GetRegisterX();
	unsigned char value = memory.GetByte(memory_offset);

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

unsigned short AbsoluteXAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	unsigned short memory_address = memory.GetShort(cpu.GetRegisterProgramCounterPlus(1));
	memory_address += cpu.GetRegisterX();
	return memory_address;
}

unsigned char AbsoluteXAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	unsigned short memory_address = memory.GetShort(cpu.GetRegisterProgramCounterPlus(1));
	unsigned short memory_offset = memory_address + cpu.GetRegisterX();
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

unsigned short AbsoluteXAddressingStrategy::GetSetAddress(const CPU &cpu, const Memory &memory) const
{
	return GetMemoryByte(cpu, memory);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RelativeAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	unsigned short destination = GetMemoryByteValue(cpu, memory);
	std::ostringstream os;
	os << std::hex << "$" << std::uppercase << std::setw(27) << std::left << (int) destination;
	out_string.append(os.str());
}

unsigned char RelativeAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	return memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
}

unsigned short RelativeAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	unsigned char data_byte = GetMemoryByte(cpu, memory);
	unsigned short destination = cpu.GetRegisterProgramCounterPlus(2) + (unsigned short) data_byte;
	return destination;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void JMPAbsoluteAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	std::ostringstream os;
	os << std::hex << "$" << std::uppercase << std::setw(27) << std::left << GetMemoryByte(cpu, memory);
	out_string.append(os.str());
}

unsigned short JMPAbsoluteAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	return memory.GetShort(cpu.GetRegisterProgramCounterPlus(1));
}

unsigned char JMPAbsoluteAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	return memory.GetByte(GetMemoryByte(cpu, memory));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ZeroPageAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	unsigned char data_byte = GetMemoryByte(cpu, memory);
	unsigned char data_value = GetMemoryByteValue(cpu, memory);
	std::ostringstream os;
	os << "$" << std::uppercase << std::setw(2) << std::right << std::setfill('0') << std::hex << (int) data_byte
	   << " = " << std::setw(2) << (int) data_value;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << os.str();
	out_string.append(spacing.str());
}

unsigned char ZeroPageAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	return memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
}

unsigned char ZeroPageAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	unsigned char zero_page_address = GetMemoryByte(cpu, memory);
	unsigned char zero_page_value = memory.GetByte((unsigned short) zero_page_address);
	return zero_page_value;
}

unsigned short ZeroPageAddressingStrategy::GetSetAddress(const CPU &cpu, const Memory &memory) const
{
	return GetMemoryByte(cpu, memory);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ZeroPageXAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	unsigned char memory_address = memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
	unsigned char memory_offset = memory_address + cpu.GetRegisterX();
	unsigned char memory_value = memory.GetByte((unsigned short) memory_offset);

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

unsigned char ZeroPageXAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	unsigned char memory_address = memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
	unsigned char memory_offset = memory_address + cpu.GetRegisterX();
	return memory_offset;
}

unsigned char ZeroPageXAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	unsigned char memory_address = memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
	unsigned char memory_offset = memory_address + cpu.GetRegisterX();
	unsigned char memory_value = memory.GetByte((unsigned short) memory_offset);
	return memory_value;
}

unsigned short ZeroPageXAddressingStrategy::GetSetAddress(const CPU &cpu, const Memory &memory) const
{
	return GetMemoryByte(cpu, memory);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ZeroPageYAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	unsigned char memory_address = memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
	unsigned char memory_offset = memory_address + cpu.GetRegisterY();
	unsigned char memory_value = memory.GetByte((unsigned short) memory_offset);

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

unsigned char ZeroPageYAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	unsigned char memory_address = memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
	unsigned char memory_offset = memory_address + cpu.GetRegisterY();
	return memory_offset;
}

unsigned char ZeroPageYAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	unsigned char memory_address = memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
	unsigned char memory_offset = memory_address + cpu.GetRegisterY();
	unsigned char memory_value = memory.GetByte((unsigned short) memory_offset);
	return memory_value;
}

unsigned short ZeroPageYAddressingStrategy::GetSetAddress(const CPU &cpu, const Memory &memory) const
{
	return GetMemoryByte(cpu, memory);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IndexedIndirectAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	unsigned char lsb_location = GetMemoryByte(cpu, memory);
	std::ostringstream lsbstream;
	lsbstream << "($";
	lsbstream << std::uppercase << std::hex << std::right << std::setfill('0') << std::setw(2) << (int) lsb_location;
	lsbstream << std::setw(0) << std::setfill(' ') << ",X)";
	lsb_location += cpu.GetRegisterX();
	unsigned char msb_location = lsb_location + (unsigned char) 1;
	unsigned short lsb = memory.GetByte(lsb_location);
	unsigned short msb = memory.GetByte(msb_location);
	unsigned short full_address = (msb << 8) | lsb;
	unsigned char value_at_address = memory.GetByte(full_address);

	lsbstream << " @ " << std::right << std::setfill('0') << std::setw(2) << (int) lsb_location;
	lsbstream << std::setfill(' ') << std::setw(0) << " = ";
	lsbstream << std::setfill('0') << std::setw(4) << full_address;
	lsbstream << std::setfill(' ') << std::setw(0) << " = ";
	lsbstream << std::setfill('0') << std::setw(2) << (int) value_at_address;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << lsbstream.str();

	out_string.append(spacing.str());
}

unsigned char IndexedIndirectAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	return memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
}

unsigned char IndexedIndirectAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	unsigned short full_address = GetValueAddress(cpu, memory);
	return memory.GetByte(full_address);
}

unsigned short IndexedIndirectAddressingStrategy::GetValueAddress(const CPU &cpu, const Memory &memory) const
{
	unsigned char lsb_location = GetMemoryByte(cpu, memory);
	lsb_location += cpu.GetRegisterX();
	unsigned char msb_location = lsb_location + (unsigned char) 1;
	unsigned short lsb = memory.GetByte((unsigned short) lsb_location);
	unsigned short msb = memory.GetByte((unsigned short) msb_location);
	unsigned short full_address = (msb << 8) | lsb;
	return full_address;
}

unsigned short IndexedIndirectAddressingStrategy::GetSetAddress(const CPU &cpu, const Memory &memory) const
{
	return GetValueAddress(cpu, memory);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IndirectIndexedAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	unsigned char lsb_location = GetMemoryByte(cpu, memory);
	std::ostringstream lsbstream;
	lsbstream << "($";
	lsbstream << std::uppercase << std::hex << std::right << std::setfill('0') << std::setw(2) << (int) lsb_location;
	lsbstream << std::setw(0) << std::setfill(' ') << "),Y";
	unsigned char msb_location = lsb_location + (unsigned char) 1;
	unsigned short lsb = memory.GetByte(lsb_location);
	unsigned short msb = memory.GetByte(msb_location);
	unsigned short full_address = (msb << 8) | lsb;

	lsbstream << std::setfill(' ') << std::setw(0) << " = ";
	lsbstream << std::setfill('0') << std::setw(4) << full_address;

	full_address += cpu.GetRegisterY();
	unsigned char value_at_address = memory.GetByte(full_address);
	lsbstream << " @ " << std::right << std::setfill('0') << std::setw(4) << (int) full_address;
	lsbstream << std::setfill(' ') << std::setw(0) << " = ";
	lsbstream << std::setfill('0') << std::setw(2) << (int) value_at_address;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << lsbstream.str();

	out_string.append(spacing.str());
}

unsigned char IndirectIndexedAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	return memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
}

unsigned char IndirectIndexedAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	unsigned short full_address = GetValueAddress(cpu, memory);
	return memory.GetByte(full_address);
}

unsigned short IndirectIndexedAddressingStrategy::GetValueAddress(const CPU &cpu, const Memory &memory) const
{
	unsigned char lsb_location = GetMemoryByte(cpu, memory);
	unsigned char msb_location = (unsigned char) lsb_location + (unsigned char) 1;
	unsigned short lsb = memory.GetByte(lsb_location);
	unsigned short msb = memory.GetByte(msb_location);
	unsigned short full_address = (msb << 8) | lsb;
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

unsigned short IndirectIndexedAddressingStrategy::GetSetAddress(const CPU &cpu, const Memory &memory) const
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

unsigned char AccumulatorAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	assert(false);
	return 0;
}

unsigned char AccumulatorAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	return cpu.GetRegisterA();
}

void AccumulatorAddressingStrategy::SetMemoryByteValue(CPU &cpu, Memory &memory, unsigned char value) const
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

unsigned char ImmediateAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	return memory.GetByte(cpu.GetRegisterProgramCounterPlus(1));
}

unsigned char ImmediateAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	return GetMemoryByte(cpu, memory);
}

unsigned short ImmediateAddressingStrategy::GetSetAddress(const CPU &cpu, const Memory &memory) const
{
	return GetMemoryByte(cpu, memory);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IndirectAddressingStrategy::ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
{
	unsigned short value = GetMemoryByte(cpu, memory);
	unsigned short full_address = GetMemoryByteValue(cpu, memory);
	std::ostringstream os;
	os << std::right << std::hex << "($" << std::uppercase << std::setw(4) << std::setfill('0') << value << ")";
	os << std::setfill(' ') << std::setw(0) << " = ";
	os << std::right << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << full_address;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << os.str();

	out_string.append(spacing.str());
}

unsigned short IndirectAddressingStrategy::GetMemoryByte(const CPU &cpu, const Memory &memory) const
{
	return memory.GetShort(cpu.GetRegisterProgramCounterPlus(1));
}

unsigned short IndirectAddressingStrategy::GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
{
	unsigned short value = GetMemoryByte(cpu, memory);
	unsigned char lsb = memory.GetByte(value);
	unsigned short msb = 0x0000;
	if ((value & 0x00FF) == 0xFF)
	{
		msb = memory.GetByte(value & 0xFF00);
	}
	else
	{
		msb = memory.GetByte(value + (unsigned short) 1);
	}
	unsigned short full_address = (msb << 8) | lsb;
	return full_address;
}

unsigned short IndirectAddressingStrategy::GetSetAddress(const CPU &cpu, const Memory &memory) const
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