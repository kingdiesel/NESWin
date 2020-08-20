//
//

#ifndef NES_TAX_H
#define NES_TAX_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

class TAX : public BaseInstruction<ImpliedAddressingStrategy, TAX, 0xAA>
{
public:
	TAX() : BaseInstruction(2, "TAX")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.SetRegisterX(cpu.GetRegisterA());
		cpu.SetZeroFlag(cpu.GetRegisterX() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterX());
	}
};

#endif //NES_TAX_H
