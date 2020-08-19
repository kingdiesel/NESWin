//
//

#ifndef NES_RTS_H
#define NES_RTS_H

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"

class RTS : public BaseInstruction<ImpliedAddressingStrategy, RTS, 0x60>
{
public:
	RTS() : BaseInstruction(6, "RTS")
	{
		m_increments_program_counter = false;
	}

	void ExecuteImplementation(CPU &cpu)
	{
		cpu.IncrementStackPointer();
		uint16_t low_byte = cpu.GetMemory().GetByte(cpu.GetFullStackAddress());
		cpu.IncrementStackPointer();
		uint16_t high_byte = cpu.GetMemory().GetByte(cpu.GetFullStackAddress());
		uint16_t return_address = ((high_byte << 8) | low_byte) + 1;
		cpu.SetRegisterProgramCounter(return_address);
	}
};

#endif //NES_RTS_H
