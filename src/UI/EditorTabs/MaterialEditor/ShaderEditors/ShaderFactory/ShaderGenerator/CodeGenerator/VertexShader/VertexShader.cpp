#include <sstream>

#include "../DependencyGraph.hpp"
#include "VertexShader.hpp"

class ZBias
{
	static std::vector<Node*> buildTree()
	{

	}

	static void buildCode(FactorySettings& settings, std::stringstream& ss)
	{
		std::string bias = "ZBias";
		std::string in_pos = "vHPosition";
		std::string out_pos = "vHPosition";
		if (settings.zbias)
			ss << "\t" << out_pos << ".z = " << in_pos << ".z + " << bias << "*0.3; \n";
	}
};



std::string generateVertexShader(const FactorySettings& settings)
{
	return "";
}
