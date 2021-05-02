#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#ORA
template<class _addressing_strategy>
class ORABase : public OpCodeBase<_addressing_strategy>
{
public:
	ORABase() : OpCodeBase<_addressing_strategy>("ORA")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue();
		cpu.SetRegisterA(value | cpu.GetRegisterA());
		cpu.SetZeroFlag(cpu.GetRegisterA() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterA());
	}
};

typedef BaseInstruction2<ORABase<ImmediateAddressingStrategy>, 0x09, 2> ORAImmediate;
typedef BaseInstruction2<ORABase<AbsoluteAddressingStrategy>, 0x0D, 4> ORAAbsolute;
typedef BaseInstruction2<ORABase<AbsoluteXAddressingStrategy>, 0x1D, 4> ORAAbsoluteX;
typedef BaseInstruction2<ORABase<AbsoluteYAddressingStrategy>, 0x19, 4> ORAAbsoluteY;
typedef BaseInstruction2<ORABase<ZeroPageAddressingStrategy>, 0x05, 3> ORAZeroPage;
typedef BaseInstruction2<ORABase<ZeroPageXAddressingStrategy>, 0x15, 4> ORAZeroPageX;
typedef BaseInstruction2<ORABase<IndexedIndirectAddressingStrategy>, 0x01, 6> ORAIndexedIndirect;
typedef BaseInstruction2<ORABase<IndirectIndexedAddressingStrategy>, 0x11, 5> ORAIndirectIndexed;
