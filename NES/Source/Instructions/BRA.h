//
// Created by kingdiesel on 12/30/17.
//
#pragma once

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"

template<typename _addressing_mode, typename _execute, int _op_code>
class BRABase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	BRABase(uint8_t cycles,
			const std::string &name) :
			BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, name)
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = BaseInstruction<_addressing_mode, _execute, _op_code>::m_console->GetCPU();
		this->m_increments_program_counter = true;
		if (static_cast<_execute *>(this)->ShouldBranch())
		{
			uint16_t destination = this->GetAddressingMode().GetMemoryShortValue();
			cpu.SetRegisterProgramCounter(destination);
			this->m_increments_program_counter = false;
			this->m_cycles = 3;
		}
		else
		{
			this->m_increments_program_counter = true;
			this->m_cycles = 2;
		}

	}
};

// http://www.obelisk.me.uk/6502/reference.html#BCS
class BCS : public BRABase<RelativeAddressingStrategy, BCS, 0xB0>
{
public:
	BCS() : BRABase(2, "BCS")
	{
	}

	bool ShouldBranch() const
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		return cpu.IsCarryFlagSet();
	}
};

// http://www.obelisk.me.uk/6502/reference.html#BPL
class BPL : public BRABase<RelativeAddressingStrategy, BPL, 0x10>
{
public:
	BPL() : BRABase(2, "BPL")
	{
	}

	bool ShouldBranch() const
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		return !cpu.IsNegativeFlagSet();
	}
};

// http://www.obelisk.me.uk/6502/reference.html#BNE
class BNE : public BRABase<RelativeAddressingStrategy, BNE, 0xD0>
{
public:
	BNE() : BRABase(2, "BNE")
	{
	}

	bool ShouldBranch() const
	{
		CPU& cpu = BRABase<RelativeAddressingStrategy, BNE, 0xD0>::m_console->GetCPU();
		return !cpu.IsZeroFlagSet();
	}
};

// http://www.obelisk.me.uk/6502/reference.html#BEQ
class BEQ : public BRABase<RelativeAddressingStrategy, BEQ, 0xF0>
{
public:
	BEQ() : BRABase(2, "BEQ")
	{
	}

	bool ShouldBranch() const
	{
		CPU& cpu = BRABase<RelativeAddressingStrategy, BEQ, 0xF0>::m_console->GetCPU();
		return cpu.IsZeroFlagSet();
	}
};

// http://www.obelisk.me.uk/6502/reference.html#BCC
class BCC : public BRABase<RelativeAddressingStrategy, BCC, 0x90>
{
public:
	BCC() : BRABase(2, "BCC")
	{
	}

	bool ShouldBranch() const
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		return !cpu.IsCarryFlagSet();
	}
};

// http://www.obelisk.me.uk/6502/reference.html#BVS
class BVS : public BRABase<RelativeAddressingStrategy, BVS, 0x70>
{
public:
	BVS() : BRABase(2, "BVS")
	{
	}

	bool ShouldBranch() const
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		return cpu.IsOverflowFlagSet();
	}
};

// http://www.obelisk.me.uk/6502/reference.html#BVC
class BVC : public BRABase<RelativeAddressingStrategy, BVC, 0x50>
{
public:
	BVC() : BRABase(2, "BVC")
	{
	}

	bool ShouldBranch() const
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		return !cpu.IsOverflowFlagSet();
	}
};

// http://www.obelisk.me.uk/6502/reference.html#BMI
class BMI : public BRABase<RelativeAddressingStrategy, BMI, 0x30>
{
public:
	BMI() : BRABase(2, "BMI")
	{
	}

	bool ShouldBranch() const
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		return cpu.IsNegativeFlagSet();
	}
};
