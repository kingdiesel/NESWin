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

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		cpu.IncrementStackPointer();
		unsigned char flags = memory.GetByte(cpu.GetFullStackAddress());
		cpu.IncrementStackPointer();
		unsigned short low_byte = memory.GetByte(cpu.GetFullStackAddress());
		cpu.IncrementStackPointer();
		unsigned short high_byte = memory.GetByte(cpu.GetFullStackAddress());;
		unsigned short address = (low_byte | (high_byte << (unsigned short) 8)) - (unsigned short) 1;
		cpu.SetRegisterProgramCounter(address);
		flags &= 0xCF;
		flags |= 0x20;
		cpu.SetRegisterFlag(flags);
	}
};

#endif //NES_RTI_H
