#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#CLC
template<class _addressing_strategy>
class CLCBase : public OpCodeBase<_addressing_strategy>
{
public:
	CLCBase() : OpCodeBase<_addressing_strategy>("CLC")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		cpu.SetCarryFlag(false);
	}
};

typedef BaseInstruction<CLCBase<ImpliedAddressingStrategy>, 0x18, 2> CLC;
