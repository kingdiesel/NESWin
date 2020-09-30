//
//

#ifndef NES_TYA_H
#define NES_TYA_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#TYA
class TYA : public BaseInstruction<ImpliedAddressingStrategy, TYA, 0x98>
{
public:
	TYA() : BaseInstruction(2, "TYA")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.SetRegisterA(cpu.GetRegisterY());
		cpu.SetZeroFlag(cpu.GetRegisterA() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterA());
	}
};

#endif //NES_TYA_H
