#pragma once

#include <string>

namespace FileFormats::DSCS::GeomFile
{
	struct CamDataStruct
	{
		uint32_t target_joint_hash = 0;    // 0x00
		float fov = 0;                     // 0x04
		float maybe_aspect_ratio = 0;      // 0x08
		float zNear = 0;                   // 0x0C

		float zFar = 0;                    // 0x10
		float orthographic_distance = 0;   // 0x14
		uint32_t projection_type = 0;      // 0x18 // 0 = perspective, 1 = orthographic
		uint32_t padding_0x1C = 0;         // 0x1C

		uint64_t padding_0x20 = 0;
		uint64_t padding_0x28 = 0;
	};
}