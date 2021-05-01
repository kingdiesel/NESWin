//
//

#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#BIT
template<class _addressing_strategy>
class BITBase : public OpCodeBase<_addressing_strategy>
{
public:
	BITBase() : OpCodeBase<_addressing_strategy>("BIT")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue();
		uint8_t result = cpu.GetRegisterA() & value;
		cpu.SetZeroFlag(result == 0);
		cpu.SetNegativeFlagForValue(value & (1 << 7));
		cpu.SetOverflowFlag(value & (1 << 6));
	}
};

typedef BaseInstruction2<BITBase<AbsoluteAddressingStrategy>, 0x2C, 4> BITAbsolute;
typedef BaseInstruction2<BITBase<ZeroPageAddressingStrategy>, 0x24, 3> BITZeroPage;
