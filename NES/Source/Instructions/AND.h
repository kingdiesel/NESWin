#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#AND
template<class _addressing_strategy>
class ANDBase : public OpCodeBase<_addressing_strategy>
{
public:
	ANDBase() : OpCodeBase<_addressing_strategy>("AND")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue();
		cpu.SetRegisterA(value & cpu.GetRegisterA());
		cpu.SetZeroFlag(cpu.GetRegisterA() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterA());
	}
};

typedef BaseInstruction<ANDBase<ImmediateAddressingStrategy>, 0x29, 2> ANDImmediate;
typedef BaseInstruction<ANDBase<AbsoluteAddressingStrategy>, 0x2D, 4> ANDAbsolute;
typedef BaseInstruction<ANDBase<AbsoluteXAddressingStrategy>, 0x3D, 4> ANDAbsoluteX;
typedef BaseInstruction<ANDBase<AbsoluteYAddressingStrategy>, 0x39, 4> ANDAbsoluteY;
typedef BaseInstruction<ANDBase<ZeroPageAddressingStrategy>, 0x25, 3> ANDZeroPage;
typedef BaseInstruction<ANDBase<ZeroPageXAddressingStrategy>, 0x35, 4> ANDZeroPageX;
typedef BaseInstruction<ANDBase<IndexedIndirectAddressingStrategy>, 0x21, 6> ANDIndexedIndirect;
typedef BaseInstruction<ANDBase<IndirectIndexedAddressingStrategy>, 0x31, 5> ANDIndirectIndexed;
