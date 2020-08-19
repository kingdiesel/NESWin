//
//

#ifndef NES_TAY_H
#define NES_TAY_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

class TAY : public BaseInstruction<ImpliedAddressingStrategy, TAY, 0xA8>
{
public:
	TAY() : BaseInstruction(2, "TAY")
	{
	}

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		cpu.SetRegisterY(cpu.GetRegisterA());
		cpu.SetZeroFlag(cpu.GetRegisterY() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterY());
	}
};

#endif //NES_TAY_H
