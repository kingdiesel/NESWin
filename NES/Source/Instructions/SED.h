#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#SED
template<class _addressing_strategy>
class SEDBase : public OpCodeBase<_addressing_strategy>
{
public:
	SEDBase() : OpCodeBase<_addressing_strategy>("SED")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		cpu.SetDecimalFlag(true);
	}
};

typedef BaseInstruction2<SEDBase<ImpliedAddressingStrategy>, 0xF8, 2> SED;
