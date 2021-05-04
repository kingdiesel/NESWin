#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#PHP
template<class _addressing_strategy>
class PHPBase : public OpCodeBase<_addressing_strategy>
{
public:
	PHPBase() : OpCodeBase<_addressing_strategy>("PHP")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		uint8_t flags = cpu.GetRegisterFlag();
		flags |= 0x30;
		memory.CPUWriteByte(cpu.GetFullStackAddress(), flags);
		cpu.DecrementStackPointer();
	}
};

typedef BaseInstruction<PHPBase<ImpliedAddressingStrategy>, 0x08, 3> PHP;
