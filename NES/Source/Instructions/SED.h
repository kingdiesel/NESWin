//
//

#ifndef NES_SED_H
#define NES_SED_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#SED
class SED : public BaseInstruction<ImpliedAddressingStrategy, SED, 0xF8>
{
public:
	SED() : BaseInstruction(2, "SED")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.SetDecimalFlag(true);
	}
};

#endif //NES_SED_H
