#pragma once
#include "../Addressing/AddressingMode.h"
#include "BaseInstruction.h"
// http://www.obelisk.me.uk/6502/reference.html#BRK
class BRK : public BaseInstruction<ImpliedAddressingStrategy, BRK, 0x00>
{
public:
	BRK() : BaseInstruction(7, "BRK")
	{
	}

	void ExecuteImplementation()
	{
		// https://wiki.nesdev.com/w/index.php/CPU_interrupts#Interrupt_hijacking
		CPU& cpu = NESConsole::GetInstance()->GetCPU();
		Memory& memory = NESConsole::GetInstance()->GetMemory();
		PPU& ppu = NESConsole::GetInstance()->GetPPU();

		cpu.IncrementProgramCounter(1);
		const uint16_t program_counter = cpu.GetRegisterProgramCounter();
		const uint8_t return_memory_high = program_counter >> 8;
		const uint8_t return_memory_low = program_counter & 0x00FF;
		memory.CPUWriteByte(cpu.GetFullStackAddress(), return_memory_high);
		cpu.DecrementStackPointer();
		memory.CPUWriteByte(cpu.GetFullStackAddress(), return_memory_low);
		cpu.DecrementStackPointer();
		
		const uint8_t status_register = cpu.GetRegisterP().Register | 0x30;
		memory.CPUWriteByte(cpu.GetFullStackAddress(), status_register);
		cpu.DecrementStackPointer();
		
		cpu.SetInterruptDisableFlag(true);
		
		const uint8_t jump_address_high = memory.CPUReadByte(ppu.GetNMIRequest() ? 0xFFFB : 0xFFFF);
		const uint8_t jump_address_low = memory.CPUReadByte(ppu.GetNMIRequest() ? 0xFFFA : 0xFFFE);

		uint16_t jump_address = jump_address_high;
		jump_address = jump_address << 8;
		jump_address |= jump_address_low;

		cpu.SetRegisterProgramCounter(jump_address);
	}
};