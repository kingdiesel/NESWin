//
// Created by kingdiesel on 12/31/17.
//

#ifndef NES_ROR_H
#define NES_ROR_H

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"

template<typename _addressing_mode, typename _execute, int _op_code>
class RORBase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	RORBase(uint8_t cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "ROR")
	{
	}

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue(cpu, memory);
		uint8_t bit_zero = value & (uint8_t) 0x01;
		bool carry_flag = cpu.IsCarryFlagSet();
		value = value >> 1;
		if (carry_flag)
		{
			value |= (uint8_t) 0x80;
		}
		else
		{
			value &= (uint8_t) 0x7F;
		}
		cpu.SetCarryFlag(bit_zero != 0);
		cpu.SetZeroFlag(value == 0);
		cpu.SetNegativeFlagForValue(value);
		this->GetAddressingMode().SetMemoryByteValue(cpu, memory, value);


	}
};

class RORAccumulator : public RORBase<AccumulatorAddressingStrategy, RORAccumulator, 0x6A>
{
public:
	RORAccumulator() : RORBase(2)
	{
	}
};

class RORAbsolute : public RORBase<AbsoluteAddressingStrategy, RORAbsolute, 0x6E>
{
public:
	RORAbsolute() : RORBase(6)
	{
	}
};

class RORAbsoluteX : public RORBase<AbsoluteXAddressingStrategy, RORAbsoluteX, 0x7E>
{
public:
	RORAbsoluteX() : RORBase(7)
	{
	}
};

class RORZeroPage : public RORBase<ZeroPageAddressingStrategy, RORZeroPage, 0x66>
{
public:
	RORZeroPage() : RORBase(5)
	{
	}
};

class RORZeroPageX : public RORBase<ZeroPageXAddressingStrategy, RORZeroPageX, 0x76>
{
public:
	RORZeroPageX() : RORBase(6)
	{
	}
};

#endif //NES_ROR_H
