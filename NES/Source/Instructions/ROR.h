#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#ROR
template<class _addressing_strategy>
class RORBase : public OpCodeBase<_addressing_strategy>
{
public:
	RORBase() : OpCodeBase<_addressing_strategy>("ROR")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue();
		uint8_t bit_zero = value & (uint8_t)0x01;
		bool carry_flag = cpu.IsCarryFlagSet();
		value = value >> 1;
		if (carry_flag)
		{
			value |= (uint8_t)0x80;
		}
		else
		{
			value &= (uint8_t)0x7F;
		}
		cpu.SetCarryFlag(bit_zero != 0);
		cpu.SetZeroFlag(value == 0);
		cpu.SetNegativeFlagForValue(value);
		this->GetAddressingMode().SetMemoryByteValue(value);
	}
};

typedef BaseInstruction2<RORBase<AccumulatorAddressingStrategy>, 0x6A, 2> RORAccumulator;
typedef BaseInstruction2<RORBase<AbsoluteAddressingStrategy>, 0x6E, 6> RORAbsolute;
typedef BaseInstruction2<RORBase<AbsoluteXAddressingStrategy>, 0x7E, 7> RORAbsoluteX;
typedef BaseInstruction2<RORBase<ZeroPageAddressingStrategy>, 0x66, 5> RORZeroPage;
typedef BaseInstruction2<RORBase<ZeroPageXAddressingStrategy>, 0x76, 6> RORZeroPageX;
