#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#PLP
template<class _addressing_strategy>
class PLPBase : public OpCodeBase<_addressing_strategy>
{
public:
	PLPBase() : OpCodeBase<_addressing_strategy>("PLP")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.IncrementStackPointer();
		uint8_t data_byte = memory.CPUReadByte(cpu.GetFullStackAddress());
		data_byte &= 0xCF;
		data_byte |= 0x20;
		cpu.SetRegisterFlag(data_byte);
	}
};

typedef BaseInstruction2<PLPBase<ImpliedAddressingStrategy>, 0x28, 4> PLP;
