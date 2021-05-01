#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#JMP
template<class _addressing_strategy>
class JMPAbsoluteBase : public OpCodeBase<_addressing_strategy>
{
public:
	JMPAbsoluteBase() : OpCodeBase<_addressing_strategy>("JMP")
	{
		this->m_increments_program_counter = false;
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		cpu.SetRegisterProgramCounter(this->GetAddressingMode().GetMemoryShort());
	}
};

template<class _addressing_strategy>
class JMPIndirectBase : public OpCodeBase<_addressing_strategy>
{
public:
	JMPIndirectBase() : OpCodeBase<_addressing_strategy>("JMP")
	{
		this->m_increments_program_counter = false;
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		cpu.SetRegisterProgramCounter(this->GetAddressingMode().GetMemoryShortValue());
	}
};

// http://www.obelisk.me.uk/6502/reference.html#JSR
template<class _addressing_strategy>
class JSRBase : public OpCodeBase<_addressing_strategy>
{
public:
	JSRBase() : OpCodeBase<_addressing_strategy>("JSR")
	{
		this->m_increments_program_counter = false;
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		uint16_t jump_address = this->GetAddressingMode().GetMemoryShort();
		uint16_t return_memory_address = cpu.GetRegisterProgramCounterPlus(3 - 1);
		uint8_t return_memory_high = static_cast<uint8_t>((return_memory_address & 0xFF00) >> 8);
		uint8_t return_memory_low = static_cast<uint8_t>(return_memory_address & 0x00FF);
		memory.CPUWriteByte(cpu.GetFullStackAddress(), return_memory_high);
		cpu.DecrementStackPointer();
		memory.CPUWriteByte(cpu.GetFullStackAddress(), return_memory_low);
		cpu.DecrementStackPointer();
		cpu.SetRegisterProgramCounter(jump_address);
	}
};

typedef BaseInstruction2<JMPAbsoluteBase<JMPAbsoluteAddressingStrategy>, 0x4C, 3> JMPAbsolute;
typedef BaseInstruction2<JMPIndirectBase<IndirectAddressingStrategy>, 0x6C, 5> JMPIndirect;
typedef BaseInstruction2<JSRBase<JMPAbsoluteAddressingStrategy>, 0x20, 6> JSR;
