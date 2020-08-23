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

void NESConsole::RunROM()
{
	m_cpu.PowerUp();
	m_cpu.SetLoggingEnabled(true);
	m_cpu.SetRegisterProgramCounter(m_memory.ReadShort(0xFFFC));
	// nes test start address
	while (true)
	{
		m_cpu.ExecuteInstruction();
	}
}

const ROM &NESConsole::GetROM() const
{
	return m_memory.GetROM();
}

void NESConsole::RunTests()
{
	bool passed = RunNesTest();
	if (passed)
	{
		std::cout << "NESTEST PASSED" << std::endl;
		return;
	}
	else
	{
		std::cout << "NESTEST FAILED" << std::endl;
		exit(1);
	}

	m_memory.Reset();
	int cycles = 0;
	float nestest_time = static_cast<float>(RunNesTestTiming(1000, cycles));
	int cycles_per_second = static_cast<int>(static_cast<float>(cycles) / (nestest_time / (1000.f)));
	std::cout << "NESTEST " << cycles_per_second << " cycles per second" << std::endl;
}

int NESConsole::RunNesTestTiming(int times, int &out_cycles)
{
	LoadROM("C:/Users/aspiv/source/repos/NES/NES/TestRoms/nestest.nes");
	m_cpu.PowerUp();
	m_cpu.SetRegisterProgramCounter(0xC000);
	m_cpu.SetLoggingEnabled(false);
	auto t1 = std::chrono::high_resolution_clock::now();
	while (times != 0)
	{
		m_cpu.ExecuteInstruction();
		if (m_cpu.GetInstructionCount() == 5003)
		{
			out_cycles += m_cpu.GetCycles() / 3;
			m_cpu.PowerUp();
			m_cpu.SetRegisterProgramCounter(0xC000);
			times--;
		}
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	return static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());
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
	m_cpu.PowerUp();
	// Start execution at $C000 to run "all tests", these will generate log output
	// that will be compared to a working emulator
	m_cpu.SetRegisterProgramCounter(0xC000);
	m_cpu.SetLoggingEnabled(true);
	while (true)
	{
		std::size_t pos = m_nestest_log[m_cpu.GetInstructionCount()].find("CYC:") + 4;
		const int nestest_cycles = std::stoi(m_nestest_log[m_cpu.GetInstructionCount()].substr(pos));
		if (m_cpu.GetCycles() - nestest_cycles > 100)
		{
			m_cpu.SetCycles(nestest_cycles);
		}
		m_cpu.ExecuteInstruction();

		const std::string &log_string = m_cpu.GetLastInstructionStr();
		if (log_string.compare(m_nestest_log[m_cpu.GetInstructionCount() - 1]) != 0)
		{
			std::cout << m_cpu.GetInstructionCount() - 1 << " " << log_string << std::endl;
			std::cout << m_cpu.GetInstructionCount() - 1 << " " << m_nestest_log[m_cpu.GetInstructionCount() - 1]
					  << std::endl;
			std::cout << " " << "mismatch" << std::endl;
			return false;
		}
		else
		{
			//std::cout << m_cpu.GetInstructionCount() - 1 << " " << log_string << std::endl;
		}

		if (m_cpu.GetInstructionCount() == 5003)
		{
			return true;
		}
	}
}