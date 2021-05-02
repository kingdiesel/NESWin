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
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.SetRegisterX(this->GetAddressingMode().GetMemoryByteValue());
		cpu.SetZeroFlag(cpu.GetRegisterX() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterX());
	}
};
typedef BaseInstruction2<LDXBase<ImmediateAddressingStrategy>, 0xA2, 2> LDXImmediate;
typedef BaseInstruction2<LDXBase<AbsoluteAddressingStrategy>, 0xAE, 4> LDXAbsolute;
typedef BaseInstruction2<LDXBase<AbsoluteYAddressingStrategy>, 0xBE, 4> LDXAbsoluteY;
typedef BaseInstruction2<LDXBase<ZeroPageAddressingStrategy>, 0xA6, 3> LDXZeroPage;
typedef BaseInstruction2<LDXBase<ZeroPageYAddressingStrategy>, 0xB6, 4> LDXZeroPageY;
