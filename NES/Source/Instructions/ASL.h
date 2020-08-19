//
// Created by kingdiesel on 12/31/17.
//

#ifndef NES_ASL_H
#define NES_ASL_H

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"

template<typename _addressing_mode, typename _execute, int _op_code>
class ASLBase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	ASLBase(unsigned char cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "ASL")
	{
	}

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		unsigned char value = this->GetAddressingMode().GetMemoryByteValue(cpu, memory);
		unsigned char bit_seven = value & (unsigned char) 0x80;
		cpu.SetCarryFlag(bit_seven != 0);
		value = value << 1;
		value &= (unsigned char) 0xFE;
		cpu.SetZeroFlag(value == 0);
		cpu.SetNegativeFlagForValue(value);
		this->GetAddressingMode().SetMemoryByteValue(cpu, memory, value);
	}
};

class ASLAccumulator : public ASLBase<AccumulatorAddressingStrategy, ASLAccumulator, 0x0A>
{
public:
	ASLAccumulator() : ASLBase(2)
	{
	}
};

class ASLAbsolute : public ASLBase<AbsoluteAddressingStrategy, ASLAbsolute, 0x0E>
{
public:
	ASLAbsolute() : ASLBase(6)
	{
	}
};

class ASLAbsoluteX : public ASLBase<AbsoluteXAddressingStrategy, ASLAbsoluteX, 0x1E>
{
public:
	ASLAbsoluteX() : ASLBase(7)
	{
	}
};

class ASLZeroPage : public ASLBase<ZeroPageAddressingStrategy, ASLZeroPage, 0x06>
{
public:
	ASLZeroPage() : ASLBase(5)
	{
	}
};

class ASLZeroPageX : public ASLBase<ZeroPageXAddressingStrategy, ASLZeroPageX, 0x16>
{
public:
	ASLZeroPageX() : ASLBase(6)
	{
	}
};

#endif //NES_ASL_H
