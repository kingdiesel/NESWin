#pragma once
#include "BaseInstruction.h"
// http://www.obelisk.me.uk/6502/reference.html#EOR
template<class _addressing_strategy>
class EORBase : public OpCodeBase<_addressing_strategy>
{
public:
	EORBase() : OpCodeBase<_addressing_strategy>("EOR")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue();
		cpu.SetRegisterA(value ^ cpu.GetRegisterA());
		cpu.SetZeroFlag(cpu.GetRegisterA() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterA());
	}
};

typedef BaseInstruction2<EORBase<ImmediateAddressingStrategy>, 0x49, 2> EORImmediate;
typedef BaseInstruction2<EORBase<AbsoluteAddressingStrategy>, 0x4D, 4> EORAbsolute;
typedef BaseInstruction2<EORBase<AbsoluteXAddressingStrategy>, 0x5D, 4> EORAbsoluteX;
typedef BaseInstruction2<EORBase<AbsoluteYAddressingStrategy>, 0x59, 4> EORAbsoluteY;
typedef BaseInstruction2<EORBase<ZeroPageAddressingStrategy>, 0x45, 3> EORZeroPage;
typedef BaseInstruction2<EORBase<ZeroPageXAddressingStrategy>, 0x55, 4> EORZeroPageX;
typedef BaseInstruction2<EORBase<IndexedIndirectAddressingStrategy>, 0x41, 6> EORIndexedIndirect;
typedef BaseInstruction2<EORBase<IndirectIndexedAddressingStrategy>, 0x51, 5> EORIndirectIndexed;
