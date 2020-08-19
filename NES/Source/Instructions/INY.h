//
//

#ifndef NES_INY_H
#define NES_INY_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

class INY : public BaseInstruction<ImpliedAddressingStrategy, INY, 0xC8>
{
public:
	INY() : BaseInstruction(2, "INY")
	{
	}

	void ExecuteImplementation(CPU &cpu)
	{
		cpu.SetRegisterY(cpu.GetRegisterY() + (uint8_t) 1);
		cpu.SetZeroFlag(cpu.GetRegisterY() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterY());
	}
};

#endif //NES_INY_H
