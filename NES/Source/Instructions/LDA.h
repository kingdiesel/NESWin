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

typedef BaseInstruction2<LDABase<ImmediateAddressingStrategy>, 0xA9, 2> LDAImmediate;
typedef BaseInstruction2<LDABase<AbsoluteAddressingStrategy>, 0xAD, 4> LDAAbsolute;
typedef BaseInstruction2<LDABase<AbsoluteXAddressingStrategy>, 0xBD, 4> LDAAbsoluteX;
typedef BaseInstruction2<LDABase<AbsoluteYAddressingStrategy>, 0xB9, 4> LDAAbsoluteY;
typedef BaseInstruction2<LDABase<ZeroPageAddressingStrategy>, 0xA5, 3> LDAZeroPage;
typedef BaseInstruction2<LDABase<ZeroPageXAddressingStrategy>, 0xB5, 4> LDAZeroPageX;
typedef BaseInstruction2<LDABase<IndexedIndirectAddressingStrategy>, 0xA1, 6> LDAIndexedIndirect;
typedef BaseInstruction2<LDABase<IndirectIndexedAddressingStrategy>, 0xB1, 5> LDAIndirectIndexed;
