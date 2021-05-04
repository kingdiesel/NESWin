#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#STX
template<class _addressing_strategy>
class STXBase : public OpCodeBase<_addressing_strategy>
{
public:
	STXBase() : OpCodeBase<_addressing_strategy>("STX")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		this->GetAddressingMode().SetMemoryByteValue(cpu.GetRegisterX());
	}
};

typedef BaseInstruction2<STXBase<AbsoluteAddressingStrategy>, 0x8E, 4> STXAbsolute;
typedef BaseInstruction2<STXBase<ZeroPageAddressingStrategy>, 0x86, 3> STXZeroPage;
typedef BaseInstruction2<STXBase<ZeroPageYAddressingStrategy>, 0x96, 4> STXZeroPageY;
