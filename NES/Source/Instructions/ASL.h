#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#ASL
template<class _addressing_strategy>
class ASLBase : public OpCodeBase<_addressing_strategy>
{
public:
	ASLBase() : OpCodeBase<_addressing_strategy>("ASL")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue();
		uint8_t bit_seven = value & (uint8_t)0x80;
		cpu.SetCarryFlag(bit_seven != 0);
		value = value << 1;
		value &= (uint8_t)0xFE;
		cpu.SetZeroFlag(value == 0);
		cpu.SetNegativeFlagForValue(value);
		this->GetAddressingMode().SetMemoryByteValue(value);
	}
};

typedef BaseInstruction2<ASLBase<AccumulatorAddressingStrategy>, 0x0A, 2> ASLAccumulator;
typedef BaseInstruction2<ASLBase<AbsoluteAddressingStrategy>, 0x0E, 6> ASLAbsolute;
typedef BaseInstruction2<ASLBase<AbsoluteXAddressingStrategy>, 0x1E, 7> ASLAbsoluteX;
typedef BaseInstruction2<ASLBase<ZeroPageAddressingStrategy>, 0x06, 5> ASLZeroPage;
typedef BaseInstruction2<ASLBase<ZeroPageXAddressingStrategy>, 0x16, 6> ASLZeroPageX;
