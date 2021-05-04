#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#SEI
template<class _addressing_strategy>
class SEIBase : public OpCodeBase<_addressing_strategy>
{
public:
	SEIBase() : OpCodeBase<_addressing_strategy>("SEI")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		cpu.SetInterruptDisableFlag(true);
	}
};

typedef BaseInstruction<SEIBase<ImpliedAddressingStrategy>, 0x78, 2> SEI;
