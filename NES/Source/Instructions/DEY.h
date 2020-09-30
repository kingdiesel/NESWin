//
//

#ifndef NES_DEY_H
#define NES_DEY_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"
// http://www.obelisk.me.uk/6502/reference.html#DEY
class DEY : public BaseInstruction<ImpliedAddressingStrategy, DEY, 0x88>
{
public:
	DEY() : BaseInstruction(2, "DEY")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		cpu.SetRegisterY(cpu.GetRegisterY() - (uint8_t) 1);
		cpu.SetZeroFlag(cpu.GetRegisterY() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterY());
	}
};

#endif //NES_DEY_H
