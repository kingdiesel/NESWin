//
//

#ifndef NES_INX_H
#define NES_INX_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

class INX : public BaseInstruction<ImpliedAddressingStrategy, INX, 0xE8>
{
public:
	INX() : BaseInstruction(2, "INX")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.SetRegisterX(cpu.GetRegisterX() + (uint8_t) 1);
		cpu.SetZeroFlag(cpu.GetRegisterX() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterX());
	}
};

#endif //NES_INX_H
