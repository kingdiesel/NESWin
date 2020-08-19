#ifndef NES_JMP_H
#define NES_JMP_H

#include <iomanip>
#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"



class JMPAbsolute : public BaseInstruction<JMPAbsoluteAddressingStrategy, JMPAbsolute, 0x4C>
{
public:
	JMPAbsolute() : BaseInstruction(3, "JMP")
	{
		m_increments_program_counter = false;
	}

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		cpu.SetRegisterProgramCounter(GetAddressingMode().GetMemoryByte(cpu, memory));
	}
};

class JMPIndirect : public BaseInstruction<IndirectAddressingStrategy, JMPIndirect, 0x6C>
{
public:
	JMPIndirect() : BaseInstruction(5, "JMP")
	{
		m_increments_program_counter = false;
	}

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		cpu.SetRegisterProgramCounter(GetAddressingMode().GetMemoryByteValue(cpu, memory));
	}
};

class JSR : public BaseInstruction<JMPAbsoluteAddressingStrategy, JSR, 0x20>
{
public:
	JSR() : BaseInstruction(6, "JSR")
	{
		m_increments_program_counter = false;
	}

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		unsigned short jump_address = GetAddressingMode().GetMemoryByte(cpu, memory);
		unsigned short return_memory_address = cpu.GetRegisterProgramCounterPlus(3 - 1);
		unsigned char return_memory_high = static_cast<unsigned char>((return_memory_address & 0xFF00) >> 8);
		unsigned char return_memory_low = static_cast<unsigned char>(return_memory_address & 0x00FF);
		memory.SetByte(cpu.GetFullStackAddress(), return_memory_high);
		cpu.DecrementStackPointer();
		memory.SetByte(cpu.GetFullStackAddress(), return_memory_low);
		cpu.DecrementStackPointer();
		cpu.SetRegisterProgramCounter(jump_address);
	}
};

#endif //NES_JMP_H
