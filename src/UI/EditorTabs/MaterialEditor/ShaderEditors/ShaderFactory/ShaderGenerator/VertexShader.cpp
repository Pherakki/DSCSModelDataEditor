#include <array>
#include <functional>
#include <iostream>
#include <sstream>
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
	virtual void generateText(const FactorySettings& settings, std::stringstream& ss) = 0;
};

class GenerateHeader : public LogicalBlock
{
public:
	void generateText(const FactorySettings& settings, std::stringstream& ss) override
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
				//text += std::format("uniform float2 ScrollSpeedSet{0}", i + 1);
				text += "uniform float2 ScrollSpeedSet" + std::to_string(i + 1);
			}
		}
		for (size_t i = 0; i < settings.uv_slots.size(); ++i)
		{
			if (settings.uv_slots[i].rotation)
			{
				text += delim;
				//text += std::format("uniform float RotationSet{0} ", i + 1);
				text += "uniform float RotationSet" + std::to_string(i + 1);
			}
		}
		for (size_t i = 0; i < settings.uv_slots.size(); ++i)
		{
			if (settings.uv_slots[i].offset)
			{
				text += delim;
				//text += std::format("uniform float2 OffsetSet{0}", i + 1);
				text += "uniform float OffsetSet" + std::to_string(i + 1);
			}
		}
		for (size_t i = 0; i < settings.uv_slots.size(); ++i)
		{
			if (settings.uv_slots[i].scale)
			{
				text += delim;
				//text += std::format("uniform float2 ScaleSet{0}", i + 1);
				text += "uniform float2 ScaleSet" + std::to_string(i + 1);
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
			if (slot.enabled && !slot.is_projection)
			{
				text += delim;
				//text += std::format("in float2 Texcoord{0} : TEXCOORD{0}", i);
				auto i_str = std::to_string(i);
				text += "in float2 Texcoord" + i_str + " : TEXCOORD" + i_str;
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
			//text += std::format("in float{0} Indices : BLENDINDICES", settings.num_vertex_weights);
			text += "in float" + std::to_string(settings.num_vertex_weights) + " Indices : BLENDINDICES";
			text += delim;
			//text += std::format("in float{0} Weights", settings.num_vertex_weights);
			text += "in float" + std::to_string(settings.num_vertex_weights) + " Weights";
		}
		else
		{
			if (settings.num_vertex_weights == 1)
			{
				text += delim;
				text += "in float Indices : BLENDINDICES";
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
				//text += std::format("out float{0} vTexcoord{1} : TEXCOORD{2}", slot.vec_size, i, num_texcoords);
				text += "out float" + std::to_string(slot.vec_size) + " vTexcoord" + std::to_string(i) + " : TEXCOORD" + std::to_string(num_texcoords);
				++num_texcoords;
			}
		}
		if (settings.use_normals)
		{
			text += delim;
			//text += std::format("out float{0} vNormal : TEXCOORD{1}", settings.fragment_normal_vec_size, num_texcoords);
			text += "out float" + std::to_string(settings.fragment_normal_vec_size) + " vNormal : TEXCOORD" + std::to_string(num_texcoords);
			++num_texcoords;
		}
		if (settings.use_tangents)
		{
			text += delim;
			//text += std::format("out float3 vTangent : TEXCOORD{0}", num_texcoords);
			text += "out float3 vTangent : TEXCOORD" + std::to_string(num_texcoords);
			++num_texcoords;
			text += delim;
			//text += std::format("out float3 vBinormal : TEXCOORD{0}", num_texcoords);
			text += "out float3 vBinormal : TEXCOORD" + std::to_string(num_texcoords);
			++num_texcoords;
		}
		if (settings.view_matrix)
		{
			text += delim;
			//text += std::format("out float3 vView : TEXCOORD{0}", num_texcoords);
			text += "out float3 vView : TEXCOORD" + std::to_string(num_texcoords);
			++num_texcoords;
		}
		if (settings.requires_screenpos)
		{
			text += delim;
			//text += std::format("out float4 vScreenPos : TEXCOORD{0}", num_texcoords);
			text += "out float4 vScreenPos : TEXCOORD" + std::to_string(num_texcoords);
			++num_texcoords;
		}
		if (settings.receive_shadows)
		{
			text += delim;
			//text += std::format("out float4 vShadowMapCoord : TEXCOORD{0}", num_texcoords);
			text += "out float4 vShadowMapCoord : TEXCOORD" + std::to_string(num_texcoords);
			++num_texcoords;
		}
		if (settings.fog && !settings.texlayer_1.colorsampler.enabled)
		{
			text += delim;
			//text += std::format("out float vFogFactor : TEXCOORD{0}", num_texcoords);
			text += "out float vFogFactor : TEXCOORD" + std::to_string(num_texcoords);
			++num_texcoords;
		}

		text += delim;
		text += "out float4 vHPosition : POSITION";
		text += " ) {\n";

		ss << text;
	}
};

class GenerateTemporaries : public LogicalBlock
{
public:
	void generateText(const FactorySettings& settings, std::stringstream& ss) override
	{
		//std::string text;

		//text += std::format("\tfloat4 {0};\n", "TmpPosition");
		ss << "\tfloat4 TmpPosition;\n";
		if (settings.use_normals)
			//text += std::format("\tfloat3 {0};\n", "TmpNormal");
			ss << "\tfloat3 TmpNormal;\n";
		if (settings.use_tangents)
		{
			//text += std::format("\tfloat3 {0};\n", "TmpTangent");
			//text += std::format("\tfloat3 {0};\n", "TmpBinormal");
			ss << "\tfloat3 TmpTangent;\n";
			ss << "\tfloat3 TmpBinormal;\n";
		}

		//return text;
	}

};


enum class SumState
{
	NoSum = 0,
	Sum = 1,
	SpacedNoSum = 2
};



void makeJointTransform(const FactorySettings& settings, std::stringstream& ss, const SumState& sum_state, std::function<std::string(const std::string&)> matrix_template, const std::string& input_position, const std::string& tmp_position, const std::string& output_position, const std::string& op, const std::string& dim)
{
	//std::string text;
	//text += std::format("\t{0}[0] = {1};", "jointMatrix", std::format(matrix_template, "0"));
	//text += std::format("\t{0}[1] = {1};", "jointMatrix", std::format(matrix_template, "1"));
	//text += std::format("\t{0}[2] = {1};", "jointMatrix", std::format(matrix_template, "2"));
	ss << "\tjointMatrix[0] = " << matrix_template("0") << ";";
	ss << "\tjointMatrix[1] = " << matrix_template("1") << ";";
	ss << "\tjointMatrix[2] = " << matrix_template("2") << ";";

	std::function<std::string(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&)> matrix_application_template;
	switch (sum_state)
	{
	case SumState::NoSum:
		//matrix_application_template = "\t{3} = mul({0},{1}.xyz);\n";
		matrix_application_template = [](const std::string& a, const std::string& b, const std::string& c, const std::string& d, const std::string& e, const std::string& f, const std::string& g)
		                                { return "\t" + c + " = mul(" + a + ", " + b + ");;\n"; };
		//text += std::format("\t{2} = mul({0}, {1});;\n", "jointMatrix", input_position, output_position);
		ss << "\t" << output_position << " = mul(jointMatrix, " << input_position << ");;\n";
		break;
	case SumState::Sum:
		//matrix_application_template += "\t{2}.xyz = mul({0},{1}.xyz);\n";
		//matrix_application_template += "\t{3}.xyz {4}= {2}.xyz*{6}.{5};\n";
		matrix_application_template = [](const std::string& a, const std::string& b, const std::string& c, const std::string& d, const std::string& e, const std::string& f, const std::string& g)
		                                { return "\t" + c + ".xyz = mul(" + a + "," + b + ".xyz);\n" + "\t" + d + ".xyz " + e + "= " + c + ".xyz*" + g + "." + f + ";\n"; };

		//text += std::format("\t{2} = mul({0}, {1});;\n", "jointMatrix", input_position, tmp_position);
		//text += std::format("\t{4} {0}= {3}*{2}.{1};\n", op, dim, "Weights", tmp_position, output_position);
		ss << "\t" << tmp_position << " = mul(jointMatrix, " << input_position << ");;\n";
		ss << "\t" << output_position << " " << op << "= " << tmp_position << "*Weights." << dim << ";\n";
		break;
	default:
		throw std::exception("Bad joint");
	}

	if (settings.use_normals)
	{
		ss << matrix_application_template("(float3x3)jointMatrix", "Normal", "tmp", "TmpNormal", op, dim, "Weights");
		if (settings.use_tangents)
		{
			ss << matrix_application_template("(float3x3)jointMatrix", "Tangent", "tmp", "TmpTangent", op, dim, "Weights");
			if (!settings.calculate_binormal)
				ss << matrix_application_template("(float3x3)jointMatrix", "Binormal", "tmp", "TmpBinormal", op, dim, "Weights");
		}
	}

}


void makeBillboardTransform(const FactorySettings& settings, std::stringstream& ss, const SumState& sum_state, std::function<std::string(const std::string&)> matrix_template, const std::string& input_position, const std::string& tmp_position, const std::string& output_position, const std::string& op, const std::string& dim)
{
	const static std::string scale = "wscale";
	const static std::string trans = "wtrans";

	//std::string palette_0 = std::format(matrix_template, "0");
	//std::string palette_1 = std::format(matrix_template, "1");
	//std::string palette_2 = std::format(matrix_template, "2");
	std::string palette_0 = matrix_template("0");
	std::string palette_1 = matrix_template("1");
	std::string palette_2 = matrix_template("2");

	//std::string text = "";
	//text += std::format("\t{0} = float3(length( float3({2}.x,{3}.x,{4}.x) ), "
	//	"length( float3({2}.y,{3}.y,{4}.y) ), "
	//	"length( float3({2}.z,{3}.z,{4}.z) ));"
	//	"\t{1} = float3({2}.w,{3}.w,{4}.w);;\n",
	//	scale, trans, palette_0, palette_1, palette_2);

	ss << "\t" << scale << " = float3(length( float3(" << palette_0 << ".x," << palette_1 << ".x," << palette_2 << ".x) ), "
		                          << "length( float3(" << palette_0 << ".y," << palette_1 << ".y," << palette_2 << ".y) ), "
		                          << "length( float3(" << palette_0 << ".z," << palette_1 << ".z," << palette_2 << ".z) ));"
	   << "\t" << trans << " = float3(" << palette_0 << ".w," << palette_2 << ".w," << palette_2 << ".w);;\n";

	switch (sum_state)
	{
	case SumState::NoSum:
		//text += std::format("\t{0}.xyz = {1}.xyz*{2} + {3};\n", output_position, input_position, scale, trans);
		ss << "\t" << output_position << ".xyz = " << input_position << ".xyz*" << scale << " + " << trans << ";\n";
		break;
	case SumState::SpacedNoSum:
		//text += std::format("\t{0}.xyz = {1}.xyz * {2} + {3};\n", output_position, input_position, scale, trans);
		ss << "\t" << output_position << ".xyz = " << input_position << ".xyz * " << scale << " + " << trans << ";\n";
		break;
	case SumState::Sum:
		//text += std::format("\t{0}.xyz = {1}.xyz*{2} + {3};\n", tmp_position, input_position, scale, trans);
		//text += std::format("\t{0}.xyz {4}= {1}*{2}.{3};\n", output_position, tmp_position, "Weights", dim, op);
		ss << "\t" << tmp_position << ".xyz = " << input_position << ".xyz*" << scale << " + " << trans << ";\n";
		ss << "\t" << output_position << ".xyz " << op << "= " << tmp_position << "*" << "Weights" << "." << dim << ";\n";
		break;
	default:
		throw std::exception("Bad billboard");
	}

	if (op == "")
		//text += std::format("\t{0}.w = 1.0;\n", output_position);
		ss << "\t" << output_position << ".w = 1.0;\n";

	//return text;
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

	void generateText(const FactorySettings& settings, std::stringstream& ss) override
	{
		//std::string text;

		std::string input_template;

		std::function<std::string(const std::string&)> inputTemplate;
		if (pre_normalize)
			//input_template = "normalize({0}.xyz)";
			inputTemplate = [](const std::string& val) { return "normalize(" + val + ".xyz)"; };
		else
			//input_template = "{0}";
			inputTemplate = [](const std::string& val) { return val; };

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
			//text += std::format("\t{0}.xyz = {1};\n", out_norm, std::format(input_template, this->norm));
			ss << "\TmpNormal.xyz = " << inputTemplate(norm) << ";\n";
			if (settings.use_tangents)
			{
				if (settings.calculate_binormal)
				{
					//text += std::format("\t{0}.xyz = normalize({2}.xyz - {1}.xyz * dot({1}.xyz, {2}.xyz)) * {3}.w;\n", out_tan, input_norm, input_tan, "Tangent");
					//text += std::format("\t{0}.xyz = cross({1}.xyz, {2}.xyz) * {3}.w;\n", out_bin, input_norm, input_tan, "Tangent");

				}
				else
				{
					//text += std::format("\t{0}.xyz = {1};\n", out_tan, std::format(input_template, tan));
					//text += std::format("\t{0}.xyz = {1};\n", out_bin, std::format(input_template, bin));
					ss << "\tTmpTangent.xyz = " << inputTemplate(tan) << ";\n";
					ss << "\TmpBinormal.xyz = " << inputTemplate(bin) << ";\n";
				}
			}
		}
	}
};

void noAdjustViewInverse(const FactorySettings& settings, std::stringstream& ss)
{
	ss << "\tfloat4x4 jointMatrix;\n";
	ss << "\tjointMatrix[3]=float4(0.0,0.0,0.0,1.0);\n";

	ss << "\tfloat4 pos = Position;\n";

	if (settings.fat)
		ss << "\tpos.xyz += Normal.xyz*Fat;\n";

}

void adjustViewInverse(const FactorySettings& settings, std::stringstream& ss)
{
	std::string view_inverse = "ViewInverse";
	const static std::string camPos = "camPos";
	if (settings.align_with_y)
	{
		//text += std::format("\tfloat3x3 {0};\n", camPos);
		//text += std::format("\t{0}[2] = normalize( float3({1}[2].x, 0.0, {1}[2].z));\n", camPos, view_inverse);
		//text += std::format("\t{0}[0] = normalize( cross(float3(0.0,1.0,0.0), {0}[2]) );\n", camPos);
		//text += std::format("\t{0}[1] = cross({0}[2],{0}[0]);\n", camPos);
		ss << "\tfloat3x3 camPos;\n";
		ss << "\tcamPos[2] = normalize( float3(ViewInverse[2].x, 0.0, ViewInverse[2].z));\n";
		ss << "\tcamPos[0] = normalize( cross(float3(0.0,1.0,0.0), camPos[2]) );\n";
		ss << "\tcamPos[1] = cross(camPos[2],camPos[0]);\n";
		view_inverse = camPos;
	}
	else
		//view_inverse = std::format("(float3x3){0}", view_inverse);
		view_inverse = "(float3x3)ViewInverse";


	//text += "\tfloat4 InPos;\n";
	ss << "\tfloat4 InPos;\n";

	//std::string view_template = std::format("\t{3}{2} = mul({0},{1}.xyz);\n", view_inverse, "{0}", "{1}", "{2}");
	if (settings.fat)
	{
		//text += "\tfloat4 pos = Position;\n";
		//text += "\tpos.xyz += Normal.xyz*Fat;\n";
		//text += std::format(view_template, "pos", "InPos.xyz", "");
		ss << "\tfloat4 pos = Position;\n";
		ss << "\tpos.xyz += Normal.xyz*Fat;\n";
		ss << "\tInPos.xyz = mul(" << view_inverse << ",pos.xyz);\n";
	}
	else
	{
		//text += std::format(view_template, "Position", "InPos.xyz", "");
		ss << "\tInPos.xyz = mul(" << view_inverse << ",Position.xyz);\n";
	}

	//text += "\tInPos.w = 1.0;\n";
	ss << "\tInPos.w = 1.0;\n";

	if (settings.use_normals)
	{
		//text += std::format(view_template, "Normal", "InNom", "float3 ");
		ss << "\tfloat3 InNom = mul(" << view_inverse << ",Normal.xyz);\n";
		if (settings.use_tangents)
		{
			//text += std::format(view_template, "Tangent", "InTan", "float3 ");
			ss << "\tfloat3 InTan = mul(" << view_inverse << ",Tangent.xyz);\n";
			if (!settings.calculate_binormal)
				//text += std::format(view_template, "Binormal", "InBin", "float3 ");
				ss << "\tfloat3 InBin = mul(" << view_inverse << ",Binormal.xyz);\n";
		}
	}

	//text += "\tfloat3 wscale;\n";
	//text += "\tfloat3 wtrans;\n";
	ss << "\tfloat3 wscale;\n";
	ss << "\tfloat3 wtrans;\n";
}

class TransformVertices : public LogicalBlock
{
public:
	void generateText(const FactorySettings& settings, std::stringstream& ss) override
	{
		const static std::array<std::string, 4> ops{ "", "+", "+", "+" };
		const static std::array<std::string, 4> dims{ "x", "y", "z", "w" };
		//std::string text;

		std::string tmp_type;
		std::function<std::string(const std::string&)> no_index_template;
		decltype(makeBillboardTransform)* fn;
		std::string input_position;
		std::string input_normal;
		std::string input_tangent;
		std::string input_binormal;
		if (settings.is_billboard)
		{
			tmp_type = "3";
			fn = makeBillboardTransform;
			//no_index_template = "MatrixPalette[0+{0}]";
			no_index_template = [](const std::string& idx) { return "MatrixPalette[0+" + idx + "]"; };
			input_position = "InPos";
			input_normal = "InNom";
			input_tangent = "InTan";
			input_binormal = "InBin";
		}
		else
		{
			tmp_type = "4";
			fn = makeJointTransform;
			//no_index_template = "MatrixPalette[ 0 + {0} ]";
			no_index_template = [](const std::string& idx) { return "MatrixPalette[ 0 + " + idx + " ]"; };
			input_position = "pos";
			input_normal = "TmpNormal";
			input_tangent = "TmpTangent";
			input_binormal = "TmpBinormal";
		}

		if (settings.use_skeleton)
		{
			if (settings.is_billboard)
				adjustViewInverse(settings, ss);
			else
				noAdjustViewInverse(settings, ss);

			if (settings.use_weights)
			{
				//text += std::format("\tfloat{1} {0};\n", "tmp", tmp_type);
				//text += std::format("\tfloat{0} {1} = {2};\n", settings.num_vertex_weights, "idx", "Indices");
				ss << "\tfloat" << tmp_type << " tmp;\n";
				ss << "\tfloat" << std::to_string(settings.num_vertex_weights) << " idx = Indices;\n";

				for (size_t i = 0; i < settings.num_vertex_weights; ++i)
				{
					auto& op = ops[i];
					auto& dim = dims[i];

					fn(settings, ss, SumState::Sum, [=](const std::string& idx) { return "MatrixPalette[ idx." + dim + " + " + idx + " ]"; }, input_position, "tmp", "TmpPosition", op, dim);
				}

			}

			else
			{
				if (settings.is_billboard)
					//text += std::format("\tfloat4 {0} = {1};\n", "pos", input_position);
					ss << "\tfloat4 pos = " << input_position << ";\n";
				//text += std::format("\tfloat {0} = {1}.w;\n", "idx", "Position");
				//text += std::format("\t{0}.w=1.0;\n", "pos");
				ss << "\tfloat idx = Position.w;\n";
				ss << "\tpos.w=1.0;\n";

				if (settings.num_vertex_weights)
					fn(settings, ss, SumState::NoSum, [](const std::string& idx) { return "MatrixPalette[ idx + " + idx + " ]"; }, "pos", "", "TmpPosition", "", "");
				else
					fn(settings, ss, SumState::NoSum, no_index_template, "pos", "", "TmpPosition", "", "");
			}

		}
		else
		{
			if (settings.is_billboard)
				//text += std::format("\t{0} = {1};\n", "TmpPosition", input_position);
				ss << "\tTmpPosition = " << input_position << ";\n";
			else
				//text += std::format("\t{0} = {1};\n", "TmpPosition", "Position");
				ss << "\tTmpPosition = Position;\n";
		}

		NormalizeVectors normalizer(input_normal, input_tangent, input_binormal, settings.use_skeleton, settings.is_billboard);
		normalizer.generateText(settings, ss);

	}
};


class HandleWorldTransform : public LogicalBlock
{
public:
	void generateText(const FactorySettings& settings, std::stringstream& ss) override
	{
		//std::string text;

		if (settings.apply_world_matrix)
		{
			if (settings.is_billboard)
			{
				makeBillboardTransform(settings, ss, SumState::SpacedNoSum, [](const std::string& idx) { return "World[" + idx + "]"; }, "TmpPosition", "", "TmpPosition", "", "");
				NormalizeVectors normalizer("TmpNormal", "TmpTangent", "TmpBinormal", true, false);
				normalizer.generateText(settings, ss);
			}
			else
			{
				//text += std::format("\t{0} = mul({1}, {0});\n", "TmpPosition", "World");
				ss << "\tWorld = mul(TmpPosition, World);\n";
				//std::string matrix_application_template = "\t{2}.xyz = normalize(mul({0}.xyz, (float3x3){1}));\n";
				if (settings.use_normals)
					//text += std::format(matrix_application_template, "TmpNormal", "WorldInverse", "vNormal");
					ss << "\tvNormal.xyz = normalize(mul(TmpNormal.xyz, (float3x3)WorldInverse));\n";
				if (settings.use_tangents)
				{
					//text += std::format(matrix_application_template, "TmpTangent", "WorldInverse", "vTangent");
					//text += std::format(matrix_application_template, "TmpBinormal", "WorldInverse", "vBinormal");
					ss << "\tvTangent.xyz = normalize(mul(TmpTangent.xyz, (float3x3)WorldInverse));\n";
					ss << "\tvBinormal.xyz = normalize(mul(TmpBinormal.xyz, (float3x3)WorldInverse));\n";
				}
			}
		}
		else
		{
			std::string basic_assignment_template = "\t{1} = {0}.xyz;\n";
			if (settings.use_normals)
				//text += std::format(basic_assignment_template, "TmpNormal", "vNormal.xyz");
				ss << "\tvNormal.xyz = TmpNormal.xyz;\n";
			if (settings.use_tangents)
			{
				//text += std::format(basic_assignment_template, "TmpTangent", "vTangent");
				//text += std::format(basic_assignment_template, "TmpBinormal", "vBinormal");
				ss << "\tvTangent = TmpTangent.xyz;\n";
				ss << "\tvBinormal = TmpBinormal.xyz;\n";
			}
		}

		//text += std::format("\t{2} = mul({0},{1});\n", "ViewProj", "TmpPosition", "vHPosition");
		ss << "\tvHPosition = mul(ViewProj,TmpPosition);\n";
	}

};

template<size_t slot>
class GenerateUVTransforms : public LogicalBlock
{
public:
	void generateText(const FactorySettings& settings, std::stringstream& ss) override
	{
		//std::string in_tex_coord = std::format("Texcoord{0}", slot);
		//std::string out_tex_coord = std::format("vTexcoord{0}", slot);
		std::string in_tex_coord = "Texcoord" + std::to_string(slot);
		std::string out_tex_coord = "vTexcoord" + std::to_string(slot);
		std::string position_variable = "vHPosition";
		auto& uv_slot = settings.uv_slots[slot];

		//std::string text;
		if (uv_slot.enabled)
		{
			// Projection
			if (uv_slot.is_projection)
			{
				//text += std::format("\t{0}.xy = {1}.xy/{1}.w;\n", out_tex_coord, position_variable);
				ss << "\t" << out_tex_coord << ".xy = " << position_variable << ".xy/" << position_variable << ".w;\n";
			}
			else
			{
				if constexpr (slot > 0)
					//text += std::format("\t{1} = {0}.xy;\n", in_tex_coord, out_tex_coord);
					ss << "\t" << out_tex_coord << " = " << in_tex_coord << ".xy;\n";
				else
					//text += std::format("\t{1}.xy = {0};\n", in_tex_coord, out_tex_coord);
					ss << "\t" << out_tex_coord << ".xy = " << in_tex_coord << ";\n";
			}

			// Scroll
			if (uv_slot.scroll)
			{
				//std::string scroll_variable = std::format("ScrollSpeedSet{0}", slot + 1);
				std::string scroll_variable = "ScrollSpeedSet" + std::to_string(slot + 1);
				std::string time_variable = "Time";
				//text += std::format("\t{0}.xy += frac({1} * {2});\n", out_tex_coord, scroll_variable, time_variable);
				ss << "\t" << out_tex_coord << ".xy += frac(" << scroll_variable << " * Time);\n";
			}

			// Rotation
			if (uv_slot.rotation)
			{
				//std::string rotation_variable = std::format("RotationSet{0}", slot + 1);
				std::string rotation_variable = "RotationSet" + std::to_string(slot + 1);
				std::string cosine_variable = "c";
				std::string sine_variable = "s";
				//text += std::format("\tfloat {1} = cos({0});\n", rotation_variable, cosine_variable);
				//text += std::format("\tfloat {1} = sin({0});\n", rotation_variable, sine_variable);
				//text += std::format("\t{0}.xy = float2({0}.x * {1} - {0}.y * {2}, {0}.x * {2} + {0}.y * {1});\n", out_tex_coord, cosine_variable, sine_variable);
				ss << "\tfloat c = cos(" << rotation_variable << ");\n";
				ss << "\tfloat s = sin(" << rotation_variable << ");\n";
				ss << "\t" << out_tex_coord << ".xy = float2(" << out_tex_coord << ".x * c - " << out_tex_coord << ".y * s, " << out_tex_coord << ".x * s + " << out_tex_coord << ".y * c);\n";
			}

			// Translation
			if (uv_slot.offset)
			{
				//std::string offset_variable = std::format("OffsetSet{0}", slot + 1);
				//text += std::format("\t{0}.xy = {0}.xy + {1};\n", out_tex_coord, offset_variable);
				std::string scale_variable = "OffsetSet" + std::to_string(slot + 1);
				ss << "\t" << out_tex_coord << ".xy = " << out_tex_coord << ".xy * " << scale_variable << ";\n";
			}

			// Scale
			if (uv_slot.scale)
			{
				//std::string scale_variable = std::format("ScaleSet{0}", slot + 1);
				//text += std::format("\t{0}.xy = {0}.xy * {1};\n", out_tex_coord, scale_variable);
				std::string scale_variable = "ScaleSet" + std::to_string(slot + 1);
				ss << "\t" << out_tex_coord << ".xy = " << out_tex_coord << ".xy * " << scale_variable << ";\n";
			}
		}
	}
};

class OutputVertexColors : public LogicalBlock
{
public:
	void generateText(const FactorySettings& settings, std::stringstream& ss) override
	{
		//std::string text;
		if (settings.use_vertex_colors)
		{
			std::string outcolor_variable = "vColor";
			std::string incolor_variable = "Color";

			//text += std::format("\t{0} = {1};\n", outcolor_variable, incolor_variable);
			ss << "\tvColor = Color;\n";
		}
	}
};

class GenerateScreenpos : public LogicalBlock
{
public:
	void generateText(const FactorySettings& settings, std::stringstream& ss) override
	{
		//std::string text;
		if (settings.requires_screenpos)
		{
			std::string screenpos_variable = "vScreenPos";
			std::string position_variable = "vHPosition";

			//text += std::format("\t{0} = {1};\n", screenpos_variable, position_variable);
			//text += std::format("\t{0}.xy = {0}.xy * 0.5 + (0.5*{1}.w);\n", screenpos_variable, position_variable);
			ss << "\tvScreenPos = vHPosition;\n";
			ss << "\tvScreenPos.xy = vScreenPos.xy * 0.5 + (0.5*vHPosition.w);\n";
		}
	}
};

class GenerateShadows : public LogicalBlock
{
public:
	void generateText(const FactorySettings& settings, std::stringstream& ss) override
	{
		//std::string text;
		if (settings.receive_shadows)
		{
			std::string shadowmap_variable = "vShadowMapCoord";
			std::string light_viewproj = "LightViewProj";
			std::string position_variable = "TmpPosition";

			//text += std::format("\t{0} = mul({1}, float4({2}.xyz,1.0));\n", shadowmap_variable, light_viewproj, position_variable);
			//text += std::format("\t{0}.x *= 0.5;\n", shadowmap_variable);
			//text += std::format("\t{0}.y *= 0.5;\n", shadowmap_variable);
			//text += std::format("\t{0}.xy += {0}.w * 0.5;\n", shadowmap_variable);
			ss << "\tvShadowMapCoord = mul(LightViewProj, float4(TmpPosition.xyz,1.0));\n";
			ss << "\tvShadowMapCoord.x *= 0.5;\n";
			ss << "\tvShadowMapCoord.y *= 0.5;\n";
			ss << "\tvShadowMapCoord.xy += vShadowMapCoord.w * 0.5;\n";
		}

	}
};

class GenerateFog : public LogicalBlock
{
public:
	void generateText(const FactorySettings& settings, std::stringstream& ss) override
	{
		//std::string text;
		if (settings.fog)
		{
			std::string fog_variable;
			std::string position_variable;
			std::string fog_params = "FogParams";

			// Set Fog Variable
			if (settings.use_normals)
				fog_variable = "vNormal.w";
			else if (settings.texlayer_1.colorsampler.enabled)
				//fog_variable = std::format("vTexcoord{0}.z", settings.texlayer_1.colorsampler.uv_slot);
				fog_variable = "vTexcoord" + std::to_string(settings.texlayer_1.colorsampler.uv_slot) + ".z";
			else
				fog_variable = "vFogFactor";

			// Set Position Variable
			if (settings.fog_height)
				position_variable = "TmpPosition.y";
			else
				position_variable = "vHPosition.w";

			// Generate text
			//text += std::format("\t{0} = 1.0 - ({1}.x + {2} * {1}.y);\n", fog_variable, fog_params, position_variable);
			//text += std::format("\t{0} = clamp({0},0.0,1.0)*{1}.z;\n", fog_variable, fog_params);
			ss << "\t" << fog_variable << " = 1.0 - (" << fog_params << ".x + " << position_variable << " * " << fog_params << ".y);\n";
			ss << "\t" << fog_variable << " = clamp(" << fog_variable << ",0.0,1.0)*" << fog_params << ".z;\n";
		}

	}
};

class OutputViewMatrix : public LogicalBlock
{
public:
	void generateText(const FactorySettings& settings, std::stringstream& ss) override
	{
		//std::string text;
		if (settings.view_matrix)
			// Inputs: CameraPosition, TmpPosition
			// Outputs: vView
			// Flags: view_matrix
			//text += std::format("\t{0} = normalize({1} - {2}.xyz);\n", "vView", "CameraPosition", "TmpPosition");
			ss << "\tvView = normalize(CameraPosition - TmpPosition.xyz);\n";
	}
};

class BiasOutputPosition : public LogicalBlock
{
public:
	void generateText(const FactorySettings& settings, std::stringstream& ss) override
	{
		if (settings.zbias)
			// Inputs: vHPosition, ZBias
			// Outputs: vHPosition
			// Flags: zbias
			//text += std::format("\t{0}.z = {1}.z + {2}*0.3;\n", "vHPosition", "vHPosition", "ZBias");
			ss << "\tvHPosition.z = vHPosition.z + ZBias*0.3;\n";
	}
};

std::string generateVertexShader(const FactorySettings& settings)
{
	std::stringstream ss;
	// Generate Header
	GenerateHeader header_gen;
	header_gen.generateText(settings, ss);
	// Generate Temporaries
	GenerateTemporaries temps_gen;
	temps_gen.generateText(settings, ss);
	// TransformVertices
	TransformVertices trans_verts;
	trans_verts.generateText(settings, ss);
	HandleWorldTransform hwt;
	hwt.generateText(settings, ss);
	// Transform UVs
	GenerateUVTransforms<0> gut1;
	gut1.generateText(settings, ss);
	GenerateUVTransforms<1> gut2;
	gut2.generateText(settings, ss);
	GenerateUVTransforms<2> gut3;
	gut3.generateText(settings, ss);
	// Pass Color
	OutputVertexColors ovc;
	ovc.generateText(settings, ss);
	// Make Screenpos
	GenerateScreenpos screenpos_gen;
	screenpos_gen.generateText(settings, ss);
	// GenerateShadows
	GenerateShadows shadow_gen;
	shadow_gen.generateText(settings, ss);
	// GenerateFog
	GenerateFog fog_gen;
	fog_gen.generateText(settings, ss);
	// Output ViewMatrix
	OutputViewMatrix ovm;
	ovm.generateText(settings, ss);
	// BiasOutputPosition
	BiasOutputPosition bop;
	bop.generateText(settings, ss);
	// Finalise shader
	ss << "}\n";

	return ss.str();
}
