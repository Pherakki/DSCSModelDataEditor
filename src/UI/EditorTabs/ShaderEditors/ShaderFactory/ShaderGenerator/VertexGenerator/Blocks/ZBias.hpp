#include <format>

#include "../../CodeGenerator.hpp"


class BiasOutputPosition : public LogicalBlock
{
public:
	VariableReference inputPosition { [](FactorySettings& settings) { return settings.zbias; } };
	VariableReference inputZBias    { [](FactorySettings& settings) { return settings.zbias; } };
	VariableReference outputPosition{ [](FactorySettings& settings) { return settings.zbias; } };

	VarRefVec inputs  = { inputPosition, inputZBias };
	VarRefVec outputs = { outputPosition };

	std::string generateCode(const FactorySettings& settings) override
	{
		std::string text;
		if (settings.zbias)
			// Inputs: vHPosition, ZBias
			// Outputs: vHPosition
			// Flags: zbias
			text += std::format(
				"\t{0}.z = {1}.z + {2}*0.3;\n", 
				this->outputPosition.variable->name, 
				this->inputPosition.variable->name,
				this->inputZBias.variable->name);
		return text;
	}
};