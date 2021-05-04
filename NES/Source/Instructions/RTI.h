#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#RTI
template<class _addressing_strategy>
class RTIBase : public OpCodeBase<_addressing_strategy>
{
public:
	RTIBase() : OpCodeBase<_addressing_strategy>("RTI")
	{
		// TODO: should this increment the program counter?
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.IncrementStackPointer();
		uint8_t flags = memory.CPUReadByte(cpu.GetFullStackAddress());
		cpu.IncrementStackPointer();
		uint16_t low_byte = memory.CPUReadByte(cpu.GetFullStackAddress());
		cpu.IncrementStackPointer();
		uint16_t high_byte = memory.CPUReadByte(cpu.GetFullStackAddress());;
		uint16_t address = (low_byte | (high_byte << (uint16_t)8)) - (uint16_t)1;
		cpu.SetRegisterProgramCounter(address);
		flags &= 0xCF;
		flags |= 0x20;
		cpu.SetRegisterFlag(flags);
	}
};

typedef BaseInstruction2<RTIBase<ImpliedAddressingStrategy>, 0x40, 6> RTI;
