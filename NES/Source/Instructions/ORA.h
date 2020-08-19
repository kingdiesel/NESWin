//
//

#ifndef NES_ORA_H
#define NES_ORA_H

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"

template<typename _addressing_mode, typename _execute, int _op_code>
class ORABase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	ORABase(uint8_t cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "ORA")
	{
	}

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue(cpu, memory);
		cpu.SetRegisterA(value | cpu.GetRegisterA());
		cpu.SetZeroFlag(cpu.GetRegisterA() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterA());
	}
};

class ORAImmediate : public ORABase<ImmediateAddressingStrategy, ORAImmediate, 0x09>
{
public:
	ORAImmediate() : ORABase(2)
	{
	}
};

class ORAAbsolute : public ORABase<AbsoluteAddressingStrategy, ORAAbsolute, 0x0D>
{
public:
	ORAAbsolute() : ORABase(4)
	{
	}
};

class ORAAbsoluteX : public ORABase<AbsoluteXAddressingStrategy, ORAAbsoluteX, 0x1D>
{
public:
	ORAAbsoluteX() : ORABase(4)
	{
	}
};

class ORAAbsoluteY : public ORABase<AbsoluteYAddressingStrategy, ORAAbsoluteY, 0x19>
{
public:
	ORAAbsoluteY() : ORABase(4)
	{
	}
};

class ORAZeroPage : public ORABase<ZeroPageAddressingStrategy, ORAZeroPage, 0x05>
{
public:
	ORAZeroPage() : ORABase(3)
	{
	}
};

class ORAZeroPageX : public ORABase<ZeroPageXAddressingStrategy, ORAZeroPageX, 0x15>
{
public:
	ORAZeroPageX() : ORABase(4)
	{
	}
};

class ORAIndexedIndirect : public ORABase<IndexedIndirectAddressingStrategy, ORAIndexedIndirect, 0x01>
{
public:
	ORAIndexedIndirect() : ORABase(6)
	{
	}
};

class ORAIndirectIndexed : public ORABase<IndirectIndexedAddressingStrategy, ORAIndirectIndexed, 0x11>
{
public:
	ORAIndirectIndexed() : ORABase(5)
	{
	}
};

#endif //NES_ORA_H
