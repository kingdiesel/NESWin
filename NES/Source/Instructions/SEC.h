#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#SEC
template<class _addressing_strategy>
class SECBase : public OpCodeBase<_addressing_strategy>
{
public:
	SECBase() : OpCodeBase<_addressing_strategy>("SEC")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		cpu.SetCarryFlag(true);
	}
};

typedef BaseInstruction<SECBase<ImpliedAddressingStrategy>, 0x38, 2> SEC;
