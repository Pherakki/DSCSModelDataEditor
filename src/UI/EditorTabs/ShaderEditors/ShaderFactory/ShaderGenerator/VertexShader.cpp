#include <array>
#include <format>
#include <iostream>
#include <string>

#include "VertexShader.hpp"

class LogicalBlockInputs
{

};

class LogicalBlockOutputs
{

};

class LogicalBlock
{
protected:
	LogicalBlockInputs  inputs;
	LogicalBlockOutputs outputs;
public:
	virtual std::string generateText(const FactorySettings& settings) = 0;
};

class GenerateHeader : public LogicalBlock
{
public:
	std::string generateText(const FactorySettings& settings) override
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

class GenerateTemporaries : public LogicalBlock
{
public:
	std::string generateText(const FactorySettings& settings) override
	{
		std::string text;

		text += std::format("\tfloat4 {0};\n", "TmpPosition");
		if (settings.use_normals)
			text += std::format("\tfloat3 {0};\n", "TmpNormal");
		if (settings.use_tangents)
		{
			text += std::format("\tfloat3 {0};\n", "TmpTangent");
			text += std::format("\tfloat3 {0};\n", "TmpBinormal");
		}

		return text;
	}

};


enum class SumState
{
	NoSum = 0,
	Sum = 1,
	SpacedNoSum = 2
};



std::string makeJointTransform(const FactorySettings& settings, const SumState& sum_state, const std::string& matrix_template, const std::string& input_position, const std::string& tmp_position, const std::string& output_position, const std::string& op, const std::string& dim)
{
	std::string text;
	text += std::format("\t{0}[0] = {1};", "jointMatrix", std::format(matrix_template, "0"));
	text += std::format("\t{0}[1] = {1};", "jointMatrix", std::format(matrix_template, "1"));
	text += std::format("\t{0}[2] = {1};", "jointMatrix", std::format(matrix_template, "2"));

	std::string matrix_application_template;
	switch (sum_state)
	{
	case SumState::NoSum:
		matrix_application_template = "\t{3} = mul({0},{1}.xyz);\n";

		text += std::format("\t{2} = mul({0}, {1});;\n", "jointMatrix", input_position, output_position);
		break;
	case SumState::Sum:
		matrix_application_template += "\t{2}.xyz = mul({0},{1}.xyz);\n";
		matrix_application_template += "\t{3}.xyz {4}= {2}.xyz*{6}.{5};\n";

		text += std::format("\t{2} = mul({0}, {1});;\n", "jointMatrix", input_position, tmp_position);
		text += std::format("\t{4} {0}= {3}*{2}.{1};\n", op, dim, "Weights", tmp_position, output_position);
		break;
	default:
		throw std::exception("Bad joint");
	}

	if (settings.use_normals)
	{
		text += std::format(matrix_application_template, "(float3x3)jointMatrix", "Normal", "tmp", "TmpNormal", op, dim, "Weights");
		if (settings.use_tangents)
		{
			text += std::format(matrix_application_template, "(float3x3)jointMatrix", "Tangent", "tmp", "TmpTangent", op, dim, "Weights");
			if (!settings.calculate_binormal)
				text += std::format(matrix_application_template, "(float3x3)jointMatrix", "Binormal", "tmp", "TmpBinormal", op, dim, "Weights");
		}
	}

	return text;
}


std::string makeBillboardTransform(const FactorySettings& settings, const SumState& sum_state, const std::string& matrix_template, const std::string& input_position, const std::string& tmp_position, const std::string& output_position, const std::string& op, const std::string& dim)
{
	const static std::string scale = "wscale";
	const static std::string trans = "wtrans";

	std::string palette_0 = std::format(matrix_template, "0");
	std::string palette_1 = std::format(matrix_template, "1");
	std::string palette_2 = std::format(matrix_template, "2");

	std::string text = "";
	text += std::format("\t{0} = float3(length( float3({2}.x,{3}.x,{4}.x) ), "
		"length( float3({2}.y,{3}.y,{4}.y) ), "
		"length( float3({2}.z,{3}.z,{4}.z) ));"
		"\t{1} = float3({2}.w,{3}.w,{4}.w);;\n",
		scale, trans, palette_0, palette_1, palette_2);

	switch (sum_state)
	{
	case SumState::NoSum:
		text += std::format("\t{0}.xyz = {1}.xyz*{2} + {3};\n", output_position, input_position, scale, trans);
		break;
	case SumState::SpacedNoSum:
		text += std::format("\t{0}.xyz = {1}.xyz * {2} + {3};\n", output_position, input_position, scale, trans);
		break;
	case SumState::Sum:
		text += std::format("\t{0}.xyz = {1}.xyz*{2} + {3};\n", tmp_position, input_position, scale, trans);
		text += std::format("\t{0}.xyz {4}= {1}*{2}.{3};\n", output_position, tmp_position, "Weights", dim, op);
		break;
	default:
		throw std::exception("Bad billboard");
	}

	if (op == "")
		text += std::format("\t{0}.w = 1.0;\n", output_position);

	return text;
}


class NormalizeVectors : public LogicalBlock
{
private:
	std::string norm;
	const std::string tan;
	const std::string bin;
	const bool pre_normalize;
	const bool use_inputs;
public:
	NormalizeVectors(const std::string& norm, const std::string& tan, const std::string& bin, bool pre_normalize, bool use_inputs)
		: LogicalBlock()
		, norm{ norm }
		, tan{ tan }
		, bin{ bin }
		, pre_normalize{ pre_normalize }
		, use_inputs{ use_inputs }
	{}

	std::string generateText(const FactorySettings& settings) override
	{
		std::string text;

		std::string input_template;
		if (pre_normalize)
			input_template = "normalize({0}.xyz)";
		else
			input_template = "{0}";

		std::string input_norm;
		std::string input_tan;
		std::string input_bin;
		if (use_inputs)
		{
			input_norm = norm;
			input_tan = tan;
			input_bin = bin;
		}
		else
		{
			input_norm = "Normal";
			input_tan = "Tangent";
			input_bin = "Binormal";
		}

		std::string out_norm = "TmpNormal";
		std::string out_tan = "TmpTangent";
		std::string out_bin = "TmpBinormal";
		if (settings.use_normals)
		{
			text += std::format("\t{0}.xyz = {1};\n", out_norm, std::format(input_template, this->norm));
			if (settings.use_tangents)
			{
				if (settings.calculate_binormal)
				{
					text += std::format("\t{0}.xyz = normalize({2}.xyz - {1}.xyz * dot({1}.xyz, {2}.xyz)) * {3}.w;\n", out_tan, input_norm, input_tan, "Tangent");
					text += std::format("\t{0}.xyz = cross({1}.xyz, {2}.xyz) * {3}.w;\n", out_bin, input_norm, input_tan, "Tangent");
				}
				else
				{
					text += std::format("\t{0}.xyz = {1};\n", out_tan, std::format(input_template, tan));
					text += std::format("\t{0}.xyz = {1};\n", out_bin, std::format(input_template, bin));
				}
			}
		}
		return text;
	}
};

std::string noAdjustViewInverse(const FactorySettings& settings)
{
	std::string text;

	text += "\tfloat4x4 jointMatrix;\n";
	text += "\tjointMatrix[3]=float4(0.0,0.0,0.0,1.0);\n";

	text += "\tfloat4 pos = Position;\n";

	if (settings.fat)
		text += "\tpos.xyz += Normal.xyz*Fat;\n";

	return text;
}

std::string adjustViewInverse(const FactorySettings& settings)
{
	std::string text = "";
	std::string view_inverse = "ViewInverse";
	const static std::string camPos = "camPos";
	if (settings.align_with_y)
	{
		text += std::format("\tfloat3x3 {0};\n", camPos);
		text += std::format("\t{0}[2] = normalize( float3({1}[2].x, 0.0, {1}[2].z));\n", camPos, view_inverse);
		text += std::format("\t{0}[0] = normalize( cross(float3(0.0,1.0,0.0), {0}[2]) );\n", camPos);
		text += std::format("\t{0}[1] = cross({0}[2],{0}[0]);\n", camPos);
		view_inverse = camPos;
	}
	else
		view_inverse = std::format("(float3x3){0}", view_inverse);


	text += "\tfloat4 InPos;\n";

	std::string view_template = std::format("\t{3}{2} = mul({0},{1}.xyz);\n", view_inverse, "{0}", "{1}", "{2}");
	if (settings.fat)
	{
		text += "\tfloat4 pos = Position;\n";
		text += "\tpos.xyz += Normal.xyz*Fat;\n";
		text += std::format(view_template, "pos", "InPos.xyz", "");
	}
	else
	{
		text += std::format(view_template, "Position", "InPos.xyz", "");
	}

	text += "\tInPos.w = 1.0;\n";


	if (settings.use_normals)
	{
		text += std::format(view_template, "Normal", "InNom", "float3 ");

		if (settings.use_tangents)
		{
			text += std::format(view_template, "Tangent", "InTan", "float3 ");
			if (!settings.calculate_binormal)
				text += std::format(view_template, "Binormal", "InBin", "float3 ");
		}
	}

	text += "\tfloat3 wscale;\n";
	text += "\tfloat3 wtrans;\n";

	return text;
}

class TransformVertices : public LogicalBlock
{
public:
	std::string generateText(const FactorySettings& settings) override
	{
		const static std::array<std::string, 4> ops{ "", "+", "+", "+" };
		const static std::array<std::string, 4> dims{ "x", "y", "z", "w" };
		std::string text;

		std::string tmp_type;
		std::string no_index_template;
		decltype(makeBillboardTransform)* fn;
		std::string input_position;
		std::string input_normal;
		std::string input_tangent;
		std::string input_binormal;
		if (settings.is_billboard)
		{
			tmp_type = "3";
			fn = makeBillboardTransform;
			no_index_template = "MatrixPalette[0+{0}]";
			input_position = "InPos";
			input_normal = "InNom";
			input_tangent = "InTan";
			input_binormal = "InBin";
		}
		else
		{
			tmp_type = "4";
			fn = makeJointTransform;
			no_index_template = "MatrixPalette[ 0 + {0} ]";
			input_position = "pos";
			input_normal = "TmpNormal";
			input_tangent = "TmpTangent";
			input_binormal = "TmpBinormal";
		}

		if (settings.use_skeleton)
		{
			if (settings.is_billboard)
				text += adjustViewInverse(settings);
			else
				text += noAdjustViewInverse(settings);

			if (settings.use_weights)
			{
				text += std::format("\tfloat{1} {0};\n", "tmp", tmp_type);
				text += std::format("\tfloat{0} {1} = {2};\n", settings.num_vertex_weights, "idx", "Indices");

				for (size_t i = 0; i < settings.num_vertex_weights; ++i)
				{
					auto& op = ops[i];
					auto& dim = dims[i];

					text += fn(settings, SumState::Sum, std::format("MatrixPalette[ {0}.{1} + {2} ]", "idx", dim, "{0}"), input_position, "tmp", "TmpPosition", op, dim);
				}

			}

			else
			{
				if (settings.is_billboard)
					text += std::format("\tfloat4 {0} = {1};\n", "pos", input_position);
				text += std::format("\tfloat {0} = {1}.w;\n", "idx", "Position");
				text += std::format("\t{0}.w=1.0;\n", "pos");

				if (settings.num_vertex_weights)
					text += fn(settings, SumState::NoSum, std::format("MatrixPalette[ {0} + {1} ]", "idx", "{0}"), "pos", "", "TmpPosition", "", "");
				else
					text += fn(settings, SumState::NoSum, no_index_template, "pos", "", "TmpPosition", "", "");
			}

		}
		else
		{
			if (settings.is_billboard)
				text += std::format("\t{0} = {1};\n", "TmpPosition", input_position);
			else
				text += std::format("\t{0} = {1};\n", "TmpPosition", "Position");
		}

		NormalizeVectors normalizer(input_normal, input_tangent, input_binormal, settings.use_skeleton, settings.is_billboard);
		text += normalizer.generateText(settings);

		return text;
	}
};


class HandleWorldTransform : public LogicalBlock
{
public:
	std::string generateText(const FactorySettings& settings) override
	{
		std::string text;

		if (settings.apply_world_matrix)
		{
			if (settings.is_billboard)
			{
				text += makeBillboardTransform(settings, SumState::SpacedNoSum, "World[{0}]", "TmpPosition", "", "TmpPosition", "", "");
				NormalizeVectors normalizer("TmpNormal", "TmpTangent", "TmpBinormal", true, false);
				normalizer.generateText(settings);
			}
			else
			{
				text += std::format("\t{0} = mul({1}, {0});\n", "TmpPosition", "World");

				std::string matrix_application_template = "\t{2}.xyz = normalize(mul({0}.xyz, (float3x3){1}));\n";
				if (settings.use_normals)
					text += std::format(matrix_application_template, "TmpNormal", "WorldInverse", "vNormal");
				if (settings.use_tangents)
				{
					text += std::format(matrix_application_template, "TmpTangent", "WorldInverse", "vTangent");
					text += std::format(matrix_application_template, "TmpBinormal", "WorldInverse", "vBinormal");
				}
			}
		}
		else
		{
			std::string basic_assignment_template = "\t{1} = {0}.xyz;\n";
			if (settings.use_normals)
				text += std::format(basic_assignment_template, "TmpNormal", "vNormal.xyz");
			if (settings.use_tangents)
			{
				text += std::format(basic_assignment_template, "TmpTangent", "vTangent");
				text += std::format(basic_assignment_template, "TmpBinormal", "vBinormal");
			}
		}

		text += std::format("\t{2} = mul({0},{1});\n", "ViewProj", "TmpPosition", "vHPosition");
		return text;
	}

};

template<size_t slot>
class GenerateUVTransforms : public LogicalBlock
{
public:
	std::string generateText(const FactorySettings& settings) override
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
				// What is this even for...?!
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

class OutputVertexColors : public LogicalBlock
{
public:
	std::string generateText(const FactorySettings& settings) override
	{
		std::string text;
		if (settings.use_vertex_colors)
		{
			std::string outcolor_variable = "vColor";
			std::string incolor_variable = "Color";

			text += std::format("\t{0} = {1};\n", outcolor_variable, incolor_variable);
		}
		return text;
	}
};

class GenerateScreenpos : public LogicalBlock
{
public:
	std::string generateText(const FactorySettings& settings) override
	{
		std::string text;
		if (settings.requires_screenpos)
		{
			std::string screenpos_variable = "vScreenPos";
			std::string position_variable = "vHPosition";

			text += std::format("\t{0} = {1};\n", screenpos_variable, position_variable);
			text += std::format("\t{0}.xy = {0}.xy * 0.5 + (0.5*{1}.w);\n", screenpos_variable, position_variable);
		}
		return text;
	}
};

class GenerateShadows : public LogicalBlock
{
public:
	std::string generateText(const FactorySettings& settings) override
	{
		std::string text;
		if (settings.receive_shadows)
		{
			std::string shadowmap_variable = "vShadowMapCoord";
			std::string light_viewproj = "LightViewProj";
			std::string position_variable = "TmpPosition";

			text += std::format("\t{0} = mul({1}, float4({2}.xyz,1.0));\n", shadowmap_variable, light_viewproj, position_variable);
			text += std::format("\t{0}.x *= 0.5;\n", shadowmap_variable);
			text += std::format("\t{0}.y *= 0.5;\n", shadowmap_variable);
			text += std::format("\t{0}.xy += {0}.w * 0.5;\n", shadowmap_variable);
		}

		return text;
	}
};

class GenerateFog : public LogicalBlock
{
public:
	std::string generateText(const FactorySettings& settings) override
	{
		std::string text;
		if (settings.fog)
		{
			std::string fog_variable;
			std::string position_variable;
			std::string fog_params = "FogParams";

			// Set Fog Variable
			if (settings.use_normals)
				fog_variable = "vNormal.w";
			else if (settings.texlayer_1.colorsampler.enabled)
				fog_variable = std::format("vTexcoord{0}.z", settings.texlayer_1.colorsampler.uv_slot);
			else
				fog_variable = "vFogFactor";

			// Set Position Variable
			if (settings.fog_height)
				position_variable = "TmpPosition.y";
			else
				position_variable = "vHPosition.w";

			// Generate text
			text += std::format("\t{0} = 1.0 - ({1}.x + {2} * {1}.y);\n", fog_variable, fog_params, position_variable);
			text += std::format("\t{0} = clamp({0},0.0,1.0)*{1}.z;\n", fog_variable, fog_params);
		}

		return text;
	}
};

class OutputViewMatrix : public LogicalBlock
{
public:
	std::string generateText(const FactorySettings& settings) override
	{
		std::string text;
		if (settings.view_matrix)
			// Inputs: CameraPosition, TmpPosition
			// Outputs: vView
			// Flags: view_matrix
			text += std::format("\t{0} = normalize({1} - {2}.xyz);\n", "vView", "CameraPosition", "TmpPosition");
		return text;
	}
};

class BiasOutputPosition : public LogicalBlock
{
public:
	std::string generateText(const FactorySettings& settings) override
	{
		std::string text;
		if (settings.zbias)
			// Inputs: vHPosition, ZBias
			// Outputs: vHPosition
			// Flags: zbias
			text += std::format("\t{0}.z = {1}.z + {2}*0.3;\n", "vHPosition", "vHPosition", "ZBias");
		return text;
	}
};

std::string generateVertexShader(const FactorySettings& settings)
{
	std::string text = "";
	// Generate Header
	GenerateHeader header_gen;
	text += header_gen.generateText(settings);
	// Generate Temporaries
	GenerateTemporaries temps_gen;
	text += temps_gen.generateText(settings);
	// TransformVertices
	TransformVertices trans_verts;
	text += trans_verts.generateText(settings);
	HandleWorldTransform hwt;
	text += hwt.generateText(settings);
	// Transform UVs
	GenerateUVTransforms<0> gut1;
	text += gut1.generateText(settings);
	GenerateUVTransforms<1> gut2;
	text += gut2.generateText(settings);
	GenerateUVTransforms<2> gut3;
	text += gut3.generateText(settings);
	// Pass Color
	OutputVertexColors ovc;
	text += ovc.generateText(settings);
	// Make Screenpos
	GenerateScreenpos screenpos_gen;
	text += screenpos_gen.generateText(settings);
	// GenerateShadows
	GenerateShadows shadow_gen;
	text += shadow_gen.generateText(settings);
	// GenerateFog
	GenerateFog fog_gen;
	text += fog_gen.generateText(settings);
	// Output ViewMatrix
	OutputViewMatrix ovm;
	text += ovm.generateText(settings);
	// BiasOutputPosition
	BiasOutputPosition bop;
	text += bop.generateText(settings);
	// Finalise shader
	text += "}\n";
	return text;
}
