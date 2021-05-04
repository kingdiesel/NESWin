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
		cpu.SetRegisterY(this->GetAddressingMode().GetMemoryByteValue());
		cpu.SetZeroFlag(cpu.GetRegisterY() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterY());
	}
};
typedef BaseInstruction<LDYBase<ImmediateAddressingStrategy>, 0xA0, 2> LDYImmediate;
typedef BaseInstruction<LDYBase<AbsoluteAddressingStrategy>, 0xAC, 4> LDYAbsolute;
typedef BaseInstruction<LDYBase<AbsoluteXAddressingStrategy>, 0xBC, 4> LDYAbsoluteX;
typedef BaseInstruction<LDYBase<ZeroPageAddressingStrategy>, 0xA4, 3> LDYZeroPage;
typedef BaseInstruction<LDYBase<ZeroPageXAddressingStrategy>, 0xB4, 4> LDYZeroPageX;
