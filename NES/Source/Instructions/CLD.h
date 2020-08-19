//
//

#ifndef NES_CLD_H
#define NES_CLD_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

class CLD : public BaseInstruction<ImpliedAddressingStrategy, CLD, 0xD8>
{
public:
	CLD() : BaseInstruction(2, "CLD")
	{
	}

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		cpu.SetDecimalFlag(false);
	}
};

#endif //NES_CLD_H
