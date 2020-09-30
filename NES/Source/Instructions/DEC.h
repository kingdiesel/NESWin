//
//

#ifndef NES_DEC_H
#define NES_DEC_H

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"
// http://www.obelisk.me.uk/6502/reference.html#DEC
template<typename _addressing_mode, typename _execute, int _op_code>
class DECBase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	DECBase(uint8_t cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "DEC")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue();
		value -= (uint8_t) 1;
		this->GetAddressingMode().SetMemoryByteValue(value);
		cpu.SetZeroFlag(value == 0);
		cpu.SetNegativeFlagForValue(value);
	}
};

class DECAbsolute : public DECBase<AbsoluteAddressingStrategy, DECAbsolute, 0xCE>
{
public:
	DECAbsolute() : DECBase(6)
	{
	}
};

class DECAbsoluteX : public DECBase<AbsoluteXAddressingStrategy, DECAbsoluteX, 0xDE>
{
public:
	DECAbsoluteX() : DECBase(7)
	{
	}
};

class DECZeroPage : public DECBase<ZeroPageAddressingStrategy, DECZeroPage, 0xC6>
{
public:
	DECZeroPage() : DECBase(5)
	{
	}
};

class DECZeroPageX : public DECBase<ZeroPageXAddressingStrategy, DECZeroPageX, 0xD6>
{
public:
	DECZeroPageX() : DECBase(6)
	{
	}
};

#endif //NES_DEC_H
