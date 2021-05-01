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
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue();
		cpu.SetRegisterA(value & cpu.GetRegisterA());
		cpu.SetZeroFlag(cpu.GetRegisterA() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterA());
	}
};

typedef BaseInstruction2<ANDBase<ImmediateAddressingStrategy>, 0x29, 2> ANDImmediate;
typedef BaseInstruction2<ANDBase<AbsoluteAddressingStrategy>, 0x2D, 4> ANDAbsolute;
typedef BaseInstruction2<ANDBase<AbsoluteXAddressingStrategy>, 0x3D, 4> ANDAbsoluteX;
typedef BaseInstruction2<ANDBase<AbsoluteYAddressingStrategy>, 0x39, 4> ANDAbsoluteY;
typedef BaseInstruction2<ANDBase<ZeroPageAddressingStrategy>, 0x25, 3> ANDZeroPage;
typedef BaseInstruction2<ANDBase<ZeroPageXAddressingStrategy>, 0x35, 4> ANDZeroPageX;
typedef BaseInstruction2<ANDBase<IndexedIndirectAddressingStrategy>, 0x21, 6> ANDIndexedIndirect;
typedef BaseInstruction2<ANDBase<IndirectIndexedAddressingStrategy>, 0x31, 5> ANDIndirectIndexed;
