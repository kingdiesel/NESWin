//
//

#ifndef NES_STY_H
#define NES_STY_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"
#include "../CPU.h"

template<typename _addressing_mode, typename _execute, int _op_code>
class STYBase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	STYBase(uint8_t cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "STY")
	{
	}

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		this->GetAddressingMode().SetMemoryByteValue(cpu, memory, cpu.GetRegisterY());
	}
};

class STYAbsolute : public STYBase<AbsoluteAddressingStrategy, STYAbsolute, 0x8C>
{
public:
	STYAbsolute() : STYBase(4)
	{
	}
};

class STYZeroPage : public STYBase<ZeroPageAddressingStrategy, STYZeroPage, 0x84>
{
public:
	STYZeroPage() : STYBase(3)
	{
	}
};

class STYZeroPageX : public STYBase<ZeroPageXAddressingStrategy, STYZeroPageX, 0x94>
{
public:
	STYZeroPageX() : STYBase(4)
	{
	}
};

#endif //NES_STY_H
