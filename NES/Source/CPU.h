#ifndef NES_CPU_H
#define NES_CPU_H

#include <string>
#include <vector>
#include <cstdint>
class CPU
{
public:
	CPU();
	void PowerUp();

	void ExecuteInstruction();

	void SetMemory(class Memory *memory);

	void SetRegisterProgramCounter(const uint16_t counter)
	{
		m_reg_pc = counter;
	}

	void IncrementProgramCounter(const uint16_t amount)
	{
		m_reg_pc += amount;
	}

	uint8_t GetRegisterA() const
	{
		return m_reg_a;
	}

	void SetRegisterA(const uint8_t value)
	{
		m_reg_a = value;
	}

	uint8_t GetRegisterX() const
	{
		return m_reg_x;
	}

	void SetRegisterX(const uint8_t value)
	{
		m_reg_x = value;
	}

	uint8_t GetRegisterY() const
	{
		return m_reg_y;
	}

	void SetRegisterY(const uint8_t value)
	{
		m_reg_y = value;
	}

	uint8_t GetRegisterFlag() const
	{
		return m_reg_p.Register;
	}

	void SetRegisterFlag(const uint8_t value)
	{
		m_reg_p.Register = value;
	}

	uint8_t GetRegisterStack() const
	{
		return m_reg_s;
	}

	void SetRegisterStack(const uint8_t value)
	{
		m_reg_s = value;
	}

	uint16_t GetRegisterProgramCounter() const
	{
		return m_reg_pc;
	}

	void DecrementStackPointer()
	{
		m_reg_s -= 1;
	}

	void IncrementStackPointer()
	{
		m_reg_s += 1;
	}

	uint16_t GetRegisterProgramCounterPlus(const uint16_t value) const;

	uint16_t GetFullStackAddress() const
	{
		return (uint16_t) (0x0100 | (uint16_t) m_reg_s);
	}

	inline bool IsDecimalFlagSet() const
	{
		return m_reg_p.Status.m_decimal != 0;
	}

	inline bool IsZeroFlagSet() const
	{
		return m_reg_p.Status.m_zero != 0;
	}

	inline bool IsNegativeFlagSet() const
	{
		return m_reg_p.Status.m_negative != 0;
	}

	inline bool IsOverflowFlagSet() const
	{
		return m_reg_p.Status.m_overflow != 0;
	}

	inline bool IsCarryFlagSet() const
	{
		return m_reg_p.Status.m_carry != 0;
	}

	void SetNegativeFlagForValue(const uint8_t value)
	{
		m_reg_p.Status.m_negative = value & 0x80 ? 1 : 0;
	}

	void SetDecimalFlag(const bool state)
	{
		m_reg_p.Status.m_decimal = state ? 1 : 0;
	}

	void SetZeroFlag(const bool state)
	{
		m_reg_p.Status.m_zero = state ? 1 : 0;
	}

	void SetInterruptFlag(const bool state)
	{
		m_reg_p.Status.m_interrupt_disable = state ? 1 : 0;
	}

	void SetOverflowFlag(const bool state)
	{
		m_reg_p.Status.m_overflow = state ? 1 : 0;
	}

	void SetCarryFlag(const bool state)
	{
		m_reg_p.Status.m_carry = state ? 1 : 0;
	}

	class Memory *GetMemory() const
	{
		return m_memory;
	}

	int GetCycles() const
	{
		return m_cycles;
	}

	void SetCycles(int cycles)
	{
		m_cycles = cycles;
	}

	int GetInstructionCount() const
	{
		return m_instruction_count;
	}

	void SetInstructionCount(int count)
	{
		m_instruction_count = count;
	}

	const std::string &GetLastInstructionStr() const
	{
		return m_last_instruction_str;
	}

	std::string &GetLastInstructionStr()
	{
		return m_last_instruction_str;
	}

	void SetLoggingEnabled(bool enabled)
	{
		m_logging_enabled = enabled;
	}

	bool GetLoggingEnabled() const
	{
		return m_logging_enabled;
	}

private:
	void HandleOpCode(const uint8_t op_code);

	uint16_t m_reg_pc = 0x0000;
	uint8_t m_reg_x = 0x00;
	uint8_t m_reg_y = 0x00;
	uint8_t m_reg_a = 0x00;
	uint8_t m_reg_s = 0x00;
	union PRegister
	{
		struct StatusBits {
			uint8_t m_carry : 1;
			uint8_t m_zero : 1;
			uint8_t m_interrupt_disable : 1;
			uint8_t m_decimal : 1;
			uint8_t m_bflag : 2;
			uint8_t m_overflow : 1;
			uint8_t m_negative : 1;

		} Status;
		uint8_t Register = 0x00;
	};

	PRegister m_reg_p;
	class Memory *m_memory = nullptr;
	int m_instruction_count = 0;
	int m_cycles = 0;
	std::string m_last_instruction_str;
	bool m_logging_enabled = false;
};


#endif //NES_CPU_H
