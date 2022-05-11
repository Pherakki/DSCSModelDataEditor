#include "NewVertexShader.hpp"

#include "Blocks/ZBias.hpp"

class VertexShaderCodeGenerator : public CodeGenerator
{
	VertexShaderCodeGenerator()
	{
		auto outputPosition = std::make_shared<Variable>( 0, "float3", "vHPosition" );
		auto ZBias = std::make_shared<Variable>(1, "float", "ZBias");
		this->code_blocks.emplace_back(std::make_unique<BiasOutputPosition>(outputPosition, ZBias, outputPosition));
	}
};


std::string generateVertexShader()
{
	return "";
}