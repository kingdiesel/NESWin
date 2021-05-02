//
//

#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#LDY
template<class _addressing_strategy>
class LDYBase : public OpCodeBase<_addressing_strategy>
{
public:
	LDYBase() : OpCodeBase<_addressing_strategy>("LDY")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.SetRegisterY(this->GetAddressingMode().GetMemoryByteValue());
		cpu.SetZeroFlag(cpu.GetRegisterY() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterY());
	}
};
typedef BaseInstruction2<LDYBase<ImmediateAddressingStrategy>, 0xA0, 2> LDYImmediate;
typedef BaseInstruction2<LDYBase<AbsoluteAddressingStrategy>, 0xAC, 4> LDYAbsolute;
typedef BaseInstruction2<LDYBase<AbsoluteXAddressingStrategy>, 0xBC, 4> LDYAbsoluteX;
typedef BaseInstruction2<LDYBase<ZeroPageAddressingStrategy>, 0xA4, 3> LDYZeroPage;
typedef BaseInstruction2<LDYBase<ZeroPageXAddressingStrategy>, 0xB4, 4> LDYZeroPageX;
