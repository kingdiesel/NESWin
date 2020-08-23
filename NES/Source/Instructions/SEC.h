//
// Created by kingdiesel on 12/30/17.
//

#ifndef NES_SEC_H
#define NES_SEC_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

class SEC : public BaseInstruction<ImpliedAddressingStrategy, SEC, 0x38>
{
public:
	SEC() : BaseInstruction(2, "SEC")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		CPUMemory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.SetCarryFlag(true);
	}
};

#endif //NES_SEC_H
