//
//

#ifndef NES_CLD_H
#define NES_CLD_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#CLD
class CLD : public BaseInstruction<ImpliedAddressingStrategy, CLD, 0xD8>
{
public:
	CLD() : BaseInstruction(2, "CLD")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.SetDecimalFlag(false);
	}
};

#endif //NES_CLD_H
