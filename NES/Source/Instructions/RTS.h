#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#RTS
template<class _addressing_strategy>
class RTSBase : public OpCodeBase<_addressing_strategy>
{
public:
	RTSBase() : OpCodeBase<_addressing_strategy>("RTS")
	{
		this->m_increments_program_counter = false;
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.IncrementStackPointer();
		uint16_t low_byte = memory.CPUReadByte(cpu.GetFullStackAddress());
		cpu.IncrementStackPointer();
		uint16_t high_byte = memory.CPUReadByte(cpu.GetFullStackAddress());
		uint16_t return_address = ((high_byte << 8) | low_byte) + 1;
		cpu.SetRegisterProgramCounter(return_address);
	}
};

typedef BaseInstruction2<RTSBase<ImpliedAddressingStrategy>, 0x60, 6> RTS;
