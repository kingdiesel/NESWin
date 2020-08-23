//
//

#ifndef NES_STX_H
#define NES_STX_H

#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"

template<typename _addressing_mode, typename _execute, int _op_code>
class STXBase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	STXBase(uint8_t cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "STX")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		CPUMemory& memory = NESConsole::GetInstance()->GetMemory();
		this->GetAddressingMode().SetMemoryByteValue(cpu.GetRegisterX());
	}
};

class STXAbsolute : public STXBase<AbsoluteAddressingStrategy, STXAbsolute, 0x8E>
{
public:
	STXAbsolute() : STXBase(4)
	{
	}
};

class STXZeroPage : public STXBase<ZeroPageAddressingStrategy, STXZeroPage, 0x86>
{
public:
	STXZeroPage() : STXBase(3)
	{
	}
};

class STXZeroPageY : public STXBase<ZeroPageYAddressingStrategy, STXZeroPageY, 0x96>
{
public:
	STXZeroPageY() : STXBase(4)
	{
	}
};

#endif //NES_STX_H
