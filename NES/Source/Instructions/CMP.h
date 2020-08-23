//
//

#ifndef NES_CMP_H
#define NES_CMP_H

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"

template<typename _addressing_mode, typename _execute, int _op_code>
class CMPBase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	CMPBase(uint8_t cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "CMP")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		CPUMemory& memory = NESConsole::GetInstance()->GetMemory();
		uint8_t memory_value = this->GetAddressingMode().GetMemoryByteValue();
		uint8_t compare_value = static_cast<_execute *>(this)->GetCompareValue(cpu);
		uint8_t value = compare_value - memory_value;
		cpu.SetCarryFlag(compare_value >= memory_value);
		cpu.SetZeroFlag(value == 0);
		cpu.SetNegativeFlagForValue(value);
	}
};

class CPXImmediate : public CMPBase<ImmediateAddressingStrategy, CPXImmediate, 0xE0>
{
public:
	CPXImmediate() : CMPBase(2)
	{
		m_name = "CPX";
	}

	uint8_t GetCompareValue(CPU &cpu)
	{
		return cpu.GetRegisterX();
	}
};

class CPXZeroPage : public CMPBase<ZeroPageAddressingStrategy, CPXZeroPage, 0xE4>
{
public:
	CPXZeroPage() : CMPBase(3)
	{
		m_name = "CPX";
	}

	uint8_t GetCompareValue(CPU &cpu)
	{
		return cpu.GetRegisterX();
	}
};

class CPXAbsolute : public CMPBase<AbsoluteAddressingStrategy, CPXAbsolute, 0xEC>
{
public:
	CPXAbsolute() : CMPBase(4)
	{
		m_name = "CPX";
	}

	uint8_t GetCompareValue(CPU &cpu)
	{
		return cpu.GetRegisterX();
	}
};

class CPYImmediate : public CMPBase<ImmediateAddressingStrategy, CPYImmediate, 0xC0>
{
public:
	CPYImmediate() : CMPBase(2)
	{
		m_name = "CPY";
	}

	uint8_t GetCompareValue(CPU &cpu)
	{
		return cpu.GetRegisterY();
	}
};

class CPYZeroPage : public CMPBase<ZeroPageAddressingStrategy, CPYZeroPage, 0xC4>
{
public:
	CPYZeroPage() : CMPBase(3)
	{
		m_name = "CPY";
	}

	uint8_t GetCompareValue(CPU &cpu)
	{
		return cpu.GetRegisterY();
	}
};

class CPYAbsolute : public CMPBase<AbsoluteAddressingStrategy, CPYAbsolute, 0xCC>
{
public:
	CPYAbsolute() : CMPBase(4)
	{
		m_name = "CPY";
	}

	uint8_t GetCompareValue(CPU &cpu)
	{
		return cpu.GetRegisterY();
	}
};

class CMPImmediate : public CMPBase<ImmediateAddressingStrategy, CMPImmediate, 0xC9>
{
public:
	CMPImmediate() : CMPBase(2)
	{
	}

	uint8_t GetCompareValue(CPU &cpu)
	{
		return cpu.GetRegisterA();
	}
};

class CMPAbsolute : public CMPBase<AbsoluteAddressingStrategy, CMPAbsolute, 0xCD>
{
public:
	CMPAbsolute() : CMPBase(4)
	{
	}

	uint8_t GetCompareValue(CPU &cpu)
	{
		return cpu.GetRegisterA();
	}
};

class CMPAbsoluteX : public CMPBase<AbsoluteXAddressingStrategy, CMPAbsoluteX, 0xDD>
{
public:
	CMPAbsoluteX() : CMPBase(4)
	{
	}

	uint8_t GetCompareValue(CPU &cpu)
	{
		return cpu.GetRegisterA();
	}
};

class CMPAbsoluteY : public CMPBase<AbsoluteYAddressingStrategy, CMPAbsoluteY, 0xD9>
{
public:
	CMPAbsoluteY() : CMPBase(4)
	{
	}

	uint8_t GetCompareValue(CPU &cpu)
	{
		return cpu.GetRegisterA();
	}
};

class CMPZeroPage : public CMPBase<ZeroPageAddressingStrategy, CMPZeroPage, 0xC5>
{
public:
	CMPZeroPage() : CMPBase(3)
	{
	}

	uint8_t GetCompareValue(CPU &cpu)
	{
		return cpu.GetRegisterA();
	}
};

class CMPZeroPageX : public CMPBase<ZeroPageXAddressingStrategy, CMPZeroPageX, 0xD5>
{
public:
	CMPZeroPageX() : CMPBase(4)
	{
	}

	uint8_t GetCompareValue(CPU &cpu)
	{
		return cpu.GetRegisterA();
	}
};

class CMPIndexedIndirect : public CMPBase<IndexedIndirectAddressingStrategy, CMPIndexedIndirect, 0xC1>
{
public:
	CMPIndexedIndirect() : CMPBase(6)
	{
	}

	uint8_t GetCompareValue(CPU &cpu)
	{
		return cpu.GetRegisterA();
	}
};

class CMPIndirectIndexed : public CMPBase<IndirectIndexedAddressingStrategy, CMPIndirectIndexed, 0xD1>
{
public:
	CMPIndirectIndexed() : CMPBase(5)
	{
	}

	uint8_t GetCompareValue(CPU &cpu)
	{
		return cpu.GetRegisterA();
	}
};

#endif //NES_CMP_H
