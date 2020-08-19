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

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		cpu.IncrementStackPointer();
		unsigned short low_byte = memory.GetByte(cpu.GetFullStackAddress());
		cpu.IncrementStackPointer();
		unsigned short high_byte = memory.GetByte(cpu.GetFullStackAddress());
		unsigned short return_address = ((high_byte << 8) | low_byte) + 1;
		cpu.SetRegisterProgramCounter(return_address);
	}
};

#endif //NES_RTS_H
