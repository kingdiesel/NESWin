//
//

#ifndef NES_SEI_H
#define NES_SEI_H

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"

// http://www.obelisk.me.uk/6502/reference.html#SEI
class SEI : public BaseInstruction<ImpliedAddressingStrategy, SEI, 0x78>
{
public:
	SEI() : BaseInstruction(2, "SEI")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.SetInterruptDisableFlag(true);
	}
};

#endif //NES_SEI_H
