#pragma once

#include "../../CodeGenerator.hpp"

struct VertexInputs
{
	bool input_inverse_view_matrix;
	bool input_world_matrix;
	bool input_world_inverse;
	bool input_light_view_proj;
	bool input_time;
	bool input_uv_scroll_1;
	bool input_uv_scroll_2;
	bool input_uv_scroll_3;
	bool input_uv_scroll_1;
	bool input_uv_scroll_2;
	bool input_uv_scroll_3;
};

class GenerateHeader : public LogicalBlock
{
public:
	std::string generateCode(const VertexInputs& settings) override
	{
		std::string text;
		std::string delim = ", ";

		text += "void std_VS ( ";

		// Shader Uniforms
		text += "uniform float4x4 ViewProj : ViewProjection ";
		if (settings.inverse_view_matrix)
		{
			text += delim;
			text += "uniform float4x4 ViewInverse : ViewInverse ";
		}
		if (settings.apply_world_matrix)
		{
			text += delim;
			text += "uniform float4x4 World : World ";
			if (settings.use_normals)
			{
				text += delim;
				text += "uniform float4x4 WorldInverse : WorldInverse ";
			}
		}
		if (settings.receive_shadows)
		{
			text += delim;
			text += "uniform float4x4 LightViewProj";
		}
		if (settings.use_time)
		{
			text += delim;
			text += "uniform float Time : TIME";
		}
		// Surely these loops can be unified...
		for (size_t i = 0; i < settings.uv_slots.size(); ++i)
		{
			if (settings.uv_slots[i].scroll)
			{
				text += delim;
				text += std::format("uniform float2 ScrollSpeedSet{0}", i + 1);
			}
		}
		for (size_t i = 0; i < settings.uv_slots.size(); ++i)
		{
			if (settings.uv_slots[i].rotation)
			{
				text += delim;
				text += std::format("uniform float RotationSet{0} ", i + 1);
			}
		}
		for (size_t i = 0; i < settings.uv_slots.size(); ++i)
		{
			if (settings.uv_slots[i].offset)
			{
				text += delim;
				text += std::format("uniform float2 OffsetSet{0}", i + 1);
			}
		}
		for (size_t i = 0; i < settings.uv_slots.size(); ++i)
		{
			if (settings.uv_slots[i].scale)
			{
				text += delim;
				text += std::format("uniform float2 ScaleSet{0}", i + 1);
			}
		}
		if (settings.zbias)
		{
			text += delim;
			text += "uniform half ZBias ";
		}
		if (settings.fog)
		{
			text += delim;
			text += "uniform float3 FogParams";
		}
		if (settings.view_matrix)
		{
			text += delim;
			text += "uniform float3 CameraPosition : CAMERAPOSITION";
		}
		if (settings.glass_map)
		{
			text += delim;
			text += "uniform half UpsideDown ";
		}
		if (settings.fat)
		{
			text += delim;
			text += "uniform float Fat ";
		}
		if (settings.use_skeleton)
		{
			text += delim;
			text += "uniform float4 MatrixPalette[56*3]";
		}

		// Shader Inputs
		text += delim;
		text += "in float4 Position : POSITION";

		if (settings.use_normals || settings.fat)
		{
			text += delim;
			text += "in float4 Normal : NORMAL";
		}
		if (settings.use_tangents)
		{
			text += delim;
			text += "in float4 Tangent : TANGENT";
			if (!settings.calculate_binormal)
			{
				text += delim;
				text += "in float4 Binormal : BINORMAL";
			}
		}
		for (size_t i = 0; i < settings.uv_slots.size(); ++i)
		{
			auto& slot = settings.uv_slots[i];
			if (slot.enabled and !slot.is_projection)
			{
				text += delim;
				text += std::format("in float2 Texcoord{0} : TEXCOORD{0}", i);
			}
		}
		if (settings.use_vertex_colors)
		{
			text += delim;
			text += "in float4 Color : COLOR0";
		}
		if (settings.use_weights)
		{
			text += delim;
			text += std::format("in float{0} Indices : BLENDINDICES", settings.num_vertex_weights);
			text += delim;
			text += std::format("in float{0} Weights", settings.num_vertex_weights);
		}
		else
		{
			if (settings.num_vertex_weights == 1)
			{
				text += delim;
				text += std::format("in float Indices : BLENDINDICES");
			}
		}

		// Shader outputs
		if (settings.use_vertex_colors)
		{
			text += delim;
			text += "out float4 vColor : COLOR0";
		}

		size_t num_texcoords = 0;

		for (size_t i = 0; i < settings.uv_slots.size(); ++i)
		{
			auto& slot = settings.uv_slots[i];
			if (slot.enabled)
			{
				text += delim;
				text += std::format("out float{0} vTexcoord{1} : TEXCOORD{2}", slot.vec_size, i, num_texcoords);
				++num_texcoords;
			}
		}
		if (settings.use_normals)
		{
			text += delim;
			text += std::format("out float{0} vNormal : TEXCOORD{1}", settings.fragment_normal_vec_size, num_texcoords);
			++num_texcoords;
		}
		if (settings.use_tangents)
		{
			text += delim;
			text += std::format("out float3 vTangent : TEXCOORD{0}", num_texcoords);
			++num_texcoords;
			text += delim;
			text += std::format("out float3 vBinormal : TEXCOORD{0}", num_texcoords);
			++num_texcoords;
		}
		if (settings.view_matrix)
		{
			text += delim;
			text += std::format("out float3 vView : TEXCOORD{0}", num_texcoords);
			++num_texcoords;
		}
		if (settings.requires_screenpos)
		{
			text += delim;
			text += std::format("out float4 vScreenPos : TEXCOORD{0}", num_texcoords);
			++num_texcoords;
		}
		if (settings.receive_shadows)
		{
			text += delim;
			text += std::format("out float4 vShadowMapCoord : TEXCOORD{0}", num_texcoords);
			++num_texcoords;
		}
		if (settings.fog and !settings.texlayer_1.colorsampler.enabled)
		{
			text += delim;
			text += std::format("out float vFogFactor : TEXCOORD{0}", num_texcoords);
			++num_texcoords;
		}

		text += delim;
		text += "out float4 vHPosition : POSITION";
		text += " ) {\n";

		return text;
	}
};