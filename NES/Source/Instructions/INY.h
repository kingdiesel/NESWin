#pragma once

#include "BaseInstruction.h"

template<class _addressing_strategy>
class INYBase : public OpCodeBase<_addressing_strategy>
{
public:
	INYBase() : OpCodeBase<_addressing_strategy>("INY")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		cpu.SetRegisterY(cpu.GetRegisterY() + (uint8_t)1);
		cpu.SetZeroFlag(cpu.GetRegisterY() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterY());
	}
};

typedef BaseInstruction<INYBase<ImpliedAddressingStrategy>, 0xC8, 2> INY;
