//
//

#ifndef NES_STA_H
#define NES_STA_H

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"

template<typename _addressing_mode, typename _execute, int _op_code>
class STABase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	STABase(unsigned char cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "STA")
	{
	}

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		this->GetAddressingMode().SetMemoryByteValue(cpu, memory, cpu.GetRegisterA());
	}
};

class STAAbsolute : public STABase<AbsoluteAddressingStrategy, STAAbsolute, 0x8D>
{
public:
	STAAbsolute() : STABase(4)
	{
	}
};

class STAAbsoluteX : public STABase<AbsoluteXAddressingStrategy, STAAbsoluteX, 0x9D>
{
public:
	STAAbsoluteX() : STABase(5)
	{
	}
};

class STAAbsoluteY : public STABase<AbsoluteYAddressingStrategy, STAAbsoluteY, 0x99>
{
public:
	STAAbsoluteY() : STABase(5)
	{
	}
};

class STAZeroPage : public STABase<ZeroPageAddressingStrategy, STAZeroPage, 0x85>
{
public:
	STAZeroPage() : STABase(3)
	{
	}
};

class STAZeroPageX : public STABase<ZeroPageXAddressingStrategy, STAZeroPageX, 0x95>
{
public:
	STAZeroPageX() : STABase(4)
	{
	}
};

class STAIndexedIndirect : public STABase<IndexedIndirectAddressingStrategy, STAIndexedIndirect, 0x81>
{
public:
	STAIndexedIndirect() : STABase(6)
	{
	}
};

class STAIndirectIndexed : public STABase<IndirectIndexedAddressingStrategy, STAIndirectIndexed, 0x91>
{
public:
	STAIndirectIndexed() : STABase(6)
	{
	}
};

#endif //NES_STA_H
