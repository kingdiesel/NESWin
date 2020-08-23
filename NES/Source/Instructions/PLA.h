//
//

#ifndef NES_PLA_H
#define NES_PLA_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

class PLA : public BaseInstruction<ImpliedAddressingStrategy, PLA, 0x68>
{
public:
	PLA() : BaseInstruction(4, "PLA")
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

#endif //NES_PLA_H
