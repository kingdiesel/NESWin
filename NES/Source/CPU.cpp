#include "CPU.h"
#include "NESConsole.h"
#include "Instructions/JMP.h"
#include "Instructions/LDX.h"
#include "Instructions/STX.h"
#include "Instructions/NOP.h"
#include "Instructions/SEC.h"
#include "Instructions/BRA.h"
#include "Instructions/BRK.h"
#include "Instructions/CLC.h"
#include "Instructions/LDA.h"
#include "Instructions/STA.h"
#include "Instructions/BIT.h"
#include "Instructions/RTS.h"
#include "Instructions/SEI.h"
#include "Instructions/SED.h"
#include "Instructions/PHP.h"
#include "Instructions/PLA.h"
#include "Instructions/AND.h"
#include "Instructions/CMP.h"
#include "Instructions/CLD.h"
#include "Instructions/PHA.h"
#include "Instructions/PLP.h"
#include "Instructions/ORA.h"
#include "Instructions/CLV.h"
#include "Instructions/EOR.h"
#include "Instructions/ADC.h"
#include "Instructions/LDY.h"
#include "Instructions/SBC.h"
#include "Instructions/INY.h"
#include "Instructions/INX.h"
#include "Instructions/DEX.h"
#include "Instructions/DEY.h"
#include "Instructions/TAX.h"
#include "Instructions/TAY.h"
#include "Instructions/TYA.h"
#include "Instructions/TXA.h"
#include "Instructions/TSX.h"
#include "Instructions/TXS.h"
#include "Instructions/RTI.h"
#include "Instructions/LSR.h"
#include "Instructions/ASL.h"
#include "Instructions/ROR.h"
#include "Instructions/ROL.h"
#include "Instructions/STY.h"
#include "Instructions/INC.h"
#include "Instructions/DEC.h"
#include <iostream>
#include <cstring>
#include <assert.h>
#include <iomanip>
#include <functional>
#include <iterator>
#include <fstream>
#include <typeinfo>

class VariadicInstructionMap
{
public:
	VariadicInstructionMap() = default;

	void InitializeInstructionTypes()
	{
		InitializeArray<JMPAbsolute, JSR, LDXAbsolute, LDXImmediate, LDXAbsolute, LDXAbsoluteY, LDXZeroPage, LDXZeroPageY,
				STXZeroPage, STXAbsolute, STXZeroPageY, NOP, NOP1A, NOP3A, NOP5A, NOPDA, NOPFA,
				SEC, BCS, BPL, BNE, BEQ, BCC, BVS, BVC, BMI, BRK, CLC, LDAImmediate, LDAAbsolute,
				LDAAbsoluteX, LDAAbsoluteY, LDAZeroPage, LDAZeroPageX, LDAIndexedIndirect, LDAIndirectIndexed,
				STXAbsolute, STAAbsoluteX, STAAbsoluteY, STAZeroPage, STAZeroPageX, STAIndexedIndirect, STAIndirectIndexed, STAAbsolute,
				BITAbsolute, BITZeroPage, RTS, SEI, SED, PHP, PLA, ANDImmediate, ANDAbsolute, ANDAbsoluteX,
				ANDAbsoluteY, ANDZeroPage, ANDZeroPageX, ANDIndexedIndirect, ANDIndirectIndexed, CMPImmediate,
				CMPAbsolute, CMPAbsoluteX, CMPAbsoluteY, CMPZeroPage, CMPZeroPageX, CMPIndexedIndirect, CMPIndirectIndexed,
				CPXAbsolute, CPXImmediate, CPXZeroPage, CPYAbsolute, CPYImmediate, CPYZeroPage, CLD, PHA, PLP,
				ORAImmediate, ORAAbsolute, ORAAbsoluteX, ORAAbsoluteY, ORAZeroPage, ORAZeroPageX, ORAIndexedIndirect,
				ORAIndirectIndexed, CLV, EORAbsolute, EORAbsoluteX, EORAbsoluteY, EORImmediate, EORIndexedIndirect, EORIndirectIndexed,
				EORZeroPage, EORZeroPageX, ADCImmediate, ADCAbsolute, ADCAbsoluteX, ADCAbsoluteY, ADCZeroPage, ADCZeroPageX,
				ADCIndexedIndirect, ADCIndirectIndexed, LDYAbsolute, LDYAbsoluteX, LDYImmediate, LDYZeroPage, LDYZeroPageX,
				SBCAbsolute, SBCAbsoluteX, SBCAbsoluteY, SBCImmediate, SBCZeroPage, SBCZeroPageX, SBCIndexedIndirect, SBCIndirectIndexed,
				INY, INX, DEX, DEY, TAX, TAY, TYA, TXA, TSX, TXS, RTI, LSRAbsolute, LSRAbsoluteX, LSRAccumulator, LSRZeroPage,
				LSRZeroPageX, ASLAccumulator, ASLAbsolute, ASLAbsoluteX, ASLZeroPage, ASLZeroPageX, RORAccumulator, RORAbsolute, RORAbsoluteX,
				RORZeroPage, RORZeroPageX, ROLAccumulator, ROLAbsolute, ROLAbsoluteX, ROLZeroPage, ROLZeroPageX, STYAbsolute, STYZeroPage,
				STYZeroPageX, INCAbsolute, INCAbsoluteX, INCZeroPage, INCZeroPageX, DECAbsolute, DECAbsoluteX, DECZeroPage, DECZeroPageX,
				JMPIndirect>();
	}

	template<class _first_instruction>
	void PrintLogString()
	{
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		if (!cpu.GetLoggingEnabled())
		{
			return;
		}
		std::ofstream& log_stream = NESConsole::GetInstance()->GetLogStream();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		memory.SetReadOnlyMode(true);
		
		_first_instruction &instruction = *static_cast<_first_instruction *>(arr[_first_instruction::OP_CODE]);
		std::string &log_string = cpu.GetLastInstructionStr();
		log_string.clear();
		std::ostringstream opos;
		//	print program counter
		opos << std::hex << std::uppercase << std::right << std::setfill('0') << std::setw(4)
			 << cpu.GetRegisterProgramCounter() << "  ";
		//	print op code and operand
		for (uint16_t i = 0; i < 3; ++i)
		{
			if (i > (instruction.GetOperandLength() + instruction.GetOpCodeLength() - 1))
			{
				opos << "  ";
			}
			else
			{
				uint8_t operand_byte = memory.CPUReadByte(cpu.GetRegisterProgramCounterPlus(i));
				opos << std::hex << std::uppercase << std::right << std::setfill('0') << std::setw(2)
					 << (int) operand_byte;
			}
			opos << " ";
		}
		opos << " ";
		std::ostringstream opstream;
		opstream << std::left << std::setw(10) << opos.str();
		log_string.append(opstream.str());

		//	op code name
		std::ostringstream namestream;
		namestream << std::left << std::setw(4) << instruction.GetName();
		log_string.append(namestream.str());


		instruction.ToString(log_string);
		std::ostringstream flagstream;
		flagstream << "A:" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase
				   << (int) cpu.GetRegisterA()
				   << " ";
		flagstream << "X:" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase
				   << (int) cpu.GetRegisterX()
				   << " ";
		flagstream << "Y:" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase
				   << (int) cpu.GetRegisterY()
				   << " ";
		flagstream << "P:" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase
				   << (int) cpu.GetRegisterFlag()
				   << " ";
		flagstream << "SP:" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase
				   << (int) cpu.GetRegisterStack()
				   << " ";
		flagstream << "CYC:" << std::dec << std::setfill(' ') << std::setw(3) << std::right << cpu.GetCycles();

		log_string.append(flagstream.str());
		log_stream << cpu.GetInstructionCount() << " " << log_string << std::endl;
		memory.SetReadOnlyMode(false);
	}

	template<typename _instruction>
	void InitializeArrayEntry()
	{
		arr[_instruction::OP_CODE] = new _instruction();
		auto func = [this]()
		{
			CPU& cpu = NESConsole::GetInstance()->GetCPU();
			_instruction &instruction = *static_cast<_instruction *>(arr[_instruction::OP_CODE]);
			if (cpu.GetLoggingEnabled())
			{
				PrintLogString<_instruction>();
			}

			instruction.Execute(cpu);
			if (instruction.GetIncrementsProgramCounter())
			{
				cpu.IncrementProgramCounter(instruction.GetProgramCounterIncrement());
			}
			cpu.SetInstructionCount(cpu.GetInstructionCount() + 1);
			cpu.SetCycles(cpu.GetCycles() + instruction.GetCycles() * 3);
		};
		farr[_instruction::OP_CODE] = func;
	}

	template<class _first_instruction>
	void InitializeArray()
	{
		InitializeArrayEntry<_first_instruction>();
	}

	template<class _first_instruction, class _second_instruction, class ... Rest>
	void InitializeArray()
	{
		InitializeArrayEntry<_first_instruction>();
		InitializeArray<_second_instruction, Rest...>();
	}

	void ExecuteInstruction(uint8_t op_code)
	{
		if (farr[op_code] != nullptr)
		{
			farr[op_code]();
		}
		else
		{
			std::cout << "Unknown opcode: 0x" << std::uppercase << std::hex << std::setw(2) << std::setfill('0')
				<< (int)op_code << std::endl;
			exit(1);
		}
	}

private:
	void *arr[0xFF];
	std::function<void()> farr[0xFF];
};

VariadicInstructionMap map;

CPU::CPU(class NESConsole* console)
{
	m_console = console;
	map.InitializeInstructionTypes();
}

void CPU::PowerUp()
{
	// https://wiki.nesdev.com/w/index.php/CPU_power_up_state#At_power-up
	m_instruction_count = 0;
	m_cycles = 0;
	m_logging_enabled = false;
	m_last_instruction_str.clear();
	m_reg_x = m_reg_y = m_reg_a = 0x00;
	m_reg_s = 0xFD;
	m_reg_p.Register = 0x24;
}

void CPU::Run()
{
	//const int CYCLES_PER_SECOND = 1790000;
	//const int FPS = 60;
	//const int CYCLES_PER_FRAME = CYCLES_PER_SECOND / FPS;
	//int cycles_at_start = GetCycles();

	PPU& ppu = NESConsole::GetInstance()->GetPPU();
	//while (GetCycles() - cycles_at_start < CYCLES_PER_FRAME)
	//{
	//	
		if (ppu.GetNMIRequest())
		{
			//SetLoggingEnabled(true);
			HandleNMI();
		}
		ExecuteInstruction();
	//}
}

uint16_t CPU::GetRegisterProgramCounterPlus(const uint16_t value) const
{
	assert(value >= 0 && value <= 0xFFFF);
	return m_reg_pc + (uint16_t) value;
}

void CPU::HandleNMI()
{
	PPU& ppu = NESConsole::GetInstance()->GetPPU();
	Memory& memory = NESConsole::GetInstance()->GetMemory();
	ppu.ResetNMIRequest();

	// https://wiki.nesdev.com/w/index.php/NMI
	// When the CPU checks for interrupts and 
	// find that the flip-flop is set, it pushes 
	// the processor status register and return address 
	// on the stack, reads the NMI handler's address from 
	// $FFFA-$FFFB, clears the flip-flop, and jumps to this address.
	const uint16_t program_counter = GetRegisterProgramCounter();
	const uint8_t return_memory_high = program_counter >> 8;
	const uint8_t return_memory_low = program_counter & 0x00FF;
	const uint8_t status_register = GetRegisterP().Register;
	// similar to JMP::ExecuteImplementation
	memory.CPUWriteByte(GetFullStackAddress(), return_memory_high);
	DecrementStackPointer();
	memory.CPUWriteByte(GetFullStackAddress(), return_memory_low);
	DecrementStackPointer();
	memory.CPUWriteByte(GetFullStackAddress(), status_register);
	DecrementStackPointer();

	const uint8_t jump_address_high = memory.CPUReadByte(0xFFFB);
	const uint8_t jump_address_low = memory.CPUReadByte(0xFFFA);

	uint16_t jump_address = jump_address_high;
	jump_address = jump_address << 8;
	jump_address |= jump_address_low;

	SetRegisterProgramCounter(jump_address);
	// http://www.6502.org/tutorials/interrupts.html#1.3
	IncrementCycles(7);
}

void CPU::HandleIRQ()
{
	// https://wiki.nesdev.com/w/index.php/Status_flags#I:_Interrupt_Disable
	if (GetRegisterP().Status.m_interrupt_disable != 1)
	{
		// If the CPU's /IRQ input is 0 at the end of an instruction, 
		// then the CPU pushes the program counter and the processor 
		// status register, sets the I flag to ignore further IRQs, 
		// and the Program Counter takes the value read at $fffe and $ffff.
		// https://wiki.nesdev.com/w/index.php/IRQ
		const uint16_t program_counter = GetRegisterProgramCounter();
		const uint8_t return_memory_high = program_counter >> 8;
		const uint8_t return_memory_low = program_counter & 0x00FF;
		const uint8_t status_register = GetRegisterP().Register;
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		// similar to JMP::ExecuteImplementation
		memory.CPUWriteByte(GetFullStackAddress(), return_memory_high);
		DecrementStackPointer();
		memory.CPUWriteByte(GetFullStackAddress(), return_memory_low);
		DecrementStackPointer();
		memory.CPUWriteByte(GetFullStackAddress(), status_register);

		const uint8_t jump_address_high = memory.CPUReadByte(0xFFFF);
		const uint8_t jump_address_low = memory.CPUReadByte(0xFFFE);

		uint16_t jump_address = jump_address_high;
		jump_address = jump_address << 8;
		jump_address &= jump_address_low;

		// ignore IRQ while processing IRQ
		SetInterruptDisableFlag(true);

		SetRegisterProgramCounter(jump_address);

		// http://www.6502.org/tutorials/interrupts.html#1.3
		IncrementCycles(7);
	}
}

void CPU::HandleOpCode(const uint8_t op_code)
{
	map.ExecuteInstruction(op_code);
}
void CPU::ExecuteInstruction()
{
	uint8_t op_code = m_console->GetMemory().CPUReadByte(m_reg_pc);
	HandleOpCode(op_code);
}