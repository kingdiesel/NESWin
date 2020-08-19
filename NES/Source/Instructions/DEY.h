//
//

#ifndef NES_DEY_H
#define NES_DEY_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

class DEY : public BaseInstruction<ImpliedAddressingStrategy, DEY, 0x88>
{
public:
	DEY() : BaseInstruction(2, "DEY")
	{
	}

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		cpu.SetRegisterY(cpu.GetRegisterY() - (uint8_t) 1);
		cpu.SetZeroFlag(cpu.GetRegisterY() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterY());
	}
};

#endif //NES_DEY_H
