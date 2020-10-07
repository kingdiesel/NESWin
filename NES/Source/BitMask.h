#pragma once
#include <cstdint>

constexpr uint8_t BIT_7 = 0x80;
constexpr uint8_t BIT_0_1 = 0x30;
constexpr uint8_t BIT_0_1_2_3_4 = 0x1F;

constexpr uint8_t BIT_CONTROL_RESET = 0x0C;

constexpr uint16_t ADDR_8K_MAX = 0x1FFF;

inline bool Bit7Set(const uint8_t value)
{
	return value & BIT_7;
}