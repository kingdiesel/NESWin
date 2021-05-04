#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#ROL

template<class _addressing_strategy>
class ROLBase : public OpCodeBase<_addressing_strategy>
{
public:
	ROLBase() : OpCodeBase<_addressing_strategy>("ROL")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue();
		uint8_t bit_seven = value & (uint8_t)0x80;
		bool carry_flag = cpu.IsCarryFlagSet();
		value = value << 1;
		if (carry_flag)
		{
			value |= (uint8_t)0x01;
		}
		else
		{
			value &= (uint8_t)~0x01;
		}
		cpu.SetCarryFlag(bit_seven != 0);
		cpu.SetZeroFlag(value == 0);
		cpu.SetNegativeFlagForValue(value);
		this->GetAddressingMode().SetMemoryByteValue(value);
	}
};

typedef BaseInstruction<ROLBase<AccumulatorAddressingStrategy>, 0x2A, 2> ROLAccumulator;
typedef BaseInstruction<ROLBase<AbsoluteAddressingStrategy>, 0x2E, 6> ROLAbsolute;
typedef BaseInstruction<ROLBase<AbsoluteXAddressingStrategy>, 0x3E, 7> ROLAbsoluteX;
typedef BaseInstruction<ROLBase<ZeroPageAddressingStrategy>, 0x26, 5> ROLZeroPage;
typedef BaseInstruction<ROLBase<ZeroPageXAddressingStrategy>, 0x36, 6> ROLZeroPageX;
