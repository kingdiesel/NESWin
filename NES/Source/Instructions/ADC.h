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
	ADCBase(unsigned char cycles) : BaseInstruction<_addressing_mode, _execute, _op_code>(cycles, "ADC")
	{
	}

	void ExecuteImplementation(CPU &cpu, Memory &memory)
	{
		unsigned char value = static_cast<_execute *>(this)->GetAddValue(cpu, memory);
		unsigned char carry_bit = 0x00;
		if (cpu.IsCarryFlagSet())
		{
			carry_bit = 0x01;
		}

		unsigned char eight_bit_result = cpu.GetRegisterA() + value + carry_bit;
		unsigned short full_bit_result =
				(unsigned short) cpu.GetRegisterA() + (unsigned short) value + (unsigned short) carry_bit;
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

	unsigned char GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, memory);
	}
};

class ADCAbsolute : public ADCBase<AbsoluteAddressingStrategy, ADCAbsolute, 0x6D>
{
public:
	ADCAbsolute() : ADCBase(4)
	{
	}

	unsigned char GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, memory);
	}
};

class ADCAbsoluteX : public ADCBase<AbsoluteXAddressingStrategy, ADCAbsoluteX, 0x7D>
{
public:
	ADCAbsoluteX() : ADCBase(4)
	{
	}

	unsigned char GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, memory);
	}
};

class ADCAbsoluteY : public ADCBase<AbsoluteYAddressingStrategy, ADCAbsoluteY, 0x79>
{
public:
	ADCAbsoluteY() : ADCBase(4)
	{
	}

	unsigned char GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, memory);
	}
};

class ADCZeroPage : public ADCBase<ZeroPageAddressingStrategy, ADCZeroPage, 0x65>
{
public:
	ADCZeroPage() : ADCBase(3)
	{
	}

	unsigned char GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, memory);
	}
};

class ADCZeroPageX : public ADCBase<ZeroPageXAddressingStrategy, ADCZeroPageX, 0x75>
{
public:
	ADCZeroPageX() : ADCBase(4)
	{
	}

	unsigned char GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, memory);
	}
};

class ADCIndexedIndirect : public ADCBase<IndexedIndirectAddressingStrategy, ADCIndexedIndirect, 0x61>
{
public:
	ADCIndexedIndirect() : ADCBase(6)
	{
	}

	unsigned char GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, memory);
	}
};

class ADCIndirectIndexed : public ADCBase<IndirectIndexedAddressingStrategy, ADCIndirectIndexed, 0x71>
{
public:
	ADCIndirectIndexed() : ADCBase(5)
	{
	}

	unsigned char GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, memory);
	}
};

#endif //NES_ADC_H
