//
//

#ifndef NES_RTI_H
#define NES_RTI_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

class RTI : public BaseInstruction<ImpliedAddressingStrategy, RTI, 0x40>
{
public:
	RTI() : BaseInstruction(6, "RTI")
	{
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
		uint16_t address = (low_byte | (high_byte << (uint16_t) 8)) - (uint16_t) 1;
		cpu.SetRegisterProgramCounter(address);
		flags &= 0xCF;
		flags |= 0x20;
		cpu.SetRegisterFlag(flags);
	}
};

#endif //NES_RTI_H
