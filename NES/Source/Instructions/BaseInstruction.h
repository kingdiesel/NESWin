#pragma once

#include <string>
#include "../CPU.h"
#include "../Addressing/AddressingMode.h"

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

	bool GetIncrementsProgramCounter() const
	{
		return m_increments_program_counter;
	}

	bool GetHasDynamicCycles() const
	{
		return m_has_dynamic_cycles;
	}

	int GetDynamicCycles() const
	{
		assert(GetHasDynamicCycles());
		return m_dynamic_cycles;
	}

private:
	_addressing_strategy m_addressing_strategy;
	std::string m_name;
protected:
	bool m_increments_program_counter = true;
	bool m_has_dynamic_cycles = false;
	int m_dynamic_cycles = 0;
};

template<OpCodeExecuteConcept _op_code_execute, int _op_code, int _cycles>
class BaseInstruction
{
public:
	BaseInstruction()
	{
		m_console = NESConsole::GetInstance();
	}

	void Execute()
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
		return (m_execute.GetHasDynamicCycles() ? m_execute.GetDynamicCycles() : _cycles) + m_execute.GetAddressingCycles();
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
		return m_execute.GetIncrementsProgramCounter();
	}

	const _op_code_execute& GetExecute() const
	{
		return m_execute;
	}
		
	static const int OP_CODE = _op_code;

protected:
	std::shared_ptr<NESConsole> m_console;
	_op_code_execute m_execute;
};