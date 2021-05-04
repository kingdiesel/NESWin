#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#TXA
template<class _addressing_strategy>
class TXABase : public OpCodeBase<_addressing_strategy>
{
public:
	TXABase() : OpCodeBase<_addressing_strategy>("TXA")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.SetRegisterA(cpu.GetRegisterX());
		cpu.SetZeroFlag(cpu.GetRegisterA() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterA());
	}
};

typedef BaseInstruction2<TXABase<ImpliedAddressingStrategy>, 0x8A, 2> TXA;
