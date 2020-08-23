//
//

#ifndef NES_LDY_H
#define NES_LDY_H

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"

template<typename _addressing_mode, typename _execute, int _op_code>
class LDYBase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	LDYBase(uint8_t cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "LDY")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.SetRegisterY(this->GetAddressingMode().GetMemoryByteValue());
		cpu.SetZeroFlag(cpu.GetRegisterY() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterY());
	}
};

class LDYImmediate : public LDYBase<ImmediateAddressingStrategy, LDYImmediate, 0xA0>
{
public:
	LDYImmediate() : LDYBase(2)
	{
	}
};

class LDYAbsolute : public LDYBase<AbsoluteAddressingStrategy, LDYAbsolute, 0xAC>
{
public:
	LDYAbsolute() : LDYBase(4)
	{
	}
};

class LDYAbsoluteX : public LDYBase<AbsoluteXAddressingStrategy, LDYAbsoluteX, 0xBC>
{
public:
	LDYAbsoluteX() : LDYBase(4)
	{
	}
};

class LDYZeroPage : public LDYBase<ZeroPageAddressingStrategy, LDYZeroPage, 0xA4>
{
public:
	LDYZeroPage() : LDYBase(3)
	{
	}
};

class LDYZeroPageX : public LDYBase<ZeroPageXAddressingStrategy, LDYZeroPageX, 0xB4>
{
public:
	LDYZeroPageX() : LDYBase(4)
	{
	}
};

#endif //NES_LDY_H
