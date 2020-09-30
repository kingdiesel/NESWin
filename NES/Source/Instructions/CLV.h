//
//

#ifndef NES_CLV_H
#define NES_CLV_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#CLV
class CLV : public BaseInstruction<ImpliedAddressingStrategy, CLV, 0xB8>
{
public:
	CLV() : BaseInstruction(2, "CLV")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.SetOverflowFlag(false);
	}
};

#endif //NES_CLV_H
