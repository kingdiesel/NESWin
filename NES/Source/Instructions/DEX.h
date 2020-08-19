//
//

#ifndef NES_DEX_H
#define NES_DEX_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

class DEX : public BaseInstruction<ImpliedAddressingStrategy, DEX, 0xCA>
{
public:
	DEX() : BaseInstruction(2, "DEX")
	{
	}

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		cpu.SetRegisterX(cpu.GetRegisterX() - (unsigned char) 1);
		cpu.SetZeroFlag(cpu.GetRegisterX() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterX());
	}
};

#endif //NES_DEX_H
