//
//

#ifndef NES_INC_H
#define NES_INC_H

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"

template<typename _addressing_mode, typename _execute, int _op_code>
class INCBase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	INCBase(unsigned char cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "INC")
	{
	}

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		unsigned char value = this->GetAddressingMode().GetMemoryByteValue(cpu, memory);
		value += (unsigned char) 1;
		this->GetAddressingMode().SetMemoryByteValue(cpu, memory, value);
		cpu.SetZeroFlag(value == 0);
		cpu.SetNegativeFlagForValue(value);
	}
};

class INCAbsolute : public INCBase<AbsoluteAddressingStrategy, INCAbsolute, 0xEE>
{
public:
	INCAbsolute() : INCBase(6)
	{
	}
};

class INCAbsoluteX : public INCBase<AbsoluteXAddressingStrategy, INCAbsoluteX, 0xFE>
{
public:
	INCAbsoluteX() : INCBase(7)
	{
	}
};

class INCZeroPage : public INCBase<ZeroPageAddressingStrategy, INCZeroPage, 0xE6>
{
public:
	INCZeroPage() : INCBase(5)
	{
	}
};

class INCZeroPageX : public INCBase<ZeroPageXAddressingStrategy, INCZeroPageX, 0xF6>
{
public:
	INCZeroPageX() : INCBase(6)
	{
	}
};

#endif //NES_INC_H
