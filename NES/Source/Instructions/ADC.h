#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#ADC
template<class _addressing_strategy>
class ADCBase : public OpCodeBase<_addressing_strategy>
{
public:
	ADCBase() : OpCodeBase<_addressing_strategy>("ADC")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue();
		uint8_t carry_bit = 0x00;
		if (cpu.IsCarryFlagSet())
		{
			carry_bit = 0x01;
		}

		uint8_t eight_bit_result = cpu.GetRegisterA() + value + carry_bit;
		uint16_t full_bit_result =
				(uint16_t) cpu.GetRegisterA() + (uint16_t) value + (uint16_t) carry_bit;
		const bool overflowed = (~((uint16_t)cpu.GetRegisterA() ^ (uint16_t)value) & ((uint16_t)cpu.GetRegisterA() ^ (uint16_t)full_bit_result)) & 0x0080;

		cpu.SetRegisterA(eight_bit_result);
		cpu.SetOverflowFlag(overflowed);
		cpu.SetCarryFlag((full_bit_result & 0x100) != 0);
		cpu.SetZeroFlag(cpu.GetRegisterA() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterA());
	}
};

typedef BaseInstruction2<ADCBase<ImmediateAddressingStrategy>, 0x69, 2> ADCImmediate;
typedef BaseInstruction2<ADCBase<AbsoluteAddressingStrategy>, 0x6D, 4> ADCAbsolute;
typedef BaseInstruction2<ADCBase<AbsoluteXAddressingStrategy>, 0x7D, 4> ADCAbsoluteX;
typedef BaseInstruction2<ADCBase<AbsoluteYAddressingStrategy>, 0x79, 4> ADCAbsoluteY;
typedef BaseInstruction2<ADCBase<ZeroPageAddressingStrategy>, 0x65, 3> ADCZeroPage;
typedef BaseInstruction2<ADCBase<ZeroPageXAddressingStrategy>, 0x75, 4> ADCZeroPageX;
typedef BaseInstruction2<ADCBase<IndexedIndirectAddressingStrategy>, 0x61, 6> ADCIndexedIndirect;
typedef BaseInstruction2<ADCBase<IndirectIndexedAddressingStrategy>, 0x71, 5> ADCIndirectIndexed;
