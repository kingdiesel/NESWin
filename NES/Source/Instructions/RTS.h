//
//

#ifndef NES_RTS_H
#define NES_RTS_H

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"

// http://www.obelisk.me.uk/6502/reference.html#RTS
class RTS : public BaseInstruction<ImpliedAddressingStrategy, RTS, 0x60>
{
public:
	RTS() : BaseInstruction(6, "RTS")
	{
		m_increments_program_counter = false;
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

#endif //NES_RTS_H
