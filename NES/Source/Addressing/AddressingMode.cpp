//
//
#include <assert.h>
#include "AddressingMode.h"
#include "../CPU.h"
#include "../Memory.h"
#include "../NESConsole.h"
#include <iomanip>

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AbsoluteAddressingImpl::ToString(std::string& out_string) const
{
	uint16_t destination = GetMemoryShort();
	uint8_t value = GetMemoryByteValue();
	std::ostringstream os;
	os << "$" << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << std::right << destination;
	os << " = " << std::setw(2) << std::setfill('0') << std::right << (int)value;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << os.str();
	out_string.append(spacing.str());
}

void AbsoluteAddressingImpl::SetMemoryByteValue(uint8_t value) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	const uint16_t address = GetSetAddress();
	memory.CPUWriteByte(address, value);
}

uint8_t AbsoluteAddressingImpl::GetMemoryByte() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadByte(cpu.GetRegisterProgramCounter() + 1);
}

uint16_t AbsoluteAddressingImpl::GetMemoryShort() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadShort(cpu.GetRegisterProgramCounter() + 1);
}

uint8_t AbsoluteAddressingImpl::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadByte(GetMemoryShort());
}

uint16_t AbsoluteAddressingImpl::GetMemoryShortValue() const
{
	assert(false);
	return 0;
}

uint16_t AbsoluteAddressingImpl::GetSetAddress() const
{
	return GetMemoryShort();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AbsoluteYAddressingImpl::ToString(std::string& out_string) const
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
	os << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << std::right << (int)value;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << os.str();
	out_string.append(spacing.str());
}

void AbsoluteYAddressingImpl::SetMemoryByteValue(uint8_t value) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	const uint16_t address = GetSetAddress();
	memory.CPUWriteByte(address, value);
}

uint8_t AbsoluteYAddressingImpl::GetMemoryByte() const
{
	assert(false);
	return 0;
}

uint16_t AbsoluteYAddressingImpl::GetMemoryShort() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint16_t memory_address = memory.CPUReadShort(cpu.GetRegisterProgramCounterPlus(1));
	memory_address += cpu.GetRegisterY();
	return memory_address;
}

uint8_t AbsoluteYAddressingImpl::GetMemoryByteValue() const
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

uint16_t AbsoluteYAddressingImpl::GetMemoryShortValue() const
{
	assert(false);
	return 0;
}

uint16_t AbsoluteYAddressingImpl::GetSetAddress() const
{
	return GetMemoryShort();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AbsoluteXAddressingImpl::ToString(std::string& out_string) const
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
	os << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << std::right << (int)value;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << os.str();
	out_string.append(spacing.str());
}

void AbsoluteXAddressingImpl::SetMemoryByteValue(uint8_t value) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	const uint16_t address = GetSetAddress();
	memory.CPUWriteByte(address, value);
}

uint8_t AbsoluteXAddressingImpl::GetMemoryByte() const
{
	assert(false);
	return 0;
}

uint16_t AbsoluteXAddressingImpl::GetMemoryShort() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint16_t memory_address = memory.CPUReadShort(cpu.GetRegisterProgramCounterPlus(1));
	memory_address += cpu.GetRegisterX();
	return memory_address;
}

uint8_t AbsoluteXAddressingImpl::GetMemoryByteValue() const
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

uint16_t AbsoluteXAddressingImpl::GetMemoryShortValue() const
{
	assert(false);
	return 0;
}

uint16_t AbsoluteXAddressingImpl::GetSetAddress() const
{
	return GetMemoryShort();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RelativeAddressingImpl::ToString(std::string& out_string) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint16_t destination = GetMemoryShortValue();
	std::ostringstream os;
	os << std::hex << "$" << std::uppercase << std::setw(27) << std::left << (int)destination;
	out_string.append(os.str());
}

void RelativeAddressingImpl::SetMemoryByteValue(uint8_t value) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	const uint16_t address = GetSetAddress();
	memory.CPUWriteByte(address, value);
}

uint8_t RelativeAddressingImpl::GetMemoryByte() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
}

uint16_t RelativeAddressingImpl::GetMemoryShort() const
{
	assert(false);
	return 0;
}

uint8_t RelativeAddressingImpl::GetMemoryByteValue() const
{
	assert(false);
	return 0;
}

uint16_t RelativeAddressingImpl::GetMemoryShortValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	// The offset is a signed byte, so it can jump a maximum of 127 bytes forward, or 128 bytes backward.
	int8_t data_byte = static_cast<int8_t>(GetMemoryByte());
	uint16_t destination = cpu.GetRegisterProgramCounterPlus(2) + (uint16_t)data_byte;
	return destination;
}

uint16_t RelativeAddressingImpl::GetSetAddress() const
{
	assert(false);
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void JMPAbsoluteAddressingImpl::ToString(std::string& out_string) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	std::ostringstream os;
	os << std::hex << "$" << std::uppercase << std::setw(27) << std::left << GetMemoryShort();
	out_string.append(os.str());
}

void JMPAbsoluteAddressingImpl::SetMemoryByteValue(uint8_t value) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	const uint16_t address = GetSetAddress();
	memory.CPUWriteByte(address, value);
}

uint8_t JMPAbsoluteAddressingImpl::GetMemoryByte() const
{
	assert(false);
	return 0;
}

uint16_t JMPAbsoluteAddressingImpl::GetMemoryShort() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadShort(cpu.GetRegisterProgramCounterPlus(1));
}

uint8_t JMPAbsoluteAddressingImpl::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadByte(GetMemoryShort());
}

uint16_t JMPAbsoluteAddressingImpl::GetMemoryShortValue() const
{
	assert(false);
	return 0;
}

uint16_t JMPAbsoluteAddressingImpl::GetSetAddress() const
{
	assert(false);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ZeroPageAddressingImpl::ToString(std::string& out_string) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t data_byte = GetMemoryByte();
	uint8_t data_value = GetMemoryByteValue();
	std::ostringstream os;
	os << "$" << std::uppercase << std::setw(2) << std::right << std::setfill('0') << std::hex << (int)data_byte
		<< " = " << std::setw(2) << (int)data_value;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << os.str();
	out_string.append(spacing.str());
}

void ZeroPageAddressingImpl::SetMemoryByteValue(uint8_t value) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	const uint16_t address = GetSetAddress();
	memory.CPUWriteByte(address, value);
}

uint8_t ZeroPageAddressingImpl::GetMemoryByte() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
}

uint16_t ZeroPageAddressingImpl::GetMemoryShort() const
{
	assert(false);
	return 0;
}

uint8_t ZeroPageAddressingImpl::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t zero_page_address = GetMemoryByte();
	uint8_t zero_page_value = memory.CPUReadByte((uint16_t)zero_page_address);
	return zero_page_value;
}

uint16_t ZeroPageAddressingImpl::GetMemoryShortValue() const
{
	assert(false);
	return 0;
}

uint16_t ZeroPageAddressingImpl::GetSetAddress() const
{
	return static_cast<uint16_t>(GetMemoryByte());
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ZeroPageXAddressingImpl::ToString(std::string& out_string) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t memory_address = memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
	uint8_t memory_offset = memory_address + cpu.GetRegisterX();
	uint8_t memory_value = memory.CPUReadByte((uint16_t)memory_offset);

	std::ostringstream os;
	os << "$" << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << std::right << (int)memory_address;
	os << std::setw(0) << ",X @ ";
	os << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << std::right << (int)memory_offset;
	os << std::setw(0) << " = ";
	os << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << std::right << (int)memory_value;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << os.str();
	out_string.append(spacing.str());
}

void ZeroPageXAddressingImpl::SetMemoryByteValue(uint8_t value) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	const uint16_t address = GetSetAddress();
	memory.CPUWriteByte(address, value);
}

uint8_t ZeroPageXAddressingImpl::GetMemoryByte() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t memory_address = memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
	uint8_t memory_offset = memory_address + cpu.GetRegisterX();
	return memory_offset;
}

uint16_t ZeroPageXAddressingImpl::GetMemoryShort() const
{
	assert(false);
	return 0;
}

uint8_t ZeroPageXAddressingImpl::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t memory_address = memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
	uint8_t memory_offset = memory_address + cpu.GetRegisterX();
	uint8_t memory_value = memory.CPUReadByte((uint16_t)memory_offset);
	return memory_value;
}

uint16_t ZeroPageXAddressingImpl::GetMemoryShortValue() const
{
	assert(false);
	return 0;
}

uint16_t ZeroPageXAddressingImpl::GetSetAddress() const
{
	return static_cast<uint16_t>(GetMemoryByte());
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ZeroPageYAddressingImpl::ToString(std::string& out_string) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t memory_address = memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
	uint8_t memory_offset = memory_address + cpu.GetRegisterY();
	uint8_t memory_value = memory.CPUReadByte((uint16_t)memory_offset);

	std::ostringstream os;
	os << "$" << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << std::right << (int)memory_address;
	os << std::setw(0) << ",Y @ ";
	os << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << std::right << (int)memory_offset;
	os << std::setw(0) << " = ";
	os << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << std::right << (int)memory_value;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << os.str();
	out_string.append(spacing.str());
}

void ZeroPageYAddressingImpl::SetMemoryByteValue(uint8_t value) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	const uint16_t address = GetSetAddress();
	memory.CPUWriteByte(address, value);
}

uint8_t ZeroPageYAddressingImpl::GetMemoryByte() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t memory_address = memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
	uint8_t memory_offset = memory_address + cpu.GetRegisterY();
	return memory_offset;
}

uint16_t ZeroPageYAddressingImpl::GetMemoryShort() const
{
	assert(false);
	return 0;
}

uint8_t ZeroPageYAddressingImpl::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t memory_address = memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
	uint8_t memory_offset = memory_address + cpu.GetRegisterY();
	uint8_t memory_value = memory.CPUReadByte((uint16_t)memory_offset);
	return memory_value;
}

uint16_t ZeroPageYAddressingImpl::GetMemoryShortValue() const
{
	assert(false);
	return 0;
}

uint16_t ZeroPageYAddressingImpl::GetSetAddress() const
{
	return static_cast<uint16_t>(GetMemoryByte());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IndexedIndirectAddressingImpl::ToString(std::string& out_string) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t lsb_location = GetMemoryByte();
	std::ostringstream lsbstream;
	lsbstream << "($";
	lsbstream << std::uppercase << std::hex << std::right << std::setfill('0') << std::setw(2) << (int)lsb_location;
	lsbstream << std::setw(0) << std::setfill(' ') << ",X)";
	lsb_location += cpu.GetRegisterX();
	uint8_t msb_location = lsb_location + (uint8_t)1;
	uint16_t lsb = memory.CPUReadByte(lsb_location);
	uint16_t msb = memory.CPUReadByte(msb_location);
	uint16_t full_address = (msb << 8) | lsb;
	uint8_t value_at_address = memory.CPUReadByte(full_address);

	lsbstream << " @ " << std::right << std::setfill('0') << std::setw(2) << (int)lsb_location;
	lsbstream << std::setfill(' ') << std::setw(0) << " = ";
	lsbstream << std::setfill('0') << std::setw(4) << full_address;
	lsbstream << std::setfill(' ') << std::setw(0) << " = ";
	lsbstream << std::setfill('0') << std::setw(2) << (int)value_at_address;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << lsbstream.str();

	out_string.append(spacing.str());
}

void IndexedIndirectAddressingImpl::SetMemoryByteValue(uint8_t value) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	const uint16_t address = GetSetAddress();
	memory.CPUWriteByte(address, value);
}

uint8_t IndexedIndirectAddressingImpl::GetMemoryByte() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
}

uint16_t IndexedIndirectAddressingImpl::GetMemoryShort() const
{
	assert(false);
	return 0;
}

uint8_t IndexedIndirectAddressingImpl::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint16_t full_address = GetSetAddress();
	return memory.CPUReadByte(full_address);
}

uint16_t IndexedIndirectAddressingImpl::GetMemoryShortValue() const
{
	assert(false);
	return 0;
}

uint16_t IndexedIndirectAddressingImpl::GetSetAddress() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t lsb_location = GetMemoryByte();
	lsb_location += cpu.GetRegisterX();
	uint8_t msb_location = lsb_location + (uint8_t)1;
	uint16_t lsb = memory.CPUReadByte((uint16_t)lsb_location);
	uint16_t msb = memory.CPUReadByte((uint16_t)msb_location);
	uint16_t full_address = (msb << 8) | lsb;
	return full_address;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IndirectIndexedAddressingImpl::ToString(std::string& out_string) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t lsb_location = GetMemoryByte();
	std::ostringstream lsbstream;
	lsbstream << "($";
	lsbstream << std::uppercase << std::hex << std::right << std::setfill('0') << std::setw(2) << (int)lsb_location;
	lsbstream << std::setw(0) << std::setfill(' ') << "),Y";
	uint8_t msb_location = lsb_location + (uint8_t)1;
	uint16_t lsb = memory.CPUReadByte(lsb_location);
	uint16_t msb = memory.CPUReadByte(msb_location);
	uint16_t full_address = (msb << 8) | lsb;

	lsbstream << std::setfill(' ') << std::setw(0) << " = ";
	lsbstream << std::setfill('0') << std::setw(4) << full_address;

	full_address += cpu.GetRegisterY();
	uint8_t value_at_address = memory.CPUReadByte(full_address);
	lsbstream << " @ " << std::right << std::setfill('0') << std::setw(4) << (int)full_address;
	lsbstream << std::setfill(' ') << std::setw(0) << " = ";
	lsbstream << std::setfill('0') << std::setw(2) << (int)value_at_address;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << lsbstream.str();

	out_string.append(spacing.str());
}

void IndirectIndexedAddressingImpl::SetMemoryByteValue(uint8_t value) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	const uint16_t address = GetSetAddress();
	memory.CPUWriteByte(address, value);
}

uint8_t IndirectIndexedAddressingImpl::GetMemoryByte() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
}

uint8_t IndirectIndexedAddressingImpl::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint16_t full_address = GetSetAddress();
	return memory.CPUReadByte(full_address);
}

uint16_t IndirectIndexedAddressingImpl::GetMemoryShort() const
{
	assert(false);
	return 0;
}

uint16_t IndirectIndexedAddressingImpl::GetMemoryShortValue() const
{
	assert(false);
	return 0;
}

uint16_t IndirectIndexedAddressingImpl::GetSetAddress() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint8_t lsb_location = GetMemoryByte();
	uint8_t msb_location = (uint8_t)lsb_location + (uint8_t)1;
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccumulatorAddressingImpl::ToString(std::string& out_string) const
{
	std::ostringstream os;
	os << std::setw(28) << std::left << "A";
	out_string.append(os.str());
}

uint8_t AccumulatorAddressingImpl::GetMemoryByte() const
{
	assert(false);
	return 0;
}

uint16_t AccumulatorAddressingImpl::GetSetAddress() const
{
	assert(false);
	return 0;
}

uint8_t AccumulatorAddressingImpl::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return cpu.GetRegisterA();
}

uint16_t AccumulatorAddressingImpl::GetMemoryShortValue() const
{
	assert(false);
	return 0;
}

uint16_t AccumulatorAddressingImpl::GetMemoryShort() const
{
	assert(false);
	return 0;
}

void AccumulatorAddressingImpl::SetMemoryByteValue(uint8_t value) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	cpu.SetRegisterA(value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ImmediateAddressingImpl::ToString(std::string& out_string) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	std::ostringstream os;
	os << std::hex << "#$" << std::uppercase << std::setw(2) << std::setfill('0') << (int)GetMemoryByte();
	os << std::setfill(' ') << std::setw(24) << " ";
	out_string.append(os.str());
}

void ImmediateAddressingImpl::SetMemoryByteValue(uint8_t value) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	const uint16_t address = GetSetAddress();
	memory.CPUWriteByte(address, value);
}

uint8_t ImmediateAddressingImpl::GetMemoryByte() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(1));
}

uint8_t ImmediateAddressingImpl::GetMemoryByteValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return GetMemoryByte();
}

uint16_t ImmediateAddressingImpl::GetMemoryShortValue() const
{
	assert(false);
	return 0;
}

uint16_t ImmediateAddressingImpl::GetMemoryShort() const
{
	assert(false);
	return 0;
}

uint16_t ImmediateAddressingImpl::GetSetAddress() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return GetMemoryShort();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IndirectAddressingImpl::ToString(std::string& out_string) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint16_t value = GetMemoryShort();
	uint16_t full_address = GetMemoryShortValue();
	std::ostringstream os;
	os << std::right << std::hex << "($" << std::uppercase << std::setw(4) << std::setfill('0') << value << ")";
	os << std::setfill(' ') << std::setw(0) << " = ";
	os << std::right << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << full_address;

	std::ostringstream spacing;
	spacing << std::left << std::setw(28) << os.str();

	out_string.append(spacing.str());
}
void IndirectAddressingImpl::SetMemoryByteValue(uint8_t value) const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	const uint16_t address = GetSetAddress();
	memory.CPUWriteByte(address, value);
}

uint8_t IndirectAddressingImpl::GetMemoryByte() const
{
	assert(false);
	return 0;
}

uint16_t IndirectAddressingImpl::GetMemoryShort() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	return memory.CPUReadShort(cpu.GetRegisterProgramCounterPlus(1));
}

uint8_t IndirectAddressingImpl::GetMemoryByteValue() const
{
	assert(false);
	return 0;
}

uint16_t IndirectAddressingImpl::GetMemoryShortValue() const
{
	CPU& cpu = NESConsole::GetInstance()->GetCPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	uint16_t value = GetMemoryShort();
	uint8_t lsb = memory.CPUReadByte(value);
	uint16_t msb = 0x0000;
	if ((value & 0x00FF) == 0xFF)
	{
		msb = memory.CPUReadByte(value & 0xFF00);
	}
	else
	{
		msb = memory.CPUReadByte(value + (uint16_t)1);
	}
	uint16_t full_address = (msb << 8) | lsb;
	return full_address;
}

uint16_t IndirectAddressingImpl::GetSetAddress() const
{
	return GetMemoryShort();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ImpliedAddressingImpl::ToString(std::string& out_string) const
{
	std::ostringstream os;
	os << std::uppercase << std::setw(28) << std::left << " ";
	out_string.append(os.str());
}