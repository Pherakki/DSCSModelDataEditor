#pragma once

#include <vector>

// Dummy for now; should provide a method that switches something on or off on a contained item
class Toggleable
{
	std::shared_ptr<ToggleableBase> internal_obj;

	struct ToggleableBase
	{
		virtual ~ToggleableBase() {}
	};

	template <class T>
	struct Holder : ToggleableBase
	{
		T& obj;
		Holder(T& obj) : obj{ obj } {}
		toggle(bool active) { this->obj.toggle(active) }
	};


public:
	template <class T>
	Toggleable(T obj) : internal_obj{ new Holder<T>(obj) }
	{}
};

class Node
{
private:
	int rank = -1;
	bool internal_flag = false;
	std::vector<Node*> parents;
	std::vector<Toggleable> subscribers;
};
