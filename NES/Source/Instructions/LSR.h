//
//

#ifndef NES_LSR_H
#define NES_LSR_H

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"

template<typename _addressing_mode, typename _execute, int _op_code>
class LSRBase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	LSRBase(uint8_t cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "LSR")
	{
	}

	void ExecuteImplementation(CPU &cpu)
	{
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue(cpu, cpu.GetMemory());
		uint8_t bit_zero = value & (uint8_t) 0x01;
		cpu.SetCarryFlag(bit_zero != 0);
		value = value >> 1;
		value &= (uint8_t) 0x7F;
		cpu.SetZeroFlag(value == 0);
		cpu.SetNegativeFlagForValue(value);
		this->GetAddressingMode().SetMemoryByteValue(cpu, cpu.GetMemory(), value);
	}
};

class LSRAccumulator : public LSRBase<AccumulatorAddressingStrategy, LSRAccumulator, 0x4A>
{
public:
	LSRAccumulator() : LSRBase(2)
	{
	}
};

class LSRAbsolute : public LSRBase<AbsoluteAddressingStrategy, LSRAbsolute, 0x4E>
{
public:
	LSRAbsolute() : LSRBase(6)
	{
	}
};

class LSRAbsoluteX : public LSRBase<AbsoluteXAddressingStrategy, LSRAbsoluteX, 0x5E>
{
public:
	LSRAbsoluteX() : LSRBase(7)
	{
	}
};

class LSRZeroPage : public LSRBase<ZeroPageAddressingStrategy, LSRZeroPage, 0x46>
{
public:
	LSRZeroPage() : LSRBase(5)
	{
	}
};

class LSRZeroPageX : public LSRBase<ZeroPageXAddressingStrategy, LSRZeroPageX, 0x56>
{
public:
	LSRZeroPageX() : LSRBase(6)
	{
	}
};

#endif //NES_LSR_H
