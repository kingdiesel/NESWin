#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#DEX
template<class _addressing_strategy>
class DEXBase : public OpCodeBase<_addressing_strategy>
{
public:
	DEXBase() : OpCodeBase<_addressing_strategy>("DEX")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		uint8_t value = cpu.GetRegisterX();
		value -= (uint8_t)1;
		cpu.SetRegisterX(value);
		cpu.SetZeroFlag(value == 0);
		cpu.SetNegativeFlagForValue(value);
	}
};

typedef BaseInstruction2<DEXBase<ImpliedAddressingStrategy>, 0xCA, 2> DEX;
