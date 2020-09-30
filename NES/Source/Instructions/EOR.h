//
//

#ifndef NES_EOR_H
#define NES_EOR_H

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"
// http://www.obelisk.me.uk/6502/reference.html#EOR
template<typename _addressing_mode, typename _execute, int _op_code>
class EORBase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	EORBase(uint8_t cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "EOR")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue();
		cpu.SetRegisterA(value ^ cpu.GetRegisterA());
		cpu.SetZeroFlag(cpu.GetRegisterA() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterA());
	}
};

class EORImmediate : public EORBase<ImmediateAddressingStrategy, EORImmediate, 0x49>
{
public:
	EORImmediate() : EORBase(2)
	{
	}
};

class EORAbsolute : public EORBase<AbsoluteAddressingStrategy, EORAbsolute, 0x4D>
{
public:
	EORAbsolute() : EORBase(4)
	{
	}
};

class EORAbsoluteX : public EORBase<AbsoluteXAddressingStrategy, EORAbsoluteX, 0x5D>
{
public:
	EORAbsoluteX() : EORBase(4)
	{
	}
};

class EORAbsoluteY : public EORBase<AbsoluteYAddressingStrategy, EORAbsoluteY, 0x59>
{
public:
	EORAbsoluteY() : EORBase(4)
	{
	}
};

class EORZeroPage : public EORBase<ZeroPageAddressingStrategy, EORZeroPage, 0x45>
{
public:
	EORZeroPage() : EORBase(3)
	{
	}
};

class EORZeroPageX : public EORBase<ZeroPageXAddressingStrategy, EORZeroPageX, 0x55>
{
public:
	EORZeroPageX() : EORBase(4)
	{
	}
};

class EORIndexedIndirect : public EORBase<IndexedIndirectAddressingStrategy, EORIndexedIndirect, 0x41>
{
public:
	EORIndexedIndirect() : EORBase(6)
	{
	}
};

class EORIndirectIndexed : public EORBase<IndirectIndexedAddressingStrategy, EORIndirectIndexed, 0x51>
{
public:
	EORIndirectIndexed() : EORBase(5)
	{
	}
};

#endif //NES_EOR_H
