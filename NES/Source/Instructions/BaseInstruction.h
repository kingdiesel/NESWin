#ifndef NES_BASEINSTRUCTION_H
#define NES_BASEINSTRUCTION_H

#include <string>
#include "../CPU.h"
template<class _addressing_mode, class _execute, int _op_code>
class BaseInstruction
{
public:
	BaseInstruction();

	BaseInstruction(
			uint8_t cycles,
			const std::string &name) :
			m_cycles(cycles),
			m_name(name)
	{
		m_console = NESConsole::GetInstance().get();
	}

	void Execute(CPU &cpu)
	{
		static_cast<_execute *>(this)->ExecuteImplementation();
	}

	void ToString(std::string &out_string) const
	{
		m_addressing_mode.ToString(out_string);
	}

	uint8_t GetOpCode() const
	{
		return _op_code;
	}

	uint8_t GetOperandLength() const
	{
		return m_addressing_mode.GetOperandLength();
	}

	uint8_t GetOpCodeLength() const
	{
		return (uint8_t) 0x01;
	}

	int GetCycles() const
	{
		return m_cycles + m_addressing_mode.GetAddressingCycles();
	}

	uint16_t GetProgramCounterIncrement() const
	{
		if (GetIncrementsProgramCounter())
		{
			return (uint16_t) GetOpCodeLength() + (uint16_t) GetOperandLength();
		}
		return (uint16_t) 0;
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
	NESConsole* m_console = nullptr;
private:
	_addressing_mode m_addressing_mode;
};

#endif //NES_BASEINSTRUCTION_H
