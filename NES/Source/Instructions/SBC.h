//
//

#ifndef NES_SBC_H
#define NES_SBC_H

#include "../Addressing/AddressingMode.h"
#include "ADC.h"

class SBCImmediate : public ADCBase<ImmediateAddressingStrategy, SBCImmediate, 0xE9>
{
public:
	SBCImmediate() : ADCBase(2)
	{
		m_name = "SBC";
	}

	uint8_t GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, memory) ^ (uint8_t) 0xFF;
	}
};

class SBCAbsolute : public ADCBase<AbsoluteAddressingStrategy, SBCAbsolute, 0xED>
{
public:
	SBCAbsolute() : ADCBase(4)
	{
		m_name = "SBC";
	}

	uint8_t GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, memory) ^ (uint8_t) 0xFF;
	}
};

class SBCAbsoluteX : public ADCBase<AbsoluteXAddressingStrategy, SBCAbsoluteX, 0xFD>
{
public:
	SBCAbsoluteX() : ADCBase(4)
	{
		m_name = "SBC";
	}

	uint8_t GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, memory) ^ (uint8_t) 0xFF;
	}
};

class SBCAbsoluteY : public ADCBase<AbsoluteYAddressingStrategy, SBCAbsoluteY, 0xF9>
{
public:
	SBCAbsoluteY() : ADCBase(4)
	{
		m_name = "SBC";
	}

	uint8_t GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, memory) ^ (uint8_t) 0xFF;
	}
};

class SBCZeroPage : public ADCBase<ZeroPageAddressingStrategy, SBCZeroPage, 0xE5>
{
public:
	SBCZeroPage() : ADCBase(3)
	{
		m_name = "SBC";
	}

	uint8_t GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, memory) ^ (uint8_t) 0xFF;
	}
};

class SBCZeroPageX : public ADCBase<ZeroPageXAddressingStrategy, SBCZeroPageX, 0xF5>
{
public:
	SBCZeroPageX() : ADCBase(4)
	{
		m_name = "SBC";
	}

	uint8_t GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, memory) ^ (uint8_t) 0xFF;
	}
};

class SBCIndexedIndirect : public ADCBase<IndexedIndirectAddressingStrategy, SBCIndexedIndirect, 0xE1>
{
public:
	SBCIndexedIndirect() : ADCBase(6)
	{
		m_name = "SBC";
	}

	uint8_t GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, memory) ^ (uint8_t) 0xFF;
	}
};

class SBCIndirectIndexed : public ADCBase<IndirectIndexedAddressingStrategy, SBCIndirectIndexed, 0xF1>
{
public:
	SBCIndirectIndexed() : ADCBase(5)
	{
		m_name = "SBC";
	}

	uint8_t GetAddValue(CPU &cpu, Memory &memory)
	{
		return this->GetAddressingMode().GetMemoryByteValue(cpu, memory) ^ (uint8_t) 0xFF;
	}
};


#endif //NES_SBC_H
