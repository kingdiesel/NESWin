//
//

#ifndef NES_TXS_H
#define NES_TXS_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#TXS
class TXS : public BaseInstruction<ImpliedAddressingStrategy, TXS, 0x9A>
{
public:
	TXS() : BaseInstruction(2, "TXS")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.SetRegisterStack(cpu.GetRegisterX());
	}
};

#endif //NES_TXS_H
