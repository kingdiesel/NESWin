#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#TXS
template<class _addressing_strategy>
class TXSBase : public OpCodeBase<_addressing_strategy>
{
public:
	TXSBase() : OpCodeBase<_addressing_strategy>("TXS")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		cpu.SetRegisterStack(cpu.GetRegisterX());
	}
};

typedef BaseInstruction2<TXSBase<ImpliedAddressingStrategy>, 0x9A, 2> TXS;
