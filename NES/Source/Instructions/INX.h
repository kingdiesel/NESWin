#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#INX
template<class _addressing_strategy>
class INXBase : public OpCodeBase<_addressing_strategy>
{
public:
	INXBase() : OpCodeBase<_addressing_strategy>("INX")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		cpu.SetRegisterX(cpu.GetRegisterX() + (uint8_t)1);
		cpu.SetZeroFlag(cpu.GetRegisterX() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterX());
	}
};

typedef BaseInstruction<INXBase<ImpliedAddressingStrategy>, 0xE8, 2> INX;
