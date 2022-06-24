#pragma once

#include <cinttypes>

float fp16tofp32(uint16_t fp16) noexcept;
uint16_t fp32tofp16(float fp32) noexcept;
