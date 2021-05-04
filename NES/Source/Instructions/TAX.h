#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#TAX
template<class _addressing_strategy>
class TAXBase : public OpCodeBase<_addressing_strategy>
{
public:
	TAXBase() : OpCodeBase<_addressing_strategy>("TAX")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		cpu.SetRegisterX(cpu.GetRegisterA());
		cpu.SetZeroFlag(cpu.GetRegisterX() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterX());
	}
};

typedef BaseInstruction2<TAXBase<ImpliedAddressingStrategy>, 0xAA, 2> TAX;
