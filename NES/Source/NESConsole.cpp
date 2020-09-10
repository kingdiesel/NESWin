#include <iostream>
#include <fstream>
#include <chrono>
#include <cstring>
#include <assert.h>
#include "NESConsole.h"
std::shared_ptr<NESConsole> NESConsole::NESConsoleSingleton(nullptr);
std::once_flag NESConsole::NESConsoleOnceFlag;

std::shared_ptr<NESConsole> NESConsole::GetInstance(void)
{
	std::call_once(NESConsoleOnceFlag, []() { NESConsoleSingleton.reset(new NESConsole); });
	return NESConsoleSingleton;
}

void NESConsole::LoadROM(const std::string &path)
{
	m_memory.GetROM().Load(path);
	m_rom_loaded = true;
}

void NESConsole::Initialize()
{
	m_cpu = new CPU(this);
}

void NESConsole::PowerUp()
{
	GetCPU().PowerUp();
	GetPPU().PowerUp();
	GetCPU().SetRegisterProgramCounter(GetMemory().CPUReadShort(0xFFFC));
	//GetCPU().SetLoggingEnabled(true);
}

void NESConsole::Run()
{
	int cycles = 0;
	const int current_cpu_cycles = GetCPU().GetCycles();
	int achieved_cycles = 0;
	while (!GetPPU().GetFrameReady())
	{
		// https://wiki.nesdev.com/w/index.php/PPU_frame_timing#CPU-PPU_Clock_Alignment
		// The NTSC PPU runs at 3 times the CPU clock rate 
		if (cycles % 3 == 0)
		{
			GetCPU().Run();
			achieved_cycles = GetCPU().GetCycles() - current_cpu_cycles;
		}
		GetPPU().Run();
		cycles++;
	}
	//std::cout << achieved_cycles << std::endl;
	GetPPU().ResetFrameReady();
}

const ROM &NESConsole::GetROM() const
{
	return m_memory.GetROM();
}

void NESConsole::RunNesTestTiming()
{
	LoadROM("C:/Users/aspiv/source/repos/NES/NES/TestRoms/nestest.nes");
	m_cpu->PowerUp();
	m_cpu->SetRegisterProgramCounter(0xC000);
	m_cpu->SetLoggingEnabled(false);
	auto t1 = std::chrono::high_resolution_clock::now();
	int times = 1000;
	int cycles = 0;
	while (times != 0)
	{
		m_cpu->ExecuteInstruction();
		if (m_cpu->GetInstructionCount() == 5003)
		{
			cycles += m_cpu->GetCycles();
			m_cpu->PowerUp();
			m_cpu->SetRegisterProgramCounter(0xC000);
			times--;
		}
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	const int elapsed_seconds = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count());

	std::cout << "NESTEST " << cycles / elapsed_seconds << " cycles per second" << std::endl;
}

bool NESConsole::RunNesTest()
{
	std::ifstream myfile("C:/Users/aspiv/source/repos/NES/NES/TestRoms/nestest_out.txt");
	std::string line;
	while (std::getline(myfile, line))
	{
		m_nestest_log.push_back(line);
	}

	LoadROM("C:/Users/aspiv/source/repos/NES/NES/TestRoms/nestest.nes");
	m_cpu->PowerUp();
	// Start execution at $C000 to run "all tests", these will generate log output
	// that will be compared to a working emulator
	m_cpu->SetRegisterProgramCounter(0xC000);
	m_cpu->SetLoggingEnabled(true);
	while (true)
	{
		std::size_t pos = m_nestest_log[m_cpu->GetInstructionCount()].find("CYC:") + 4;
		const int nestest_cycles = std::stoi(m_nestest_log[m_cpu->GetInstructionCount()].substr(pos));
		if (m_cpu->GetCycles() - nestest_cycles > 100)
		{
			m_cpu->SetCycles(nestest_cycles);
		}
		m_cpu->ExecuteInstruction();

		const std::string &log_string = m_cpu->GetLastInstructionStr();
		if (log_string.compare(m_nestest_log[m_cpu->GetInstructionCount() - 1]) != 0)
		{
			std::cout << m_cpu->GetInstructionCount() - 1 << " " << log_string << std::endl;
			std::cout << m_cpu->GetInstructionCount() - 1 << " " << m_nestest_log[m_cpu->GetInstructionCount() - 1]
					  << std::endl;
			std::cout << " " << "mismatch" << std::endl;
			return false;
		}
		else
		{
			//std::cout << m_cpu.GetInstructionCount() - 1 << " " << log_string << std::endl;
		}

		if (m_cpu->GetInstructionCount() == 5003)
		{
			return true;
		}
	}
}