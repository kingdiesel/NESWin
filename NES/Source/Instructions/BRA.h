#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#BCS
template<class _addressing_strategy>
class BCSBase : public OpCodeBase<_addressing_strategy>
{
public:
	BCSBase() : OpCodeBase<_addressing_strategy>("BCS")
	{
		this->m_has_dynamic_cycles = true;
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		this->m_increments_program_counter = true;
		if (ShouldBranch())
		{
			uint16_t destination = this->GetAddressingMode().GetMemoryShortValue();
			cpu.SetRegisterProgramCounter(destination);
			this->m_increments_program_counter = false;
			this->m_dynamic_cycles = 3;
		}
		else
		{
			this->m_increments_program_counter = true;
			this->m_dynamic_cycles = 2;
		}
	}

	bool ShouldBranch() const
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		return cpu.IsCarryFlagSet();
	}
};


// http://www.obelisk.me.uk/6502/reference.html#BPL
template<class _addressing_strategy>
class BPLBase : public OpCodeBase<_addressing_strategy>
{
public:
	BPLBase() : OpCodeBase<_addressing_strategy>("BPL")
	{
		this->m_has_dynamic_cycles = true;
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		this->m_increments_program_counter = true;
		if (ShouldBranch())
		{
			uint16_t destination = this->GetAddressingMode().GetMemoryShortValue();
			cpu.SetRegisterProgramCounter(destination);
			this->m_increments_program_counter = false;
			this->m_dynamic_cycles = 3;
		}
		else
		{
			this->m_increments_program_counter = true;
			this->m_dynamic_cycles = 2;
		}
	}

	bool ShouldBranch() const
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		return !cpu.IsNegativeFlagSet();
	}
};

// http://www.obelisk.me.uk/6502/reference.html#BNE
template<class _addressing_strategy>
class BNEBase : public OpCodeBase<_addressing_strategy>
{
public:
	BNEBase() : OpCodeBase<_addressing_strategy>("BNE")
	{
		this->m_has_dynamic_cycles = true;
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		this->m_increments_program_counter = true;
		if (ShouldBranch())
		{
			uint16_t destination = this->GetAddressingMode().GetMemoryShortValue();
			cpu.SetRegisterProgramCounter(destination);
			this->m_increments_program_counter = false;
			this->m_dynamic_cycles = 3;
		}
		else
		{
			this->m_increments_program_counter = true;
			this->m_dynamic_cycles = 2;
		}
	}

	bool ShouldBranch() const
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		return !cpu.IsZeroFlagSet();
	}
};

// http://www.obelisk.me.uk/6502/reference.html#BEQ
template<class _addressing_strategy>
class BEQBase : public OpCodeBase<_addressing_strategy>
{
public:
	BEQBase() : OpCodeBase<_addressing_strategy>("BEQ")
	{
		this->m_has_dynamic_cycles = true;
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		this->m_increments_program_counter = true;
		if (ShouldBranch())
		{
			uint16_t destination = this->GetAddressingMode().GetMemoryShortValue();
			cpu.SetRegisterProgramCounter(destination);
			this->m_increments_program_counter = false;
			this->m_dynamic_cycles = 3;
		}
		else
		{
			this->m_increments_program_counter = true;
			this->m_dynamic_cycles = 2;
		}
	}

	bool ShouldBranch() const
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		return cpu.IsZeroFlagSet();
	}
};


// http://www.obelisk.me.uk/6502/reference.html#BCC
template<class _addressing_strategy>
class BCCBase : public OpCodeBase<_addressing_strategy>
{
public:
	BCCBase() : OpCodeBase<_addressing_strategy>("BCC")
	{
		this->m_has_dynamic_cycles = true;
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		this->m_increments_program_counter = true;
		if (ShouldBranch())
		{
			uint16_t destination = this->GetAddressingMode().GetMemoryShortValue();
			cpu.SetRegisterProgramCounter(destination);
			this->m_increments_program_counter = false;
			this->m_dynamic_cycles = 3;
		}
		else
		{
			this->m_increments_program_counter = true;
			this->m_dynamic_cycles = 2;
		}
	}

	bool ShouldBranch() const
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		return !cpu.IsCarryFlagSet();
	}
};

// http://www.obelisk.me.uk/6502/reference.html#BVS
template<class _addressing_strategy>
class BVSBase : public OpCodeBase<_addressing_strategy>
{
public:
	BVSBase() : OpCodeBase<_addressing_strategy>("BVS")
	{
		this->m_has_dynamic_cycles = true;
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		this->m_increments_program_counter = true;
		if (ShouldBranch())
		{
			uint16_t destination = this->GetAddressingMode().GetMemoryShortValue();
			cpu.SetRegisterProgramCounter(destination);
			this->m_increments_program_counter = false;
			this->m_dynamic_cycles = 3;
		}
		else
		{
			this->m_increments_program_counter = true;
			this->m_dynamic_cycles = 2;
		}
	}

	bool ShouldBranch() const
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		return cpu.IsOverflowFlagSet();
	}
};

// http://www.obelisk.me.uk/6502/reference.html#BVC
template<class _addressing_strategy>
class BVCBase : public OpCodeBase<_addressing_strategy>
{
public:
	BVCBase() : OpCodeBase<_addressing_strategy>("BVC")
	{
		this->m_has_dynamic_cycles = true;
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		this->m_increments_program_counter = true;
		if (ShouldBranch())
		{
			uint16_t destination = this->GetAddressingMode().GetMemoryShortValue();
			cpu.SetRegisterProgramCounter(destination);
			this->m_increments_program_counter = false;
			this->m_dynamic_cycles = 3;
		}
		else
		{
			this->m_increments_program_counter = true;
			this->m_dynamic_cycles = 2;
		}
	}

	bool ShouldBranch() const
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		return !cpu.IsOverflowFlagSet();
	}
};

// http://www.obelisk.me.uk/6502/reference.html#BMI
template<class _addressing_strategy>
class BMIBase : public OpCodeBase<_addressing_strategy>
{
public:
	BMIBase() : OpCodeBase<_addressing_strategy>("BMI")
	{
		this->m_has_dynamic_cycles = true;
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		this->m_increments_program_counter = true;
		if (ShouldBranch())
		{
			uint16_t destination = this->GetAddressingMode().GetMemoryShortValue();
			cpu.SetRegisterProgramCounter(destination);
			this->m_increments_program_counter = false;
			this->m_dynamic_cycles = 3;
		}
		else
		{
			this->m_increments_program_counter = true;
			this->m_dynamic_cycles = 2;
		}
	}

	bool ShouldBranch() const
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		return cpu.IsNegativeFlagSet();
	}
};

typedef BaseInstruction2<BVCBase<RelativeAddressingStrategy>, 0x50, 2> BVC;
typedef BaseInstruction2<BMIBase<RelativeAddressingStrategy>, 0x30, 2> BMI;
typedef BaseInstruction2<BCSBase<RelativeAddressingStrategy>, 0xB0, 2> BCS;
typedef BaseInstruction2<BPLBase<RelativeAddressingStrategy>, 0x10, 2> BPL;
typedef BaseInstruction2<BNEBase<RelativeAddressingStrategy>, 0xD0, 2> BNE;
typedef BaseInstruction2<BEQBase<RelativeAddressingStrategy>, 0xF0, 2> BEQ;
typedef BaseInstruction2<BCCBase<RelativeAddressingStrategy>, 0x90, 2> BCC;
typedef BaseInstruction2<BVSBase<RelativeAddressingStrategy>, 0x70, 2> BVS;