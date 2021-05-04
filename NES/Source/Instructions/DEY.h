#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#DEY
template<class _addressing_strategy>
class DEYBase : public OpCodeBase<_addressing_strategy>
{
public:
	DEYBase() : OpCodeBase<_addressing_strategy>("DEY")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		uint8_t value = cpu.GetRegisterY();
		value -= (uint8_t)1;
		cpu.SetRegisterY(value);
		cpu.SetZeroFlag(value == 0);
		cpu.SetNegativeFlagForValue(value);
	}
};

typedef BaseInstruction<DEYBase<ImpliedAddressingStrategy>, 0x88, 2> DEY;
