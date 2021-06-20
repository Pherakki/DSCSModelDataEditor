#pragma once

#include <string>

namespace FileFormats::DSCS::GeomFile
{
	typedef uint16_t FakeFloat16_t;

	struct Sec5ReadWrite
	{
		uint16_t unknown_0x00 = 0;
		uint16_t unknown_0x02 = 0;
		uint16_t unknown_0x04 = 0;
		FakeFloat16_t unknown_0x06 = 0;
		uint16_t unknown_0x08 = 0;
		FakeFloat16_t unknown_0x0A = 0;
		uint16_t unknown_0x0C = 0;
		FakeFloat16_t unknown_0x0E = 0;

		int16_t unknown_0x10 = 0;
		FakeFloat16_t unknown_0x12 = 0;
		int16_t unknown_0x14 = 0;
		FakeFloat16_t unknown_0x16 = 0;
		int16_t unknown_0x18 = 0;
		uint16_t unknown_0x1A = 0;
		uint32_t unknown_0x1C = 0;

		uint64_t unknown_0x20 = 0;
		uint64_t unknown_0x28 = 0;
	};
}