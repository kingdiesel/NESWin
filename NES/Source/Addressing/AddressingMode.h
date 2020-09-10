#pragma once
#include <string>
#include <sstream>
#include "../Memory.h"
#include "../CPU.h"
#include "../NESConsole.h"

class CPU;
class Memory;

template<typename _derived_addressing, int _operand_length>
class BaseAddressingStrategy
{
public:
	BaseAddressingStrategy();
	void SetMemoryByteValue(uint8_t value)
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		uint16_t address = static_cast<_derived_addressing *>(this)->GetSetAddress();
		memory.CPUWriteByte(address, value);
	}

	int GetAddressingCycles() const
	{
		return m_addressing_cycles;
	}

	int GetOperandLength() const
	{
		return _operand_length;
	}

protected:
	mutable int m_addressing_cycles = 0;
	class NESConsole* m_console = nullptr;
};

class AbsoluteAddressingStrategy : public BaseAddressingStrategy<AbsoluteAddressingStrategy, 2>
{
public:
	void ToString(std::string &out_string) const;

	uint16_t GetMemoryByte() const;

	uint8_t GetMemoryByteValue() const;

	uint16_t GetSetAddress() const;
};

class RelativeAddressingStrategy : public BaseAddressingStrategy<RelativeAddressingStrategy, 1>
{
public:
	void ToString(std::string &out_string) const;

	uint8_t GetMemoryByte() const;

	uint16_t GetMemoryByteValue() const;
};

class AbsoluteYAddressingStrategy : public BaseAddressingStrategy<AbsoluteYAddressingStrategy, 2>
{
public:
	void ToString(std::string &out_string) const;

	uint16_t GetMemoryByte() const;

	uint8_t GetMemoryByteValue() const;

	uint16_t GetSetAddress() const;
};

class AbsoluteXAddressingStrategy : public BaseAddressingStrategy<AbsoluteXAddressingStrategy, 2>
{
public:
	void ToString(std::string &out_string) const;

	uint16_t GetMemoryByte() const;

	uint8_t GetMemoryByteValue() const;

	uint16_t GetSetAddress() const;
};

class ZeroPageAddressingStrategy : public BaseAddressingStrategy<ZeroPageAddressingStrategy, 1>
{
public:
	void ToString(std::string &out_string) const;

	uint8_t GetMemoryByte() const;

	uint8_t GetMemoryByteValue() const;

	uint16_t GetSetAddress() const;
};

class ZeroPageXAddressingStrategy : public BaseAddressingStrategy<ZeroPageXAddressingStrategy, 1>
{
public:
	void ToString(std::string &out_string) const;

	uint8_t GetMemoryByte() const;

	uint8_t GetMemoryByteValue() const;

	uint16_t GetSetAddress() const;
};

class ZeroPageYAddressingStrategy : public BaseAddressingStrategy<ZeroPageYAddressingStrategy, 1>
{
public:
	void ToString(std::string &out_string) const;

	uint8_t GetMemoryByte() const;

	uint8_t GetMemoryByteValue() const;

	uint16_t GetSetAddress() const;
};

class IndexedIndirectAddressingStrategy : public BaseAddressingStrategy<IndexedIndirectAddressingStrategy, 1>
{
public:
	void ToString(std::string &out_string) const;

	uint8_t GetMemoryByte() const;

	uint8_t GetMemoryByteValue() const;

	uint16_t GetValueAddress() const;

	uint16_t GetSetAddress() const;
};

class IndirectIndexedAddressingStrategy : public BaseAddressingStrategy<IndirectIndexedAddressingStrategy, 1>
{
public:
	void ToString(std::string &out_string) const;

	uint8_t GetMemoryByte() const;

	uint8_t GetMemoryByteValue() const;

	uint16_t GetValueAddress() const;

	uint16_t GetSetAddress() const;
};

class JMPAbsoluteAddressingStrategy : public BaseAddressingStrategy<JMPAbsoluteAddressingStrategy, 2>
{
public:
	void ToString(std::string &out_string) const;

	uint16_t GetMemoryByte() const;

	uint8_t GetMemoryByteValue() const;
};

class AccumulatorAddressingStrategy
{
public:
	AccumulatorAddressingStrategy()
	{
		m_console = NESConsole::GetInstance().get();
	}
	void ToString(std::string &out_string) const;

	uint8_t GetMemoryByte() const;

	uint8_t GetMemoryByteValue() const;

	void SetMemoryByteValue(uint8_t value) const;

	int GetAddressingCycles() const
	{
		return m_addressing_cycles;
	}

	int GetOperandLength() const
	{
		return 0;
	}

private:
	mutable int m_addressing_cycles = 0;
	NESConsole* m_console = nullptr;
};

class ImmediateAddressingStrategy : public BaseAddressingStrategy<ImmediateAddressingStrategy, 1>
{
public:
	void ToString(std::string &out_string) const;

	uint8_t GetMemoryByte() const;

	uint8_t GetMemoryByteValue() const;

	uint16_t GetSetAddress() const;
};

class IndirectAddressingStrategy : public BaseAddressingStrategy<IndirectAddressingStrategy, 2>
{
public:
	void ToString(std::string &out_string) const;

	uint16_t GetMemoryByte() const;

	uint16_t GetMemoryByteValue() const;

	uint16_t GetSetAddress() const;
};

class ImpliedAddressingStrategy : public BaseAddressingStrategy<ImpliedAddressingStrategy, 0>
{
public:
	void ToString(std::string &out_string) const;

	uint16_t GetMemoryByte() const
	{ return 0; };;

	uint8_t GetMemoryByteValue() const
	{ return 0; };

	uint16_t GetSetAddress() const
	{ return 0; };
};

template<typename _derived_addressing, int _operand_length>
inline BaseAddressingStrategy<_derived_addressing, _operand_length>::BaseAddressingStrategy()
{
	m_console = NESConsole::GetInstance().get();
}
