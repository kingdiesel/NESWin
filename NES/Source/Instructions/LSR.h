#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#LSR
template<class _addressing_strategy>
class LSRBase : public OpCodeBase<_addressing_strategy>
{
public:
	LSRBase() : OpCodeBase<_addressing_strategy>("LSR")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue();
		uint8_t bit_zero = value & (uint8_t)0x01;
		cpu.SetCarryFlag(bit_zero != 0);
		value = value >> 1;
		// makes sure a 0 gets inserted into msb
		value &= (uint8_t)0x7F;
		cpu.SetZeroFlag(value == 0);
		cpu.SetNegativeFlagForValue(value);
		this->GetAddressingMode().SetMemoryByteValue(value);
	}
};
typedef BaseInstruction2<LSRBase<AccumulatorAddressingStrategy>, 0x4A, 2> LSRAccumulator;
typedef BaseInstruction2<LSRBase<AbsoluteAddressingStrategy>, 0x4E, 6> LSRAbsolute;
typedef BaseInstruction2<LSRBase<AbsoluteXAddressingStrategy>, 0x5E, 7> LSRAbsoluteX;
typedef BaseInstruction2<LSRBase<ZeroPageAddressingStrategy>, 0x46, 5> LSRZeroPage;
typedef BaseInstruction2<LSRBase<ZeroPageXAddressingStrategy>, 0x56, 6> LSRZeroPageX;
