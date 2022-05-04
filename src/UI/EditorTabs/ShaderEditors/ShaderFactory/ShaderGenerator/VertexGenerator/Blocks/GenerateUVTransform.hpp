#pragma once

#include "../../CodeGenerator.hpp"

template<size_t slot>
class GenerateUVTransforms : public LogicalBlock
{
public:
	bool coord_enabled;
	bool projection_enabled;
	bool scroll_enabled;
	bool rotation_enabled;
	bool offset_enabled;
	bool scale_enabled;

	VariableReference inputTexcoord{ [](FactorySettings& settings) { return settings.uv_slot[slot].is_enabled; } };
	VariableReference inputScroll{ [](FactorySettings& settings) { return settings.uv_slot[slot].scroll; } };
	VariableReference inputTime{ [](FactorySettings& settings) { return settings.uv_slot[slot].scroll; } };
	VariableReference inputRotation{ [](FactorySettings& settings) { return settings.uv_slot[slot].rotation; } };
	VariableReference inputOffset{ [](FactorySettings& settings) { return settings.uv_slot[slot].offset; } };
	VariableReference inputScale{ [](FactorySettings& settings) { return settings.uv_slot[slot].scale; } };
	VariableReference inputPosition{ [](FactorySettings& settings) { return settings.uv_slot[slot].is_enabled && settings.uv_slot[slot].is_projection; }};
	VariableReference outputTexcoord{ [](FactorySettings& settings) { return settings.uv_slot[slot].is_enabled; } };

	void updateFlags(const FactorySettings& settings)
	{

	}

	std::string generateCode(const FactorySettings& settings) override
	{
		std::string in_tex_coord = std::format("Texcoord{0}", slot);
		std::string out_tex_coord = std::format("vTexcoord{0}", slot);
		std::string position_variable = "vHPosition";
		auto& uv_slot = settings.uv_slots[slot];

		std::string text;
		if (uv_slot.enabled)
		{
			// Projection
			if (uv_slot.is_projection)
			{
				text += std::format("\t{0}.xy = {1}.xy/{1}.w;\n", out_tex_coord, position_variable);
			}
			else
			{
				if constexpr (slot > 0)
					text += std::format("\t{1} = {0}.xy;\n", in_tex_coord, out_tex_coord);
				else
					text += std::format("\t{1}.xy = {0};\n", in_tex_coord, out_tex_coord);
			}

			// Scroll
			if (uv_slot.scroll)
			{
				std::string scroll_variable = std::format("ScrollSpeedSet{0}", slot + 1);
				std::string time_variable = "Time";
				text += std::format("\t{0}.xy += frac({1} * {2});\n", out_tex_coord, scroll_variable, time_variable);
			}

			// Rotation
			if (uv_slot.rotation)
			{
				std::string rotation_variable = std::format("RotationSet{0}", slot + 1);
				std::string cosine_variable = "c";
				std::string sine_variable = "s";
				text += std::format("\tfloat {1} = cos({0});\n", rotation_variable, cosine_variable);
				text += std::format("\tfloat {1} = sin({0});\n", rotation_variable, sine_variable);
				text += std::format("\t{0}.xy = float2({0}.x * {1} - {0}.y * {2}, {0}.x * {2} + {0}.y * {1});\n", out_tex_coord, cosine_variable, sine_variable);
			}

			// Translation
			if (uv_slot.offset)
			{
				std::string offset_variable = std::format("OffsetSet{0}", slot + 1);
				text += std::format("\t{0}.xy = {0}.xy + {1};\n", out_tex_coord, offset_variable);
			}

			// Scale
			if (uv_slot.scale)
			{
				std::string scale_variable = std::format("ScaleSet{0}", slot + 1);
				text += std::format("\t{0}.xy = {0}.xy * {1};\n", out_tex_coord, scale_variable);
			}
		}
		return text;
	}
};
