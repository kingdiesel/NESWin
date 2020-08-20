//
// Created by kingdiesel on 12/30/17.
//

#ifndef NES_CLC_H
#define NES_CLC_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

class CLC : public BaseInstruction<ImpliedAddressingStrategy, CLC, 0x18>
{
public:
	CLC() : BaseInstruction(2, "CLC")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.SetCarryFlag(false);
	}
};

#endif //NES_CLC_H
