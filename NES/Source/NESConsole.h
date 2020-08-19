#ifndef NES_NESCONSOLE_H
#define NES_NESCONSOLE_H

#include "CPU.h"
#include "Memory.h"

class NESConsole
{
public:
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

	Memory& GetMemory()
	{
		return m_memory;
	}

private:
	CPU m_cpu;
	Memory m_memory;
	bool m_rom_loaded = false;

	//	testing
	std::vector<std::string> m_nestest_log;
};


#endif //NES_NESCONSOLE_H
