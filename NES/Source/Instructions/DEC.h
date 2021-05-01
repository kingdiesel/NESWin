#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#DEC
template<class _addressing_strategy>
class DECBase : public OpCodeBase<_addressing_strategy>
{
public:
	DECBase() : OpCodeBase<_addressing_strategy>("DEC")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue();
		value -= (uint8_t)1;
		this->GetAddressingMode().SetMemoryByteValue(value);
		cpu.SetZeroFlag(value == 0);
		cpu.SetNegativeFlagForValue(value);
	}
};
typedef BaseInstruction2<DECBase<AbsoluteAddressingStrategy>, 0xCE, 6> DECAbsolute;
typedef BaseInstruction2<DECBase<AbsoluteXAddressingStrategy>, 0xDE, 7> DECAbsoluteX;
typedef BaseInstruction2<DECBase<ZeroPageAddressingStrategy>, 0xC6, 5> DECZeroPage;
typedef BaseInstruction2<DECBase<ZeroPageXAddressingStrategy>, 0xD6, 6> DECZeroPageX;
