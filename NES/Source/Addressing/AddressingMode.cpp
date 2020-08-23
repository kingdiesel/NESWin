//
//
#include <assert.h>
#include "AddressingMode.h"
#include "../CPU.h"
#include "../Memory.h"
#include "../NESConsole.h"
#include <iomanip>

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AbsoluteAddressingStrategy::ToString(std::string &out_string) const
{
	uint16_t destination = GetMemoryByte();
	uint8_t value = GetMemoryByteValue();
	std::ostringstream os;
	os << "$" << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << std::right << destination;
	os << " = " << std::setw(2) << std::setfill('0') << std::right << (int) value;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << os.str();
	out_string.append(spacing.str());
}

uint16_t AbsoluteAddressingStrategy::GetMemoryByte() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadShort(cpu.GetRegisterProgramCounter() + 1);
}

uint8_t AbsoluteAddressingStrategy::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadByte(GetMemoryByte());
}

uint16_t AbsoluteAddressingStrategy::GetSetAddress() const
{
	return GetMemoryByte();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AbsoluteYAddressingStrategy::ToString(std::string &out_string) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint16_t memory_address = memory.CPUReadShort(cpu.GetRegisterProgramCounterPlus(1));
	uint16_t memory_offset = memory_address + cpu.GetRegisterY();
	uint8_t value = memory.CPUReadByte(memory_offset);

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

uint16_t AbsoluteYAddressingStrategy::GetMemoryByte() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint16_t memory_address = memory.CPUReadShort(cpu.GetRegisterProgramCounterPlus(1));
	memory_address += cpu.GetRegisterY();
	return memory_address;
}

uint8_t AbsoluteYAddressingStrategy::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint16_t memory_address = memory.CPUReadShort(cpu.GetRegisterProgramCounterPlus(1));
	uint16_t memory_offset = memory_address + cpu.GetRegisterY();
	if (memory_address / 256 != memory_offset / 256)
	{
		m_addressing_cycles = 1;
	}
	else
	{
		m_addressing_cycles = 0;
	}
	return memory.CPUReadByte(memory_offset);
}

uint16_t AbsoluteYAddressingStrategy::GetSetAddress() const
{
	return GetMemoryByte();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AbsoluteXAddressingStrategy::ToString(std::string &out_string) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint16_t memory_address = memory.CPUReadShort(cpu.GetRegisterProgramCounterPlus(1));
	uint16_t memory_offset = memory_address + cpu.GetRegisterX();
	uint8_t value = memory.CPUReadByte(memory_offset);

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

uint16_t AbsoluteXAddressingStrategy::GetMemoryByte() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint16_t memory_address = memory.CPUReadShort(cpu.GetRegisterProgramCounterPlus(1));
	memory_address += cpu.GetRegisterX();
	return memory_address;
}

uint8_t AbsoluteXAddressingStrategy::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint16_t memory_address = memory.CPUReadShort(cpu.GetRegisterProgramCounterPlus(1));
	uint16_t memory_offset = memory_address + cpu.GetRegisterX();
	if (memory_address / 256 != memory_offset / 256)
	{
		m_addressing_cycles = 1;
	}
	else
	{
		m_addressing_cycles = 0;
	}
	return memory.CPUReadByte(memory_offset);
}

uint16_t AbsoluteXAddressingStrategy::GetSetAddress() const
{
	return GetMemoryByte();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RelativeAddressingStrategy::ToString(std::string &out_string) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint16_t destination = GetMemoryByteValue();
	std::ostringstream os;
	os << std::hex << "$" << std::uppercase << std::setw(27) << std::left << (int) destination;
	out_string.append(os.str());
}

uint8_t RelativeAddressingStrategy::GetMemoryByte() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
}

uint16_t RelativeAddressingStrategy::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t data_byte = GetMemoryByte();
	uint16_t destination = cpu.GetRegisterProgramCounterPlus(2) + (uint16_t) data_byte;
	return destination;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void JMPAbsoluteAddressingStrategy::ToString(std::string &out_string) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	std::ostringstream os;
	os << std::hex << "$" << std::uppercase << std::setw(27) << std::left << GetMemoryByte();
	out_string.append(os.str());
}

uint16_t JMPAbsoluteAddressingStrategy::GetMemoryByte() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadShort(cpu.GetRegisterProgramCounterPlus(1));
}

uint8_t JMPAbsoluteAddressingStrategy::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadByte(GetMemoryByte());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ZeroPageAddressingStrategy::ToString(std::string &out_string) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t data_byte = GetMemoryByte();
	uint8_t data_value = GetMemoryByteValue();
	std::ostringstream os;
	os << "$" << std::uppercase << std::setw(2) << std::right << std::setfill('0') << std::hex << (int) data_byte
	   << " = " << std::setw(2) << (int) data_value;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << os.str();
	out_string.append(spacing.str());
}

uint8_t ZeroPageAddressingStrategy::GetMemoryByte() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
}

uint8_t ZeroPageAddressingStrategy::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t zero_page_address = GetMemoryByte();
	uint8_t zero_page_value = memory.CPUReadByte((uint16_t) zero_page_address);
	return zero_page_value;
}

uint16_t ZeroPageAddressingStrategy::GetSetAddress() const
{
	return GetMemoryByte();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ZeroPageXAddressingStrategy::ToString(std::string &out_string) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t memory_address = memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
	uint8_t memory_offset = memory_address + cpu.GetRegisterX();
	uint8_t memory_value = memory.CPUReadByte((uint16_t) memory_offset);

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

uint8_t ZeroPageXAddressingStrategy::GetMemoryByte() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t memory_address = memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
	uint8_t memory_offset = memory_address + cpu.GetRegisterX();
	return memory_offset;
}

uint8_t ZeroPageXAddressingStrategy::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t memory_address = memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
	uint8_t memory_offset = memory_address + cpu.GetRegisterX();
	uint8_t memory_value = memory.CPUReadByte((uint16_t) memory_offset);
	return memory_value;
}

uint16_t ZeroPageXAddressingStrategy::GetSetAddress() const
{
	return GetMemoryByte();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ZeroPageYAddressingStrategy::ToString(std::string &out_string) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t memory_address = memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
	uint8_t memory_offset = memory_address + cpu.GetRegisterY();
	uint8_t memory_value = memory.CPUReadByte((uint16_t) memory_offset);

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

uint8_t ZeroPageYAddressingStrategy::GetMemoryByte() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t memory_address = memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
	uint8_t memory_offset = memory_address + cpu.GetRegisterY();
	return memory_offset;
}

uint8_t ZeroPageYAddressingStrategy::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t memory_address = memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
	uint8_t memory_offset = memory_address + cpu.GetRegisterY();
	uint8_t memory_value = memory.CPUReadByte((uint16_t) memory_offset);
	return memory_value;
}

uint16_t ZeroPageYAddressingStrategy::GetSetAddress() const
{
	return GetMemoryByte();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IndexedIndirectAddressingStrategy::ToString(std::string &out_string) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t lsb_location = GetMemoryByte();
	std::ostringstream lsbstream;
	lsbstream << "($";
	lsbstream << std::uppercase << std::hex << std::right << std::setfill('0') << std::setw(2) << (int) lsb_location;
	lsbstream << std::setw(0) << std::setfill(' ') << ",X)";
	lsb_location += cpu.GetRegisterX();
	uint8_t msb_location = lsb_location + (uint8_t) 1;
	uint16_t lsb = memory.CPUReadByte(lsb_location);
	uint16_t msb = memory.CPUReadByte(msb_location);
	uint16_t full_address = (msb << 8) | lsb;
	uint8_t value_at_address = memory.CPUReadByte(full_address);

	lsbstream << " @ " << std::right << std::setfill('0') << std::setw(2) << (int) lsb_location;
	lsbstream << std::setfill(' ') << std::setw(0) << " = ";
	lsbstream << std::setfill('0') << std::setw(4) << full_address;
	lsbstream << std::setfill(' ') << std::setw(0) << " = ";
	lsbstream << std::setfill('0') << std::setw(2) << (int) value_at_address;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << lsbstream.str();

	out_string.append(spacing.str());
}

uint8_t IndexedIndirectAddressingStrategy::GetMemoryByte() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
}

uint8_t IndexedIndirectAddressingStrategy::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint16_t full_address = GetValueAddress();
	return memory.CPUReadByte(full_address);
}

uint16_t IndexedIndirectAddressingStrategy::GetValueAddress() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t lsb_location = GetMemoryByte();
	lsb_location += cpu.GetRegisterX();
	uint8_t msb_location = lsb_location + (uint8_t) 1;
	uint16_t lsb = memory.CPUReadByte((uint16_t) lsb_location);
	uint16_t msb = memory.CPUReadByte((uint16_t) msb_location);
	uint16_t full_address = (msb << 8) | lsb;
	return full_address;
}

uint16_t IndexedIndirectAddressingStrategy::GetSetAddress() const
{
	return GetValueAddress();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IndirectIndexedAddressingStrategy::ToString(std::string &out_string) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t lsb_location = GetMemoryByte();
	std::ostringstream lsbstream;
	lsbstream << "($";
	lsbstream << std::uppercase << std::hex << std::right << std::setfill('0') << std::setw(2) << (int) lsb_location;
	lsbstream << std::setw(0) << std::setfill(' ') << "),Y";
	uint8_t msb_location = lsb_location + (uint8_t) 1;
	uint16_t lsb = memory.CPUReadByte(lsb_location);
	uint16_t msb = memory.CPUReadByte(msb_location);
	uint16_t full_address = (msb << 8) | lsb;

	lsbstream << std::setfill(' ') << std::setw(0) << " = ";
	lsbstream << std::setfill('0') << std::setw(4) << full_address;

	full_address += cpu.GetRegisterY();
	uint8_t value_at_address = memory.CPUReadByte(full_address);
	lsbstream << " @ " << std::right << std::setfill('0') << std::setw(4) << (int) full_address;
	lsbstream << std::setfill(' ') << std::setw(0) << " = ";
	lsbstream << std::setfill('0') << std::setw(2) << (int) value_at_address;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << lsbstream.str();

	out_string.append(spacing.str());
}

uint8_t IndirectIndexedAddressingStrategy::GetMemoryByte() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
}

uint8_t IndirectIndexedAddressingStrategy::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint16_t full_address = GetValueAddress();
	return memory.CPUReadByte(full_address);
}

uint16_t IndirectIndexedAddressingStrategy::GetValueAddress() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t lsb_location = GetMemoryByte();
	uint8_t msb_location = (uint8_t) lsb_location + (uint8_t) 1;
	uint16_t lsb = memory.CPUReadByte(lsb_location);
	uint16_t msb = memory.CPUReadByte(msb_location);
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

uint16_t IndirectIndexedAddressingStrategy::GetSetAddress() const
{
	return GetValueAddress();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccumulatorAddressingStrategy::ToString(std::string &out_string) const
{
	std::ostringstream os;
	os << std::setw(28) << std::left << "A";
	out_string.append(os.str());
}

uint8_t AccumulatorAddressingStrategy::GetMemoryByte() const
{
	assert(false);
	return 0;
}

uint8_t AccumulatorAddressingStrategy::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return cpu.GetRegisterA();
}

void AccumulatorAddressingStrategy::SetMemoryByteValue(uint8_t value) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	cpu.SetRegisterA(value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ImmediateAddressingStrategy::ToString(std::string &out_string) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	std::ostringstream os;
	os << std::hex << "#$" << std::uppercase << std::setw(2) << std::setfill('0') << (int) GetMemoryByte();
	os << std::setfill(' ') << std::setw(24) << " ";
	out_string.append(os.str());
}

uint8_t ImmediateAddressingStrategy::GetMemoryByte() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
}

uint8_t ImmediateAddressingStrategy::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return GetMemoryByte();
}

uint16_t ImmediateAddressingStrategy::GetSetAddress() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return GetMemoryByte();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IndirectAddressingStrategy::ToString(std::string &out_string) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint16_t value = GetMemoryByte();
	uint16_t full_address = GetMemoryByteValue();
	std::ostringstream os;
	os << std::right << std::hex << "($" << std::uppercase << std::setw(4) << std::setfill('0') << value << ")";
	os << std::setfill(' ') << std::setw(0) << " = ";
	os << std::right << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << full_address;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << os.str();

	out_string.append(spacing.str());
}

uint16_t IndirectAddressingStrategy::GetMemoryByte() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadShort(cpu.GetRegisterProgramCounterPlus(1));
}

uint16_t IndirectAddressingStrategy::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint16_t value = GetMemoryByte();
	uint8_t lsb = memory.CPUReadByte(value);
	uint16_t msb = 0x0000;
	if ((value & 0x00FF) == 0xFF)
	{
		msb = memory.CPUReadByte(value & 0xFF00);
	}
	else
	{
		msb = memory.CPUReadByte(value + (uint16_t) 1);
	}
	uint16_t full_address = (msb << 8) | lsb;
	return full_address;
}

uint16_t IndirectAddressingStrategy::GetSetAddress() const
{
	return GetMemoryByte();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ImpliedAddressingStrategy::ToString(std::string &out_string) const
{
	std::ostringstream os;
	os << std::uppercase << std::setw(28) << std::left << " ";
	out_string.append(os.str());
}