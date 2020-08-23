//
//

#ifndef NES_PLP_H
#define NES_PLP_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

class PLP : public BaseInstruction<ImpliedAddressingStrategy, PLP, 0x28>
{
public:
	PLP() : BaseInstruction(4, "PLP")
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

#endif //NES_PLP_H
