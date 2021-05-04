#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#PLA
template<class _addressing_strategy>
class PLABase : public OpCodeBase<_addressing_strategy>
{
public:
	PLABase() : OpCodeBase<_addressing_strategy>("PLA")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.IncrementStackPointer();
		uint8_t data_byte = memory.CPUReadByte(cpu.GetFullStackAddress());
		cpu.SetRegisterA(data_byte);
		cpu.SetZeroFlag(cpu.GetRegisterA() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterA());
	}
};

typedef BaseInstruction<PLABase<ImpliedAddressingStrategy>, 0x68, 4> PLA;
