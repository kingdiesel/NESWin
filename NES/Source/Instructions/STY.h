#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#STY
template<class _addressing_strategy>
class STYBase : public OpCodeBase<_addressing_strategy>
{
public:
	STYBase() : OpCodeBase<_addressing_strategy>("STY")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		this->GetAddressingMode().SetMemoryByteValue(cpu.GetRegisterY());
	}
};

typedef BaseInstruction<STYBase<AbsoluteAddressingStrategy>, 0x8C, 4> STYAbsolute;
typedef BaseInstruction<STYBase<ZeroPageAddressingStrategy>, 0x84, 3> STYZeroPage;
typedef BaseInstruction<STYBase<ZeroPageXAddressingStrategy>, 0x94, 4> STYZeroPageX;
