//
//

#ifndef NES_PHA_H
#define NES_PHA_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

class PHA : public BaseInstruction<ImpliedAddressingStrategy, PHA, 0x48>
{
public:
	PHA() : BaseInstruction(3, "PHA")
	{
	}

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		memory.SetByte(cpu.GetFullStackAddress(), cpu.GetRegisterA());
		cpu.DecrementStackPointer();
	}
};

#endif //NES_PHA_H
