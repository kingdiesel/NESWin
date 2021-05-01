#pragma once

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"

template<typename _addressing_mode, typename _execute, int _op_code>
class ADCBase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	ADCBase(uint8_t cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "ADC")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		uint8_t value = static_cast<_execute*>(this)->GetAddValue();
		uint8_t carry_bit = 0x00;
		if (cpu.IsCarryFlagSet())
		{
			carry_bit = 0x01;
		}

		uint8_t eight_bit_result = cpu.GetRegisterA() + value + carry_bit;
		uint16_t full_bit_result =
			(uint16_t)cpu.GetRegisterA() + (uint16_t)value + (uint16_t)carry_bit;
		const bool overflowed = (~((uint16_t)cpu.GetRegisterA() ^ (uint16_t)value) & ((uint16_t)cpu.GetRegisterA() ^ (uint16_t)full_bit_result)) & 0x0080;

		cpu.SetRegisterA(eight_bit_result);
		cpu.SetOverflowFlag(overflowed);
		cpu.SetCarryFlag((full_bit_result & 0x100) != 0);
		cpu.SetZeroFlag(cpu.GetRegisterA() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterA());
	}
};

class ADCImmediate : public ADCBase<ImmediateAddressingStrategy, ADCImmediate, 0x69>
{
public:
	ADCImmediate() : ADCBase(2)
	{
	}

	uint8_t GetAddValue()
	{
		return this->GetAddressingMode().GetMemoryByteValue();
	}
};

class ADCAbsolute : public ADCBase<AbsoluteAddressingStrategy, ADCAbsolute, 0x6D>
{
public:
	ADCAbsolute() : ADCBase(4)
	{
	}

	uint8_t GetAddValue()
	{
		return this->GetAddressingMode().GetMemoryByteValue();
	}
};

class ADCAbsoluteX : public ADCBase<AbsoluteXAddressingStrategy, ADCAbsoluteX, 0x7D>
{
public:
	ADCAbsoluteX() : ADCBase(4)
	{
	}

	uint8_t GetAddValue()
	{
		return this->GetAddressingMode().GetMemoryByteValue();
	}
};

class ADCAbsoluteY : public ADCBase<AbsoluteYAddressingStrategy, ADCAbsoluteY, 0x79>
{
public:
	ADCAbsoluteY() : ADCBase(4)
	{
	}

	uint8_t GetAddValue()
	{
		return this->GetAddressingMode().GetMemoryByteValue();
	}
};

class ADCZeroPage : public ADCBase<ZeroPageAddressingStrategy, ADCZeroPage, 0x65>
{
public:
	ADCZeroPage() : ADCBase(3)
	{
	}

	uint8_t GetAddValue()
	{
		return this->GetAddressingMode().GetMemoryByteValue();
	}
};

class ADCZeroPageX : public ADCBase<ZeroPageXAddressingStrategy, ADCZeroPageX, 0x75>
{
public:
	ADCZeroPageX() : ADCBase(4)
	{
	}

	uint8_t GetAddValue()
	{
		return this->GetAddressingMode().GetMemoryByteValue();
	}
};

class ADCIndexedIndirect : public ADCBase<IndexedIndirectAddressingStrategy, ADCIndexedIndirect, 0x61>
{
public:
	ADCIndexedIndirect() : ADCBase(6)
	{
	}

	uint8_t GetAddValue()
	{
		return this->GetAddressingMode().GetMemoryByteValue();
	}
};

class ADCIndirectIndexed : public ADCBase<IndirectIndexedAddressingStrategy, ADCIndirectIndexed, 0x71>
{
public:
	ADCIndirectIndexed() : ADCBase(5)
	{
	}

	uint8_t GetAddValue()
	{
		return this->GetAddressingMode().GetMemoryByteValue();
	}
};

// http://www.obelisk.me.uk/6502/reference.html#ADC
template<class _addressing_strategy>
class ADCBase2 : public OpCodeBase<_addressing_strategy>
{
public:
	ADCBase2() : OpCodeBase<_addressing_strategy>("ADC")
	{
	}

	void ExecuteImplementation()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		uint8_t value = this->GetAddressingMode().GetMemoryByteValue();
		uint8_t carry_bit = 0x00;
		if (cpu.IsCarryFlagSet())
		{
			carry_bit = 0x01;
		}

		uint8_t eight_bit_result = cpu.GetRegisterA() + value + carry_bit;
		uint16_t full_bit_result =
				(uint16_t) cpu.GetRegisterA() + (uint16_t) value + (uint16_t) carry_bit;
		const bool overflowed = (~((uint16_t)cpu.GetRegisterA() ^ (uint16_t)value) & ((uint16_t)cpu.GetRegisterA() ^ (uint16_t)full_bit_result)) & 0x0080;

		cpu.SetRegisterA(eight_bit_result);
		cpu.SetOverflowFlag(overflowed);
		cpu.SetCarryFlag((full_bit_result & 0x100) != 0);
		cpu.SetZeroFlag(cpu.GetRegisterA() == 0);
		cpu.SetNegativeFlagForValue(cpu.GetRegisterA());
	}
};

typedef ADCBase2<ImmediateAddressingStrategy> ADCImmediateExecute;
typedef BaseInstruction2<ADCImmediateExecute, 0x69, 2> ADCImmediate2;
//typedef ADCBase2<AbsoluteAddressingStrategy, 0x6D, 4> ADCAbsolute;
//typedef ADCBase2<AbsoluteXAddressingStrategy, 0x7D, 4> ADCAbsoluteX;
//typedef ADCBase2<AbsoluteYAddressingStrategy, 0x79, 4> ADCAbsoluteY;
//typedef ADCBase2<ZeroPageAddressingStrategy, 0x65, 3> ADCZeroPage;
//typedef ADCBase2<ZeroPageXAddressingStrategy, 0x75, 4> ADCZeroPageX;
//typedef ADCBase2<IndexedIndirectAddressingStrategy, 0x61, 6> ADCIndexedIndirect;
//typedef ADCBase2<IndirectIndexedAddressingStrategy, 0x71, 5> ADCIndirectIndexed;
