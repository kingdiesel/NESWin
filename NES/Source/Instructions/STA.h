#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#STA
template<class _addressing_strategy>
class STABase : public OpCodeBase<_addressing_strategy>
{
public:
	STABase() : OpCodeBase<_addressing_strategy>("STA")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		this->GetAddressingMode().SetMemoryByteValue(cpu.GetRegisterA());
	}
};

typedef BaseInstruction<STABase<AbsoluteAddressingStrategy>, 0x8D, 4> STAAbsolute;
typedef BaseInstruction<STABase<AbsoluteXAddressingStrategy>, 0x9D, 5> STAAbsoluteX;
typedef BaseInstruction<STABase<AbsoluteYAddressingStrategy>, 0x99, 5> STAAbsoluteY;
typedef BaseInstruction<STABase<ZeroPageAddressingStrategy>, 0x85, 3> STAZeroPage;
typedef BaseInstruction<STABase<ZeroPageXAddressingStrategy>, 0x95, 4> STAZeroPageX;
typedef BaseInstruction<STABase<IndexedIndirectAddressingStrategy>, 0x81, 6> STAIndexedIndirect;
typedef BaseInstruction<STABase<IndirectIndexedAddressingStrategy>, 0x91, 6> STAIndirectIndexed;
