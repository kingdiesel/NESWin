//
//

#ifndef NES_ADC_H
#define NES_ADC_H

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"

template<typename _addressing_mode, typename _execute, int _op_code>
class ADCBase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	ADCBase(uint8_t cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "ADC")
	{
	}

	void ExecuteImplementation(CPU &cpu)
	{
		uint8_t value = static_cast<_execute *>(this)->GetAddValue(cpu, cpu.GetMemory());
		uint8_t carry_bit = 0x00;
		if (cpu.IsCarryFlagSet())
		{
			carry_bit = 0x01;
		}

		uint8_t eight_bit_result = cpu.GetRegisterA() + value + carry_bit;
		uint16_t full_bit_result =
				(uint16_t) cpu.GetRegisterA() + (uint16_t) value + (uint16_t) carry_bit;
		bool overflowed = bool((cpu.GetRegisterA() ^ eight_bit_result) & (value ^ eight_bit_result) & 0x80);

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

	uint8_t GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, cpu.GetMemory());
	}
};

class ADCAbsolute : public ADCBase<AbsoluteAddressingStrategy, ADCAbsolute, 0x6D>
{
public:
	ADCAbsolute() : ADCBase(4)
	{
	}

	uint8_t GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, cpu.GetMemory());
	}
};

class ADCAbsoluteX : public ADCBase<AbsoluteXAddressingStrategy, ADCAbsoluteX, 0x7D>
{
public:
	ADCAbsoluteX() : ADCBase(4)
	{
	}

	uint8_t GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, cpu.GetMemory());
	}
};

class ADCAbsoluteY : public ADCBase<AbsoluteYAddressingStrategy, ADCAbsoluteY, 0x79>
{
public:
	ADCAbsoluteY() : ADCBase(4)
	{
	}

	uint8_t GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, cpu.GetMemory());
	}
};

class ADCZeroPage : public ADCBase<ZeroPageAddressingStrategy, ADCZeroPage, 0x65>
{
public:
	ADCZeroPage() : ADCBase(3)
	{
	}

	uint8_t GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, cpu.GetMemory());
	}
};

class ADCZeroPageX : public ADCBase<ZeroPageXAddressingStrategy, ADCZeroPageX, 0x75>
{
public:
	ADCZeroPageX() : ADCBase(4)
	{
	}

	uint8_t GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, cpu.GetMemory());
	}
};

class ADCIndexedIndirect : public ADCBase<IndexedIndirectAddressingStrategy, ADCIndexedIndirect, 0x61>
{
public:
	ADCIndexedIndirect() : ADCBase(6)
	{
	}

	uint8_t GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, cpu.GetMemory());
	}
};

class ADCIndirectIndexed : public ADCBase<IndirectIndexedAddressingStrategy, ADCIndirectIndexed, 0x71>
{
public:
	ADCIndirectIndexed() : ADCBase(5)
	{
	}

	uint8_t GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, cpu.GetMemory());
	}
};

#endif //NES_ADC_H
