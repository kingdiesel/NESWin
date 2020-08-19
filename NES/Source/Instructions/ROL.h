//
// Created by kingdiesel on 12/31/17.
//

#ifndef NES_ROL_H
#define NES_ROL_H

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"

template<typename _addressing_mode, typename _execute, int _op_code>
class ROLBase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	ROLBase(uint8_t cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "ROL")
	{
	}

	void ExecuteImplementation(CPU &cpu)
	{
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue(cpu, cpu.GetMemory());
		uint8_t bit_seven = value & (uint8_t) 0x80;
		bool carry_flag = cpu.IsCarryFlagSet();
		value = value << 1;
		if (carry_flag)
		{
			value |= (uint8_t) 0x01;
		}
		else
		{
			value &= (uint8_t) ~0x01;
		}
		cpu.SetCarryFlag(bit_seven != 0);
		cpu.SetZeroFlag(value == 0);
		cpu.SetNegativeFlagForValue(value);
		this->GetAddressingMode().SetMemoryByteValue(cpu, cpu.GetMemory(), value);


	}
};

class ROLAccumulator : public ROLBase<AccumulatorAddressingStrategy, ROLAccumulator, 0x2A>
{
public:
	ROLAccumulator() : ROLBase(2)
	{
	}
};

class ROLAbsolute : public ROLBase<AbsoluteAddressingStrategy, ROLAbsolute, 0x2E>
{
public:
	ROLAbsolute() : ROLBase(6)
	{
	}
};

class ROLAbsoluteX : public ROLBase<AbsoluteXAddressingStrategy, ROLAbsoluteX, 0x3E>
{
public:
	ROLAbsoluteX() : ROLBase(7)
	{
	}
};

class ROLZeroPage : public ROLBase<ZeroPageAddressingStrategy, ROLZeroPage, 0x26>
{
public:
	ROLZeroPage() : ROLBase(5)
	{
	}
};

class ROLZeroPageX : public ROLBase<ZeroPageXAddressingStrategy, ROLZeroPageX, 0x36>
{
public:
	ROLZeroPageX() : ROLBase(6)
	{
	}
};

#endif //NES_ROL_H
