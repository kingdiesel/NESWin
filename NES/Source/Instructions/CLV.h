#pragma once
#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#CLV
template<class _addressing_strategy>
class CLVBase : public OpCodeBase<_addressing_strategy>
{
public:
	CLVBase() : OpCodeBase<_addressing_strategy>("CLV")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		cpu.SetOverflowFlag(false);
	}
};

typedef BaseInstruction2<CLVBase<ImpliedAddressingStrategy>, 0xB8, 2> CLV;
