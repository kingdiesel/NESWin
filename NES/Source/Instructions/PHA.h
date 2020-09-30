//
//

#ifndef NES_PHA_H
#define NES_PHA_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#PHA
class PHA : public BaseInstruction<ImpliedAddressingStrategy, PHA, 0x48>
{
public:
	PHA() : BaseInstruction(3, "PHA")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		memory.CPUWriteByte(cpu.GetFullStackAddress(), cpu.GetRegisterA());
		cpu.DecrementStackPointer();
	}
};

#endif //NES_PHA_H
