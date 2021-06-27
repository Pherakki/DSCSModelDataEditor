#pragma once

#include <string>

namespace FileFormats::DSCS::GeomFile
{
	typedef uint16_t FakeFloat16_t;
	
	struct Sec4ReadWrite
	{
		uint16_t unknown_0x00 = 0;
		uint16_t unknown_0x02 = 0;
		uint16_t flag =         0;
		uint16_t unknown_0x06 = 0;

		float unknown_0x08 = 0.f;
		float unknown_0x0C = 0.f;

		float unknown_0x10 = 0.f;
		float unknown_0x14 = 0.f;
		float unknown_0x18 = 0.f;
		float unknown_0x1C = 0.f;

		float unknown_0x20 = 0;
		float unknown_0x24 = 0;
		float unknown_0x28 = 0;
		uint32_t unknown_0x2C = 0;

		uint64_t unknown_0x30 = 0;
		uint64_t unknown_0x38 = 0;
	};
}