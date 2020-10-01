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

NESConsole::NESConsole()
{
	std::string filename = "C:/Users/Public/logs/example";
	time_t rawtime;
	struct tm timeinfo;
	char buffer[80];

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);

	strftime(buffer, 80, "%Y_%m_%d_%H_%M_%S", &timeinfo);
	filename += buffer;
	filename += ".txt";
	m_log_file.open(filename);
}

NESConsole::~NESConsole()
{
	m_log_file.close();
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
	while (!GetPPU().GetFrameReady())
	{
		// Clocking. The heart and soul of an emulator. The running
		// frequency is controlled by whatever calls this function.
		// So here we "divide" the clock as necessary and call
		// the peripheral devices clock() function at the correct
		// times.

		// The fastest clock frequency the digital system cares
		// about is equivalent to the PPU clock. So the PPU is clocked
		// each time this function is called...
		m_ppu.Run();

		// The CPU runs 3 times slower than the PPU so we only call its
		// clock() function every 3 times this function is called. We
		// have a global counter to keep track of this.
		if (cycles % 3 == 0)
		{
			// Is the system performing a DMA transfer form CPU memory to 
			// OAM memory on PPU?...
			if (dma_transfer)
			{
				// ...Yes! We need to wait until the next even CPU clock cycle
				// before it starts...
				if (dma_dummy)
				{
					// ...So hang around in here each clock until 1 or 2 cycles
					// have elapsed...
					if (cycles % 2 == 1)
					{
						// ...and finally allow DMA to start
						dma_dummy = false;
					}
				}
				else
				{
					// DMA can take place!
					if (cycles % 2 == 0)
					{
						// On even clock cycles, read from CPU bus
						m_dma_data = m_memory.CPUReadByte(m_dma_page << 8 | m_dma_addr);
					}
					else
					{
						// On odd clock cycles, write to PPU OAM
						assert(m_dma_addr >= 0 && m_dma_addr < 256);
						m_memory.GetPrimaryOAM()[m_dma_addr] = m_dma_data;
						//ppu.pOAM[m_dma_addr] = m_dma_data;
						// Increment the lo byte of the address
						m_dma_addr++;
						// If this wraps around, we know that 256
						// bytes have been written, so end the DMA
						// transfer, and proceed as normal
						if (m_dma_addr == 0x00)
						{
							dma_transfer = false;
							dma_dummy = true;
						}
					}
				}
			}
			else
			{
				// No DMA happening, the CPU is in control of its
				// own destiny. Go forth my friend and calculate
				// awesomeness for many generations to come...
				m_cpu->Run();
			}
		}
		cycles++;
	}
	GetPPU().ResetFrameReady();
#if 0
	int cycles = 0;
	const int current_cpu_cycles = GetCPU().GetCycles();
	while (!GetPPU().GetFrameReady())
	{
		// https://wiki.nesdev.com/w/index.php/PPU_frame_timing#CPU-PPU_Clock_Alignment
		// The NTSC PPU runs at 3 times the CPU clock rate 
		if (cycles % 3 == 0)
		{
			GetCPU().Run();
			
		}
		GetPPU().Run();
		cycles++;
	}
	const int achieved_cycles = GetCPU().GetCycles() - current_cpu_cycles;
	//std::cout << achieved_cycles << std::endl;
	GetPPU().ResetFrameReady();
#endif
}

const ROM &NESConsole::GetROM() const
{
	return m_memory.GetROM();
}

void NESConsole::RunNesTestTiming()
{
	Initialize();
	LoadROM("C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/nestest.nes");
	m_cpu->PowerUp();
	m_cpu->SetRegisterProgramCounter(0xC000);
	m_cpu->SetLoggingEnabled(false);
	auto t1 = std::chrono::high_resolution_clock::now();
	int times = 10000;
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

void NESConsole::FlushLog()
{
	m_log_file.flush();
	m_log_file.close();
}

bool NESConsole::RunNesTest()
{
	Initialize();
	std::ifstream myfile("C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/nestest_out.txt");
	std::string line;
	while (std::getline(myfile, line))
	{
		m_nestest_log.push_back(line);
	}

	LoadROM("C:/Users/aspiv/source/repos/kingdiesel/NESWin/NES/TestRoms/nestest.nes");
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