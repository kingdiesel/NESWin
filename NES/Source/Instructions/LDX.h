//
//

#ifndef NES_LDX_H
#define NES_LDX_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#LDX
template<typename _addressing_mode, typename _execute, int _op_code>
class LDXBase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	LDXBase(uint8_t cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "LDX")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.SetRegisterX(this->GetAddressingMode().GetMemoryByteValue());
		cpu.SetZeroFlag(cpu.GetRegisterX() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterX());
	}
};

class LDXImmediate : public LDXBase<ImmediateAddressingStrategy, LDXImmediate, 0xA2>
{
public:
	LDXImmediate() : LDXBase(2)
	{
	}
};

class LDXAbsolute : public LDXBase<AbsoluteAddressingStrategy, LDXAbsolute, 0xAE>
{
public:
	LDXAbsolute() : LDXBase(4)
	{
	}
};

class LDXAbsoluteY : public LDXBase<AbsoluteYAddressingStrategy, LDXAbsoluteY, 0xBE>
{
public:
	LDXAbsoluteY() : LDXBase(4)
	{
	}
};

class LDXZeroPage : public LDXBase<ZeroPageAddressingStrategy, LDXZeroPage, 0xA6>
{
public:
	LDXZeroPage() : LDXBase(3)
	{
	}
};

class LDXZeroPageY : public LDXBase<ZeroPageYAddressingStrategy, LDXZeroPageY, 0xB6>
{
public:
	LDXZeroPageY() : LDXBase(4)
	{
	}
};
#endif //NES_LDX_H
