//
//

#ifndef NES_BIT_H
#define NES_BIT_H

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"

template<typename _addressing_mode, typename _execute, int _op_code>
class BITBase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	BITBase(unsigned char cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "BIT")
	{
	}

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		unsigned char value = this->GetAddressingMode().GetMemoryByteValue(cpu, memory);
		unsigned char result = cpu.GetRegisterA() & value;
		cpu.SetZeroFlag(result == 0);
		cpu.SetNegativeFlagForValue(value);
		cpu.SetOverflowFlag((value & 0x40) != 0);
	}
};

class BITAbsolute : public BITBase<AbsoluteAddressingStrategy, BITAbsolute, 0x2C>
{
public:
	BITAbsolute() : BITBase(4)
	{
	}
};

class BITZeroPage : public BITBase<ZeroPageAddressingStrategy, BITZeroPage, 0x24>
{
public:
	BITZeroPage() : BITBase(3)
	{
	}
};

#endif //NES_BIT_H
