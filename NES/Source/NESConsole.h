#pragma once
#include <memory>
#include <mutex>
#include <fstream>
#include "CPU.h"
#include "Memory.h"
#include "../PPU.h"

class NESConsole
{
public:
	static std::shared_ptr<NESConsole> GetInstance(void);
	NESConsole();
	~NESConsole();
	void LoadROM(const std::string &path);
	void Initialize();
	void PowerUp();
	void Run();

	const ROM &GetROM() const;

	bool RunNesTest();

	void RunNesTestTiming();

	CPU& GetCPU()
	{
		return *m_cpu;
	}

	PPU& GetPPU()
	{
		return m_ppu;
	}

	Memory& GetMemory()
	{
		return m_memory;
	}
	
	void FlushLog();

	std::ofstream& GetLogStream() { return m_log_file; }

	void SetDownPressed(const bool down_pressed) { m_down_pressed = down_pressed; }
	bool GetDownPressed() const { return m_down_pressed; }

	void SetUpPressed(const bool up_pressed) { m_up_pressed = up_pressed; }
	bool GetUpPressed() const { return m_up_pressed; }

	void SetLeftPressed(const bool left_pressed) { m_left_pressed = left_pressed; }
	bool GetLeftPressed() const { return m_left_pressed; }

	void SetRightPressed(const bool right_pressed) { m_right_pressed = right_pressed; }
	bool GetRightPressed() const { return m_right_pressed; }

	void SetStartPressed(const bool start_pressed) { m_start_pressed = start_pressed; }
	bool GetStartPressed() const { return m_start_pressed; }

	void SetSelectPressed(const bool select_pressed) { m_select_pressed = select_pressed; }
	bool GetSelectPressed() const { return m_select_pressed; }

	void SetAPressed(const bool a_pressed) { m_a_pressed = a_pressed; }
	bool GetAPressed() const { return m_a_pressed; }

	void SetBPressed(const bool b_pressed) { m_b_pressed = b_pressed; }
	bool GetBPressed() const { return m_b_pressed; }

	void SetDmaPage(const uint8_t page) { m_dma_page = page; }
	void SetDmaAddr(const uint8_t addr) { m_dma_addr = addr; }
	void SetDmaTransfer(const bool transfer) { dma_transfer = true; }

private:
	CPU* m_cpu = nullptr;
	PPU m_ppu;
	Memory m_memory;
	bool m_rom_loaded = false;

	//	testing
	std::vector<std::string> m_nestest_log;

	static std::shared_ptr<NESConsole> NESConsoleSingleton;
	static std::once_flag NESConsoleOnceFlag;
	bool m_down_pressed = false;
	bool m_up_pressed = false;
	bool m_left_pressed = false;
	bool m_right_pressed = false;
	bool m_a_pressed = false;
	bool m_b_pressed = false;
	bool m_start_pressed = false;
	bool m_select_pressed = false;

	uint8_t m_dma_page = 0x00;
	uint8_t m_dma_addr = 0x00;
	uint8_t m_dma_data = 0x00;
	bool dma_dummy = true;
	bool dma_transfer = false;

	uint32_t cycles = 0;

	std::ofstream m_log_file;
};