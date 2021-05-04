#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#TSX
template<class _addressing_strategy>
class TSXBase : public OpCodeBase<_addressing_strategy>
{
public:
	TSXBase() : OpCodeBase<_addressing_strategy>("TSX")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		cpu.SetRegisterX(cpu.GetRegisterStack());
		cpu.SetZeroFlag(cpu.GetRegisterX() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterX());
	}
};

typedef BaseInstruction<TSXBase<ImpliedAddressingStrategy>, 0xBA, 2> TSX;
