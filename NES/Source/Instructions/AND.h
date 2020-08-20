//
//

#ifndef NES_AND_H
#define NES_AND_H

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"

template<typename _addressing_mode, typename _execute, int _op_code>
class ANDBase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	ANDBase(uint8_t cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "AND")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue();
		cpu.SetRegisterA(value & cpu.GetRegisterA());
		cpu.SetZeroFlag(cpu.GetRegisterA() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterA());
	}
};

class ANDImmediate : public ANDBase<ImmediateAddressingStrategy, ANDImmediate, 0x29>
{
public:
	ANDImmediate() : ANDBase(2)
	{
	}
};

class ANDAbsolute : public ANDBase<AbsoluteAddressingStrategy, ANDAbsolute, 0x2D>
{
public:
	ANDAbsolute() : ANDBase(4)
	{
	}
};

class ANDAbsoluteX : public ANDBase<AbsoluteXAddressingStrategy, ANDAbsoluteX, 0x3D>
{
public:
	ANDAbsoluteX() : ANDBase(4)
	{
	}
};

class ANDAbsoluteY : public ANDBase<AbsoluteYAddressingStrategy, ANDAbsoluteY, 0x39>
{
public:
	ANDAbsoluteY() : ANDBase(4)
	{
	}
};

class ANDZeroPage : public ANDBase<ZeroPageAddressingStrategy, ANDZeroPage, 0x25>
{
public:
	ANDZeroPage() : ANDBase(3)
	{
	}
};

class ANDZeroPageX : public ANDBase<ZeroPageXAddressingStrategy, ANDZeroPageX, 0x35>
{
public:
	ANDZeroPageX() : ANDBase(4)
	{
	}
};

class ANDIndexedIndirect : public ANDBase<IndexedIndirectAddressingStrategy, ANDIndexedIndirect, 0x21>
{
public:
	ANDIndexedIndirect() : ANDBase(6)
	{
	}
};

class ANDIndirectIndexed : public ANDBase<IndirectIndexedAddressingStrategy, ANDIndirectIndexed, 0x31>
{
public:
	ANDIndirectIndexed() : ANDBase(5)
	{
	}
};

#endif //NES_AND_H
