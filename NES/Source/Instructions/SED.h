//
//

#ifndef NES_SED_H
#define NES_SED_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

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
