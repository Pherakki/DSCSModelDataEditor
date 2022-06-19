#include "../../CodeGenerator.hpp"


class BiasOutputPosition : public LogicalBlock
{
public:
	VariableReference inputPosition { [](FactorySettings& settings) { return settings.zbias; } };
	VariableReference inputZBias    { [](FactorySettings& settings) { return settings.zbias; } };
	VariableReference outputPosition{ [](FactorySettings& settings) { return settings.zbias; } };

	VarRefVec inputs  = { inputPosition, inputZBias };
	VarRefVec outputs = { outputPosition };

	BiasOutputPosition(const VariablePtr& inputPos, const VariablePtr& inputzbias, const VariablePtr& outputPos) : LogicalBlock()
	{
		this->inputPosition.variable = inputPos;
		this->inputZBias.variable = inputzbias;
		this->outputPosition.variable = outputPos;
	}

	std::string generateCode(const FactorySettings& settings) override
	{
		std::string text;
		if (settings.zbias)
			// Inputs: vHPosition, ZBias
			// Outputs: vHPosition
			// Flags: zbias
			text += "\t" + this->outputPosition.variable->name + ".z = " + this->inputPosition.variable->name + ".z + " + this->inputZBias.variable->name + "*0.3;\n";
		return text;
	}
};
