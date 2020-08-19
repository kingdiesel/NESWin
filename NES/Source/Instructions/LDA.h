//
//

#ifndef NES_LDA_H
#define NES_LDA_H

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"

template<typename _addressing_mode, typename _execute, int _op_code>
class LDABase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	LDABase(unsigned char cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "LDA")
	{
	}

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		cpu.SetRegisterA(this->GetAddressingMode().GetMemoryByteValue(cpu, memory));
		cpu.SetZeroFlag(cpu.GetRegisterA() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterA());
	}
};

class LDAImmediate : public LDABase<ImmediateAddressingStrategy, LDAImmediate, 0xA9>
{
public:
	LDAImmediate() : LDABase(2)
	{
	}
};

class LDAAbsolute : public LDABase<AbsoluteAddressingStrategy, LDAAbsolute, 0xAD>
{
public:
	LDAAbsolute() : LDABase(4)
	{
	}
};

class LDAAbsoluteX : public LDABase<AbsoluteXAddressingStrategy, LDAAbsoluteX, 0xBD>
{
public:
	LDAAbsoluteX() : LDABase(4)
	{
	}
};

class LDAAbsoluteY : public LDABase<AbsoluteYAddressingStrategy, LDAAbsoluteY, 0xB9>
{
public:
	LDAAbsoluteY() : LDABase(4)
	{
	}
};

class LDAZeroPage : public LDABase<ZeroPageAddressingStrategy, LDAZeroPage, 0xA5>
{
public:
	LDAZeroPage() : LDABase(3)
	{
	}
};

class LDAZeroPageX : public LDABase<ZeroPageXAddressingStrategy, LDAZeroPageX, 0xB5>
{
public:
	LDAZeroPageX() : LDABase(4)
	{
	}
};

class LDAIndexedIndirect : public LDABase<IndexedIndirectAddressingStrategy, LDAIndexedIndirect, 0xA1>
{
public:
	LDAIndexedIndirect() : LDABase(6)
	{
	}
};

class LDAIndirectIndexed : public LDABase<IndirectIndexedAddressingStrategy, LDAIndirectIndexed, 0xB1>
{
public:
	LDAIndirectIndexed() : LDABase(5)
	{
	}
};

#endif //NES_LDA_H
