#pragma once
#include <string>
#include <sstream>
#include "../Memory.h"
#include "../CPU.h"
#include "../NESConsole.h"
// https://wiki.nesdev.com/w/index.php/CPU_addressing_modes
template<class _T>
concept AddressingConcept = requires(_T AddressStrategy, std::string & out_string, uint8_t value)
{
	AddressStrategy.ToString(out_string);
	AddressStrategy.SetMemoryByteValue(value);
	AddressStrategy.GetSetAddress();
	{AddressStrategy.GetMemoryByte()} -> std::same_as<uint8_t>;
	{AddressStrategy.GetMemoryShort()} -> std::same_as<uint16_t>;
	{AddressStrategy.GetMemoryByteValue()} -> std::same_as<uint8_t>;
	{AddressStrategy.GetMemoryShortValue()} -> std::same_as<uint16_t>;
	{AddressStrategy.GetAddressingCycles()} -> std::same_as<int>;
};

template<AddressingConcept _Addressing_Strategy, int _operand_length>
class BaseAddressingStrategy2
{
public:
	BaseAddressingStrategy2() = default;

	void SetMemoryByteValue(uint8_t value)
	{
		m_addressing_strategy.SetMemoryByteValue(value);
	}

	uint8_t GetMemoryByte() const
	{
		return m_addressing_strategy.GetMemoryByte();
	}

	uint16_t GetMemoryShort() const
	{
		return m_addressing_strategy.GetMemoryShort();
	}

	uint8_t GetMemoryByteValue() const
	{
		return m_addressing_strategy.GetMemoryByteValue();
	}

	uint16_t GetMemoryShortValue() const
	{
		return m_addressing_strategy.GetMemoryShortValue();
	}

	void ToString(std::string& out_string) const
	{
		m_addressing_strategy.ToString(out_string);
	}

	int GetAddressingCycles() const
	{
		return m_addressing_strategy.GetAddressingCycles();
	}

	int GetOperandLength() const
	{
		return _operand_length;
	}
private:
	_Addressing_Strategy m_addressing_strategy;
};

class BaseAddressingImpl
{
public:
	int GetAddressingCycles() const
	{
		return m_addressing_cycles;
	}
protected:
	mutable int m_addressing_cycles = 0;
};

class AbsoluteAddressingImpl : public BaseAddressingImpl
{
public:
	void ToString(std::string& out_string) const;
	void SetMemoryByteValue(uint8_t value) const;
	uint8_t GetMemoryByte() const;
	uint16_t GetMemoryShort() const;
	uint8_t GetMemoryByteValue() const;
	uint16_t GetMemoryShortValue() const;
	uint16_t GetSetAddress() const;
};

class RelativeAddressingImpl : public BaseAddressingImpl
{
public:
	void ToString(std::string& out_string) const;
	void SetMemoryByteValue(uint8_t value) const;
	uint8_t GetMemoryByte() const;
	uint16_t GetMemoryShort() const;
	uint8_t GetMemoryByteValue() const;
	uint16_t GetMemoryShortValue() const;
	uint16_t GetSetAddress() const;
};

class AbsoluteYAddressingImpl : public BaseAddressingImpl
{
public:
	void ToString(std::string& out_string) const;
	void SetMemoryByteValue(uint8_t value) const;
	uint8_t GetMemoryByte() const;
	uint16_t GetMemoryShort() const;
	uint8_t GetMemoryByteValue() const;
	uint16_t GetMemoryShortValue() const;
	uint16_t GetSetAddress() const;
};

class AbsoluteXAddressingImpl : public BaseAddressingImpl
{
public:
	void ToString(std::string& out_string) const;
	void SetMemoryByteValue(uint8_t value) const;
	uint8_t GetMemoryByte() const;
	uint16_t GetMemoryShort() const;
	uint8_t GetMemoryByteValue() const;
	uint16_t GetMemoryShortValue() const;
	uint16_t GetSetAddress() const;
};

class ZeroPageAddressingImpl : public BaseAddressingImpl
{
public:
	void ToString(std::string& out_string) const;
	void SetMemoryByteValue(uint8_t value) const;
	uint8_t GetMemoryByte() const;
	uint16_t GetMemoryShort() const;
	uint8_t GetMemoryByteValue() const;
	uint16_t GetMemoryShortValue() const;
	uint16_t GetSetAddress() const;
};

class ZeroPageXAddressingImpl : public BaseAddressingImpl
{
public:
	void ToString(std::string& out_string) const;
	void SetMemoryByteValue(uint8_t value) const;
	uint8_t GetMemoryByte() const;
	uint16_t GetMemoryShort() const;
	uint8_t GetMemoryByteValue() const;
	uint16_t GetMemoryShortValue() const;
	uint16_t GetSetAddress() const;
};

class ZeroPageYAddressingImpl : public BaseAddressingImpl
{
public:
	void ToString(std::string& out_string) const;
	void SetMemoryByteValue(uint8_t value) const;
	uint8_t GetMemoryByte() const;
	uint16_t GetMemoryShort() const;
	uint8_t GetMemoryByteValue() const;
	uint16_t GetMemoryShortValue() const;
	uint16_t GetSetAddress() const;
};

class IndexedIndirectAddressingImpl : public BaseAddressingImpl
{
public:
	void ToString(std::string& out_string) const;
	void SetMemoryByteValue(uint8_t value) const;
	uint8_t GetMemoryByte() const;
	uint16_t GetMemoryShort() const;
	uint8_t GetMemoryByteValue() const;
	uint16_t GetMemoryShortValue() const;
	uint16_t GetSetAddress() const;
};

class IndirectIndexedAddressingImpl : public BaseAddressingImpl
{
public:
	void ToString(std::string& out_string) const;
	void SetMemoryByteValue(uint8_t value) const;
	uint8_t GetMemoryByte() const;
	uint16_t GetMemoryShort() const;
	uint8_t GetMemoryByteValue() const;
	uint16_t GetMemoryShortValue() const;
	uint16_t GetSetAddress() const;
};

class ImpliedAddressingImpl : public BaseAddressingImpl
{
public:
	void ToString(std::string& out_string) const;

	void SetMemoryByteValue(uint8_t value) const
	{
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		const uint16_t address = GetSetAddress();
		memory.CPUWriteByte(address, value);
	}

	uint8_t GetMemoryByte() const
	{
		return 0;
	}

	uint16_t GetMemoryShort() const
	{
		return 0;
	}

	uint8_t GetMemoryByteValue() const
	{
		return 0;
	}

	uint16_t GetSetAddress() const
	{
		return 0;
	}

	uint16_t GetMemoryShortValue() const
	{
		return 0;
	}
};

class IndirectAddressingImpl : public BaseAddressingImpl
{
public:
	void ToString(std::string& out_string) const;
	void SetMemoryByteValue(uint8_t value) const;
	uint8_t GetMemoryByte() const;
	uint16_t GetMemoryShort() const;
	uint8_t GetMemoryByteValue() const;
	uint16_t GetMemoryShortValue() const;
	uint16_t GetSetAddress() const;
};

class ImmediateAddressingImpl : public BaseAddressingImpl
{
public:
	void ToString(std::string& out_string) const;
	void SetMemoryByteValue(uint8_t value) const;
	uint8_t GetMemoryByte() const;
	uint16_t GetMemoryShort() const;
	uint8_t GetMemoryByteValue() const;
	uint16_t GetMemoryShortValue() const;
	uint16_t GetSetAddress() const;
};

class AccumulatorAddressingImpl : public BaseAddressingImpl
{
public:
	void ToString(std::string& out_string) const;
	uint8_t GetMemoryByte() const;
	uint16_t GetMemoryShort() const;
	uint8_t GetMemoryByteValue() const;
	uint16_t GetMemoryShortValue() const;
	void SetMemoryByteValue(uint8_t value) const;
	uint16_t GetSetAddress() const;
};

class JMPAbsoluteAddressingImpl : public BaseAddressingImpl
{
public:
	void ToString(std::string& out_string) const;
	void SetMemoryByteValue(uint8_t value) const;
	uint8_t GetMemoryByte() const;
	uint16_t GetMemoryShort() const;
	uint8_t GetMemoryByteValue() const;
	uint16_t GetMemoryShortValue() const;
	uint16_t GetSetAddress() const;
};

// Fetches the value from a 16-bit address anywhere in memory.
typedef BaseAddressingStrategy2<AbsoluteAddressingImpl, 2> AbsoluteAddressingStrategy;
typedef BaseAddressingStrategy2<AbsoluteYAddressingImpl, 2> AbsoluteYAddressingStrategy;
typedef BaseAddressingStrategy2<AbsoluteXAddressingImpl, 2> AbsoluteXAddressingStrategy;

// Branch instructions (e.g. BEQ, BCS) have a relative addressing mode that 
// specifies an 8-bit signed offset relative to the current PC
typedef BaseAddressingStrategy2<RelativeAddressingImpl, 1> RelativeAddressingStrategy;

// Fetches the value from an 8-bit address on the zero page.
typedef BaseAddressingStrategy2<ZeroPageAddressingImpl, 1> ZeroPageAddressingStrategy;
typedef BaseAddressingStrategy2<ZeroPageXAddressingImpl, 1> ZeroPageXAddressingStrategy;
typedef BaseAddressingStrategy2<ZeroPageYAddressingImpl, 1> ZeroPageYAddressingStrategy;

// The JMP instruction has a special indirect addressing mode that can 
// jump to the address stored in a 16-bit pointer anywhere in memory.
typedef BaseAddressingStrategy2<IndexedIndirectAddressingImpl, 1> IndexedIndirectAddressingStrategy;
typedef BaseAddressingStrategy2<IndirectIndexedAddressingImpl, 1> IndirectIndexedAddressingStrategy;
typedef BaseAddressingStrategy2<IndirectAddressingImpl, 2> IndirectAddressingStrategy;

// Instructions like RTS or CLC have no address operand, the destination of results are implied.
typedef BaseAddressingStrategy2<ImpliedAddressingImpl, 0> ImpliedAddressingStrategy;

// Uses the 8-bit operand itself as the value for the operation, rather than fetching a value from a memory address.
typedef BaseAddressingStrategy2<ImmediateAddressingImpl, 1> ImmediateAddressingStrategy;

// Many instructions can operate on the accumulator, e.g. LSR A. 
// Some assemblers will treat no operand as an implicit A where applicable
typedef BaseAddressingStrategy2<AccumulatorAddressingImpl, 0> AccumulatorAddressingStrategy;
typedef BaseAddressingStrategy2<JMPAbsoluteAddressingImpl, 2> JMPAbsoluteAddressingStrategy;