#pragma once
#include <cstdint>

constexpr uint8_t BIT7 = 0x80;
constexpr uint8_t BIT0_1 = 0x30;

inline bool Bit7Set(const uint8_t value)
{
	return value & BIT7;
}