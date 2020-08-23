//
//

#ifndef NES_TSX_H
#define NES_TSX_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

class TSX : public BaseInstruction<ImpliedAddressingStrategy, TSX, 0xBA>
{
public:
	TSX() : BaseInstruction(2, "TSX")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.SetRegisterX(cpu.GetRegisterStack());
		cpu.SetZeroFlag(cpu.GetRegisterX() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterX());
	}
};

#endif //NES_TSX_H
