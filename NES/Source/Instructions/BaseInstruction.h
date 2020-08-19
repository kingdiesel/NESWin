#ifndef NES_BASEINSTRUCTION_H
#define NES_BASEINSTRUCTION_H

#include <string>

class CPU;

class Memory;

template<class _addressing_mode, class _execute, int _op_code>
class BaseInstruction
{
public:
	BaseInstruction();

	BaseInstruction(
			unsigned char cycles,
			const std::string &name) :
			m_cycles(cycles),
			m_name(name)
	{

	}

	void Execute(CPU &cpu, Memory &memory)
	{
		static_cast<_execute *>(this)->ExecuteImplementation(cpu, memory);
	}

	void ToString(const CPU &cpu, const Memory &memory, std::string &out_string) const
	{
		m_addressing_mode.ToString(cpu, memory, out_string);
	}

	unsigned char GetOpCode() const
	{
		return _op_code;
	}

	unsigned char GetOperandLength() const
	{
		return m_addressing_mode.GetOperandLength();
	}

	unsigned char GetOpCodeLength() const
	{
		return (unsigned char) 0x01;
	}

	int GetCycles() const
	{
		return m_cycles + m_addressing_mode.GetAddressingCycles();
	}

	unsigned short GetProgramCounterIncrement() const
	{
		if (GetIncrementsProgramCounter())
		{
			return (unsigned short) GetOpCodeLength() + (unsigned short) GetOperandLength();
		}
		return (unsigned short) 0;
	}

	const _addressing_mode &GetAddressingMode() const
	{
		return m_addressing_mode;
	}

	_addressing_mode &GetAddressingMode()
	{
		return m_addressing_mode;
	}

	const std::string &GetName() const
	{
		return m_name;
	}

	bool GetIncrementsProgramCounter() const
	{
		return m_increments_program_counter;
	}

	static const int OP_CODE = _op_code;

protected:
	int m_cycles = 0x00;
	bool m_increments_program_counter = true;
	std::string m_name;
private:
	_addressing_mode m_addressing_mode;
};

#endif //NES_BASEINSTRUCTION_H
