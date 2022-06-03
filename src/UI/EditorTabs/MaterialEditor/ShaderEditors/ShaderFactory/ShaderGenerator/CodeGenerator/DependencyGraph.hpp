#pragma once

#include "Node.hpp"

class DependencyGraph
{
private:
	std::vector<Node> nodes;
	std::vector<std::vector<Node*>> node_ranks; // Not sure on what type this will end up being yet, consider this a placeholder
};
