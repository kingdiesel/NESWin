#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#LDA
template<class _addressing_strategy>
class LDABase : public OpCodeBase<_addressing_strategy>
{
public:
	LDABase() : OpCodeBase<_addressing_strategy>("LDA")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		cpu.SetRegisterA(this->GetAddressingMode().GetMemoryByteValue());
		cpu.SetZeroFlag(cpu.GetRegisterA() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterA());
	}
};

typedef BaseInstruction<LDABase<ImmediateAddressingStrategy>, 0xA9, 2> LDAImmediate;
typedef BaseInstruction<LDABase<AbsoluteAddressingStrategy>, 0xAD, 4> LDAAbsolute;
typedef BaseInstruction<LDABase<AbsoluteXAddressingStrategy>, 0xBD, 4> LDAAbsoluteX;
typedef BaseInstruction<LDABase<AbsoluteYAddressingStrategy>, 0xB9, 4> LDAAbsoluteY;
typedef BaseInstruction<LDABase<ZeroPageAddressingStrategy>, 0xA5, 3> LDAZeroPage;
typedef BaseInstruction<LDABase<ZeroPageXAddressingStrategy>, 0xB5, 4> LDAZeroPageX;
typedef BaseInstruction<LDABase<IndexedIndirectAddressingStrategy>, 0xA1, 6> LDAIndexedIndirect;
typedef BaseInstruction<LDABase<IndirectIndexedAddressingStrategy>, 0xB1, 5> LDAIndirectIndexed;
