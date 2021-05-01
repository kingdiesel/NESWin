#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#SBC
template<class _addressing_strategy>
class SBCBase : public OpCodeBase<_addressing_strategy>
{
public:
	SBCBase() : OpCodeBase<_addressing_strategy>("SBC")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue() ^ (uint8_t)0xFF;
		uint8_t carry_bit = 0x00;
		if (cpu.IsCarryFlagSet())
		{
			carry_bit = 0x01;
		}

		uint8_t eight_bit_result = cpu.GetRegisterA() + value + carry_bit;
		uint16_t full_bit_result =
			(uint16_t)cpu.GetRegisterA() + (uint16_t)value + (uint16_t)carry_bit;
		const bool overflowed = (~((uint16_t)cpu.GetRegisterA() ^ (uint16_t)value) & ((uint16_t)cpu.GetRegisterA() ^ (uint16_t)full_bit_result)) & 0x0080;

		cpu.SetRegisterA(eight_bit_result);
		cpu.SetOverflowFlag(overflowed);
		cpu.SetCarryFlag((full_bit_result & 0x100) != 0);
		cpu.SetZeroFlag(cpu.GetRegisterA() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterA());
	}
};

typedef BaseInstruction2<SBCBase<ImmediateAddressingStrategy>, 0xE9, 2> SBCImmediate;
typedef BaseInstruction2<SBCBase<AbsoluteAddressingStrategy>, 0xED, 4> SBCAbsolute;
typedef BaseInstruction2<SBCBase<AbsoluteXAddressingStrategy>, 0xFD, 4> SBCAbsoluteX;
typedef BaseInstruction2<SBCBase<AbsoluteYAddressingStrategy>, 0xF9, 4> SBCAbsoluteY;
typedef BaseInstruction2<SBCBase<ZeroPageAddressingStrategy>, 0xE5, 3> SBCZeroPage;
typedef BaseInstruction2<SBCBase<ZeroPageXAddressingStrategy>, 0xF5, 4> SBCZeroPageX;
typedef BaseInstruction2<SBCBase<IndexedIndirectAddressingStrategy>, 0xE1, 6> SBCIndexedIndirect;
typedef BaseInstruction2<SBCBase<IndirectIndexedAddressingStrategy>, 0xF1, 5> SBCIndirectIndexed;
