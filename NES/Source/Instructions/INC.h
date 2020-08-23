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
	INCBase(uint8_t cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "INC")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue();
		value += (uint8_t) 1;
		this->GetAddressingMode().SetMemoryByteValue(value);
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
