#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#TYA
template<class _addressing_strategy>
class TYABase : public OpCodeBase<_addressing_strategy>
{
public:
	TYABase() : OpCodeBase<_addressing_strategy>("TYA")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		cpu.SetRegisterA(cpu.GetRegisterY());
		cpu.SetZeroFlag(cpu.GetRegisterA() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterA());
	}
};

typedef BaseInstruction2<TYABase<ImpliedAddressingStrategy>, 0x98, 2> TYA;
