//
//

#ifndef NES_SEI_H
#define NES_SEI_H

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"

class SEI : public BaseInstruction<ImpliedAddressingStrategy, SEI, 0x78>
{
public:
	SEI() : BaseInstruction(2, "SEI")
	{
	}

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		cpu.SetInterruptFlag(true);
	}
};

#endif //NES_SEI_H
