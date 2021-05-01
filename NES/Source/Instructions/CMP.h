#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#CPX
template<class _addressing_strategy>
class CPXBase : public OpCodeBase<_addressing_strategy>
{
public:
	CPXBase() : OpCodeBase<_addressing_strategy>("CPX")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		const uint8_t memory_value = this->GetAddressingMode().GetMemoryByteValue();
		const uint8_t compare_value = cpu.GetRegisterX();
		uint8_t value = compare_value - memory_value;
		cpu.SetCarryFlag(compare_value >= memory_value);
		cpu.SetZeroFlag(value == 0);
		cpu.SetNegativeFlagForValue(value);
	}
};

// http://www.obelisk.me.uk/6502/reference.html#CPY
template<class _addressing_strategy>
class CPYBase : public OpCodeBase<_addressing_strategy>
{
public:
	CPYBase() : OpCodeBase<_addressing_strategy>("CPY")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		const uint8_t memory_value = this->GetAddressingMode().GetMemoryByteValue();
		const uint8_t compare_value = cpu.GetRegisterY();
		uint8_t value = compare_value - memory_value;
		cpu.SetCarryFlag(compare_value >= memory_value);
		cpu.SetZeroFlag(value == 0);
		cpu.SetNegativeFlagForValue(value);
	}
};

// http://www.obelisk.me.uk/6502/reference.html#CMP
template<class _addressing_strategy>
class CMPBase : public OpCodeBase<_addressing_strategy>
{
public:
	CMPBase() : OpCodeBase<_addressing_strategy>("CMP")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		const uint8_t memory_value = this->GetAddressingMode().GetMemoryByteValue();
		const uint8_t compare_value = cpu.GetRegisterA();
		uint8_t value = compare_value - memory_value;
		cpu.SetCarryFlag(compare_value >= memory_value);
		cpu.SetZeroFlag(value == 0);
		cpu.SetNegativeFlagForValue(value);
	}
};

typedef BaseInstruction2<CMPBase<ImmediateAddressingStrategy>, 0xC9, 2> CMPImmediate;
typedef BaseInstruction2<CMPBase<AbsoluteAddressingStrategy>, 0xCD, 4> CMPAbsolute;
typedef BaseInstruction2<CMPBase<AbsoluteXAddressingStrategy>, 0xDD, 4> CMPAbsoluteX;
typedef BaseInstruction2<CMPBase<AbsoluteYAddressingStrategy>, 0xD9, 4> CMPAbsoluteY;
typedef BaseInstruction2<CMPBase<ZeroPageAddressingStrategy>, 0xC5, 3> CMPZeroPage;
typedef BaseInstruction2<CMPBase<ZeroPageXAddressingStrategy>, 0xD5, 4> CMPZeroPageX;
typedef BaseInstruction2<CMPBase<IndexedIndirectAddressingStrategy>, 0xC1, 6> CMPIndexedIndirect;
typedef BaseInstruction2<CMPBase<IndirectIndexedAddressingStrategy>, 0xD1, 5> CMPIndirectIndexed;
typedef BaseInstruction2<CPYBase<ImmediateAddressingStrategy>, 0xC0, 2> CPYImmediate;
typedef BaseInstruction2<CPYBase<ZeroPageAddressingStrategy>, 0xC4, 3> CPYZeroPage;
typedef BaseInstruction2<CPYBase<AbsoluteAddressingStrategy>, 0xCC, 4> CPYAbsolute;
typedef BaseInstruction2<CPXBase<ImmediateAddressingStrategy>, 0xE0, 2> CPXImmediate;
typedef BaseInstruction2<CPXBase<ZeroPageAddressingStrategy>, 0xE4, 3> CPXZeroPage;
typedef BaseInstruction2<CPXBase<AbsoluteAddressingStrategy>, 0xEC, 4> CPXAbsolute;