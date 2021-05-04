#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#PHA
template<class _addressing_strategy>
class PHABase : public OpCodeBase<_addressing_strategy>
{
public:
	PHABase() : OpCodeBase<_addressing_strategy>("PHA")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		memory.CPUWriteByte(cpu.GetFullStackAddress(), cpu.GetRegisterA());
		cpu.DecrementStackPointer();
	}
};

typedef BaseInstruction<PHABase<ImpliedAddressingStrategy>, 0x48, 3> PHA;
