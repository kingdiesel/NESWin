//
//

#ifndef NES_CLV_H
#define NES_CLV_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

class CLV : public BaseInstruction<ImpliedAddressingStrategy, CLV, 0xB8>
{
public:
	CLV() : BaseInstruction(2, "CLV")
	{
	}

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		cpu.SetOverflowFlag(false);
	}
};

#endif //NES_CLV_H
