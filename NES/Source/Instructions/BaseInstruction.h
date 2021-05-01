#pragma once

#include <string>
#include "../CPU.h"
#include "../Addressing/AddressingMode.h"

template<class _addressing_mode, class _execute, int _op_code>
class BaseInstruction
{
public:
	BaseInstruction() = delete;

	BaseInstruction(
			uint8_t cycles,
			const std::string &name) :
			m_cycles(cycles),
			m_name(name)
	{
		m_console = NESConsole::GetInstance();
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
	std::shared_ptr<NESConsole> m_console;
private:
	_addressing_mode m_addressing_mode;
};

template<class _T>
concept OpCodeExecuteConcept = requires(_T OpCodeExecute, std::string& out_string)
{
	OpCodeExecute.ExecuteImplementation();
	OpCodeExecute.ToString(out_string);
	{OpCodeExecute.GetOperandLength()} -> std::same_as<uint8_t>;
	{OpCodeExecute.GetAddressingCycles()} -> std::same_as<int>;
	{OpCodeExecute.GetName()} -> std::same_as<const std::string&>;
};

template<class _addressing_strategy>
class OpCodeBase
{
public:
	OpCodeBase() = delete;
	OpCodeBase(const std::string name) : m_name(name)
	{

	}
	const std::string& GetName() const
	{
		return m_name;
	}

	void ToString(std::string& out_string) const
	{
		m_addressing_strategy.ToString(out_string);
	}

	const _addressing_strategy& GetAddressingMode() const
	{
		return m_addressing_strategy;
	}

	_addressing_strategy& GetAddressingMode()
	{
		return m_addressing_strategy;
	}

	uint8_t GetOperandLength() const
	{
		return m_addressing_strategy.GetOperandLength();
	}

	int GetAddressingCycles() const
	{
		return m_addressing_strategy.GetAddressingCycles();
	}

private:
	_addressing_strategy m_addressing_strategy;
	std::string m_name;
};

template<OpCodeExecuteConcept _op_code_execute, int _op_code, int _cycles>
class BaseInstruction2
{
public:
	BaseInstruction2()
	{
		m_console = NESConsole::GetInstance();
	}

	void Execute(CPU& cpu)
	{
		m_execute.ExecuteImplementation();
	}

	void ToString(std::string& out_string) const
	{
		m_execute.ToString(out_string);
	}

	uint8_t GetOpCode() const
	{
		return _op_code;
	}

	uint8_t GetOperandLength() const
	{
		return m_execute.GetOperandLength();
	}

	uint8_t GetOpCodeLength() const
	{
		return (uint8_t)0x01;
	}

	int GetCycles() const
	{
		return m_cycles + m_execute.GetAddressingCycles();
	}

	uint16_t GetProgramCounterIncrement() const
	{
		if (GetIncrementsProgramCounter())
		{
			return (uint16_t)GetOpCodeLength() + (uint16_t)GetOperandLength();
		}
		return (uint16_t)0;
	}

	const std::string& GetName() const
	{
		return m_execute.GetName();
	}

	bool GetIncrementsProgramCounter() const
	{
		return m_increments_program_counter;
	}

	const _op_code_execute& GetExecute() const
	{
		return m_execute;
	}
		
	static const int OP_CODE = _op_code;

protected:
	int m_cycles = _cycles;
	bool m_increments_program_counter = true;
	std::shared_ptr<NESConsole> m_console;
	_op_code_execute m_execute;
};