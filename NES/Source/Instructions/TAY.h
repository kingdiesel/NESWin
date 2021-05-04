#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#TAX
template<class _addressing_strategy>
class TAYase : public OpCodeBase<_addressing_strategy>
{
public:
	TAYase() : OpCodeBase<_addressing_strategy>("TAY")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		cpu.SetRegisterY(cpu.GetRegisterA());
		cpu.SetZeroFlag(cpu.GetRegisterY() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterY());
	}
};

typedef BaseInstruction2<TAYase<ImpliedAddressingStrategy>, 0xA8, 2> TAY;
