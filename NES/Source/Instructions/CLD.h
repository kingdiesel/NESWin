#pragma once
#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#CLD
template<class _addressing_strategy>
class CLDBase : public OpCodeBase<_addressing_strategy>
{
public:
	CLDBase() : OpCodeBase<_addressing_strategy>("CLD")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		cpu.SetDecimalFlag(false);
	}
};

typedef BaseInstruction2<CLDBase<ImpliedAddressingStrategy>, 0xD8, 2> CLD;
