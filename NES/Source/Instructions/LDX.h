#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#LDX
template<class _addressing_strategy>
class LDXBase : public OpCodeBase<_addressing_strategy>
{
public:
	LDXBase() : OpCodeBase<_addressing_strategy>("LDX")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		cpu.SetRegisterX(this->GetAddressingMode().GetMemoryByteValue());
		cpu.SetZeroFlag(cpu.GetRegisterX() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterX());
	}
};
typedef BaseInstruction<LDXBase<ImmediateAddressingStrategy>, 0xA2, 2> LDXImmediate;
typedef BaseInstruction<LDXBase<AbsoluteAddressingStrategy>, 0xAE, 4> LDXAbsolute;
typedef BaseInstruction<LDXBase<AbsoluteYAddressingStrategy>, 0xBE, 4> LDXAbsoluteY;
typedef BaseInstruction<LDXBase<ZeroPageAddressingStrategy>, 0xA6, 3> LDXZeroPage;
typedef BaseInstruction<LDXBase<ZeroPageYAddressingStrategy>, 0xB6, 4> LDXZeroPageY;
