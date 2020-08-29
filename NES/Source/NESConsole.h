#pragma once
#include <memory>
#include <mutex>
#include "CPU.h"
#include "Memory.h"
#include "../PPU.h"

class NESConsole
{
public:
	static std::shared_ptr<NESConsole> GetInstance(void);
	void LoadROM(const std::string &path);

	void RunROM();

	const ROM &GetROM() const;

	void RunTests();

	bool RunNesTest();

	int RunNesTestTiming(int times, int &out_cycles);

	CPU& GetCPU()
	{
		return m_cpu;
	}

	PPU& GetPPU()
	{
		return m_ppu;
	}

	Memory& GetMemory()
	{
		return m_memory;
	}

private:
	CPU m_cpu;
	PPU m_ppu;
	Memory m_memory;
	bool m_rom_loaded = false;

	//	testing
	std::vector<std::string> m_nestest_log;

	static std::shared_ptr<NESConsole> NESConsoleSingleton;
	static std::once_flag NESConsoleOnceFlag;
};