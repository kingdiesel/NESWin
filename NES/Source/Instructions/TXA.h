//
//

#ifndef NES_TXA_H
#define NES_TXA_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#TXA
class TXA : public BaseInstruction<ImpliedAddressingStrategy, TXA, 0x8A>
{
public:
	TXA() : BaseInstruction(2, "TXA")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.SetRegisterA(cpu.GetRegisterX());
		cpu.SetZeroFlag(cpu.GetRegisterA() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterA());
	}
};

#endif //NES_TXA_H
