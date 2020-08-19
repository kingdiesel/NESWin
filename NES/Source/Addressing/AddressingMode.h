#ifndef NES_ADDRESSINGMODE_H
#define NES_ADDRESSINGMODE_H

#include <string>
#include <sstream>
#include "../Memory.h"
#include "../CPU.h"

class CPU;
class Memory;

template<typename _derived_addressing, int _operand_length>
class BaseAddressingStrategy
{
public:
	void SetMemoryByteValue(CPU &cpu, uint8_t value)
	{
		uint16_t address = static_cast<_derived_addressing *>(this)->GetSetAddress(cpu, cpu.GetMemory());
		cpu.GetMemory().SetByte(address, value);
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
};

class AbsoluteAddressingStrategy : public BaseAddressingStrategy<AbsoluteAddressingStrategy, 2>
{
public:
	void ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const;

	uint16_t GetMemoryByte(const CPU &cpu, const Memory &memory) const;

	uint8_t GetMemoryByteValue(const CPU &cpu, const Memory &memory) const;

	uint16_t GetSetAddress(const CPU &cpu, const Memory &memory) const;
};

class RelativeAddressingStrategy : public BaseAddressingStrategy<RelativeAddressingStrategy, 1>
{
public:
	void ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const;

	uint8_t GetMemoryByte(const CPU &cpu, const Memory &memory) const;

	uint16_t GetMemoryByteValue(const CPU &cpu, const Memory &memory) const;
};

class AbsoluteYAddressingStrategy : public BaseAddressingStrategy<AbsoluteYAddressingStrategy, 2>
{
public:
	void ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const;

	uint16_t GetMemoryByte(const CPU &cpu, const Memory &memory) const;

	uint8_t GetMemoryByteValue(const CPU &cpu, const Memory &memory) const;

	uint16_t GetSetAddress(const CPU &cpu, const Memory &memory) const;
};

class AbsoluteXAddressingStrategy : public BaseAddressingStrategy<AbsoluteXAddressingStrategy, 2>
{
public:
	void ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const;

	uint16_t GetMemoryByte(const CPU &cpu, const Memory &memory) const;

	uint8_t GetMemoryByteValue(const CPU &cpu, const Memory &memory) const;

	uint16_t GetSetAddress(const CPU &cpu, const Memory &memory) const;
};

class ZeroPageAddressingStrategy : public BaseAddressingStrategy<ZeroPageAddressingStrategy, 1>
{
public:
	void ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const;

	uint8_t GetMemoryByte(const CPU &cpu, const Memory &memory) const;

	uint8_t GetMemoryByteValue(const CPU &cpu, const Memory &memory) const;

	uint16_t GetSetAddress(const CPU &cpu, const Memory &memory) const;
};

class ZeroPageXAddressingStrategy : public BaseAddressingStrategy<ZeroPageXAddressingStrategy, 1>
{
public:
	void ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const;

	uint8_t GetMemoryByte(const CPU &cpu, const Memory &memory) const;

	uint8_t GetMemoryByteValue(const CPU &cpu, const Memory &memory) const;

	uint16_t GetSetAddress(const CPU &cpu, const Memory &memory) const;
};

class ZeroPageYAddressingStrategy : public BaseAddressingStrategy<ZeroPageYAddressingStrategy, 1>
{
public:
	void ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const;

	uint8_t GetMemoryByte(const CPU &cpu, const Memory &memory) const;

	uint8_t GetMemoryByteValue(const CPU &cpu, const Memory &memory) const;

	uint16_t GetSetAddress(const CPU &cpu, const Memory &memory) const;
};

class IndexedIndirectAddressingStrategy : public BaseAddressingStrategy<IndexedIndirectAddressingStrategy, 1>
{
public:
	void ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const;

	uint8_t GetMemoryByte(const CPU &cpu, const Memory &memory) const;

	uint8_t GetMemoryByteValue(const CPU &cpu, const Memory &memory) const;

	uint16_t GetValueAddress(const CPU &cpu, const Memory &memory) const;

	uint16_t GetSetAddress(const CPU &cpu, const Memory &memory) const;
};

class IndirectIndexedAddressingStrategy : public BaseAddressingStrategy<IndirectIndexedAddressingStrategy, 1>
{
public:
	void ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const;

	uint8_t GetMemoryByte(const CPU &cpu, const Memory &memory) const;

	uint8_t GetMemoryByteValue(const CPU &cpu, const Memory &memory) const;

	uint16_t GetValueAddress(const CPU &cpu, const Memory &memory) const;

	uint16_t GetSetAddress(const CPU &cpu, const Memory &memory) const;
};

class JMPAbsoluteAddressingStrategy : public BaseAddressingStrategy<JMPAbsoluteAddressingStrategy, 2>
{
public:
	void ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const;

	uint16_t GetMemoryByte(const CPU &cpu, const Memory &memory) const;

	uint8_t GetMemoryByteValue(const CPU &cpu, const Memory &memory) const;
};

class AccumulatorAddressingStrategy
{
public:
	void ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const;

	uint8_t GetMemoryByte(const CPU &cpu, const Memory &memory) const;

	uint8_t GetMemoryByteValue(const CPU &cpu, const Memory &memory) const;

	void SetMemoryByteValue(CPU &cpu, uint8_t value) const;

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
};

class ImmediateAddressingStrategy : public BaseAddressingStrategy<ImmediateAddressingStrategy, 1>
{
public:
	void ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const;

	uint8_t GetMemoryByte(const CPU &cpu, const Memory &memory) const;

	uint8_t GetMemoryByteValue(const CPU &cpu, const Memory &memory) const;

	uint16_t GetSetAddress(const CPU &cpu, const Memory &memory) const;
};

class IndirectAddressingStrategy : public BaseAddressingStrategy<IndirectAddressingStrategy, 2>
{
public:
	void ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const;

	uint16_t GetMemoryByte(const CPU &cpu, const Memory &memory) const;

	uint16_t GetMemoryByteValue(const CPU &cpu, const Memory &memory) const;

	uint16_t GetSetAddress(const CPU &cpu, const Memory &memory) const;
};

class ImpliedAddressingStrategy : public BaseAddressingStrategy<ImpliedAddressingStrategy, 0>
{
public:
	void ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const;

	uint16_t GetMemoryByte(const CPU &cpu, const Memory &memory) const
	{ return 0; };;

	uint8_t GetMemoryByteValue(const CPU &cpu, const Memory &memory) const
	{ return 0; };

	uint16_t GetSetAddress(const CPU &cpu, const Memory &memory) const
	{ return 0; };
};


#endif //NES_ADDRESSINGMODE_H
