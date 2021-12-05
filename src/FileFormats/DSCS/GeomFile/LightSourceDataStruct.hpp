#pragma once

#include <string>

namespace FileFormats::DSCS::GeomFile
{	
	struct LightSourceDataStruct
	{
		uint32_t bone_name_hash = 0;
		uint16_t lighting_type = 0;
		uint16_t light_id = 0;

		float intensity = 0.f;
		float unknown_fog_param = 0.f;

		float red = 0.f;
		float green = 0.f;
		float blue = 0.f;
		float alpha = 0.f;

		float unknown_0x20 = 0;
		float unknown_0x24 = 0;
		float unknown_0x28 = 0;
		uint32_t padding_0x2C = 0;

		uint64_t padding_0x30 = 0;
		uint64_t padding_0x38 = 0;
	};
}