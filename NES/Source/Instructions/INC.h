#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#INC
template<class _addressing_strategy>
class INCBase : public OpCodeBase<_addressing_strategy>
{
public:
	INCBase() : OpCodeBase<_addressing_strategy>("INC")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue();
		value += (uint8_t)1;
		this->GetAddressingMode().SetMemoryByteValue(value);
		cpu.SetZeroFlag(value == 0);
		cpu.SetNegativeFlagForValue(value);
	}
};

typedef BaseInstruction2<INCBase<AbsoluteAddressingStrategy>, 0xEE, 6> INCAbsolute;
typedef BaseInstruction2<INCBase<AbsoluteXAddressingStrategy>, 0xFE, 7> INCAbsoluteX;
typedef BaseInstruction2<INCBase<ZeroPageAddressingStrategy>, 0xE6, 5> INCZeroPage;
typedef BaseInstruction2<INCBase<ZeroPageXAddressingStrategy>, 0xF6, 6> INCZeroPageX;
