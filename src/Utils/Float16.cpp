#include <cstring>

#include "Float16.hpp"


float fp16tofp32(uint16_t fp16) noexcept
{
	bool     signbit = (fp16 & 0x8000) >> 15;
	int16_t  exp = (fp16 & 0x7C00) >> 10;
	uint16_t sig = (fp16 & 0x03FF) >> 0;

	uint32_t out;

	if (exp == 0)
	{
		if (sig)
		{
			// Subnormal
			// This is significantly faster (>2x) than _BitScanReverse for all but the smallest subnormals
			exp = 0;
			while ((sig & 0x0200) == 0)
			{
				sig <<= 1;
				exp++;
			}

			sig <<= 1;
			sig &= 0x03FF;
			out = (signbit << 31) | ((-exp + (-15 + 127)) << 23) | (sig << 13);
		}
		else
		{
			// Return +/- 0
			out = (signbit << 31);
		}
	}
	else if (exp == 0x1F)
	{
		// +/- inf or quiet NaN
		out = (signbit << 31) | (0xFF << 23) | (sig << 13);
	}
	else
	{
		out = (signbit << 31) | ((exp + (-15 + 127)) << 23) | (sig << 13);
	}

	// Use memcpy: it'll get elided by the compiler, reinterpret_cast and unions can cause UB
	float f;
	memcpy(&f, &out, sizeof(decltype(f)));
	return f;
}

uint16_t fp32tofp16(float fp32) noexcept
{
	// Use memcpy: it'll get elided by the compiler, reinterpret_cast and unions can cause UB
	uint32_t ifp32;
	memcpy(&ifp32, &fp32, sizeof(decltype(ifp32)));

	bool     signbit = (ifp32 & 0x80000000) >> 31;
	uint16_t  exp = (ifp32 & 0x7F800000) >> 23;
	uint32_t sig = (ifp32 & 0x007FFFFF) >> 0;

	uint16_t out;

	if (exp == 0)
	{
		// Maps to 0 or a fp32 subnormal; these all underflow fp16 so set to 0
		out = (signbit << 15) | (0x00 << 10) | 0x00;
	}
	else if (exp == 0xFF)
	{
		// +/- inf or quiet NaN
		out = (signbit << 15) | (0x1F << 10) | (sig ? 0x0200 : 0x0000);
	}
	else
	{
		int16_t rebiased_exp = exp + (-127 + 15);
		if (rebiased_exp >= 0x1F) // Larger than max value; return +/- inf
			out = (signbit << 15) | (0x1F << 10) | 0x0000;
		else if (rebiased_exp <= 0) // Handle normal fp32s that map to subnormals
		{
			int16_t shift = rebiased_exp * -1;
			// Extra shift + 0x200 accounts for the implicit leading 1 in the normal that is explicit in the subnormal
			out = (signbit << 15) | (0x00 << 10) | (((0x200 | (sig >> (13 + 1))) >> shift));
		}
		else
			out = (signbit << 15) | (rebiased_exp << 10) | (sig >> 13);
	}

	return out;
}