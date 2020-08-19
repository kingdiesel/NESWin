//
//

#ifndef NES_PHP_H
#define NES_PHP_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

class PHP : public BaseInstruction<ImpliedAddressingStrategy, PHP, 0x08>
{
public:
	PHP() : BaseInstruction(3, "PHP")
	{
	}

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		unsigned char flags = cpu.GetRegisterFlag();
		flags |= 0x30;
		memory.SetByte(cpu.GetFullStackAddress(), flags);
		cpu.DecrementStackPointer();
	}
};

#endif //NES_PHP_H
