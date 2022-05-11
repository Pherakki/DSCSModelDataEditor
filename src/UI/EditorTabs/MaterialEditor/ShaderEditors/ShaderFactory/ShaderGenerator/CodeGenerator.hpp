#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Utils.hpp"

class Variable
{
private:
	typedef bool(*ConditionFunction)(FactorySettings&);
	typedef std::vector<ConditionFunction> ConditionVector;
	uint32_t id;
	ConditionVector conditions;
public:
	std::string type;
	std::string name;

	Variable(uint32_t id, const std::string& type, const std::string& name)
		: id{ id }
		, type{ type }
		, name{ name }
	{}

	const decltype(Variable::id)& getID() const
	{
		return this->id;
	}

	bool operator==(const Variable& other) const
	{
		return this->id == other.getID();
	}

	void addCondition(const ConditionFunction& condition)
	{
		this->conditions.push_back(condition);
	}
};

class VariableReference
{
public:
	typedef std::shared_ptr<Variable> VarPtr;
	typedef bool(*ConditionFunction)(FactorySettings&);

	VarPtr variable = nullptr;
	const ConditionFunction condition;

	VariableReference(ConditionFunction fn) : condition{fn} {};

	void setVariable(VarPtr& variable)
	{
		this->variable = variable;
		this->variable->addCondition(this->condition);
	}
};

// Can contain sub-blocks...
class LogicalBlock
{
protected:
	typedef std::vector<VariableReference> VarRefVec;
	typedef std::shared_ptr<Variable> VariablePtr;

	VarRefVec inputs;
	VarRefVec outputs;
public:
	virtual const VarRefVec& getInputs() { return this->inputs; };
	virtual const VarRefVec& getOutputs() { return this->outputs; };
	virtual std::string generateCode(const FactorySettings& settings) = 0;
};

// Might be an idea to implement later?
class Scope
{
private:
	typedef std::unique_ptr<LogicalBlock> LogicalBlockPtr;
	std::vector<LogicalBlockPtr> code_blocks;
};

class CodeGenerator
{
protected:
	typedef std::unique_ptr<LogicalBlock> LogicalBlockPtr;
	std::vector<LogicalBlockPtr> code_blocks;
public:
	void checkAllVariablesAreDefined()
	{

	}

	std::string generateCode()
	{
		std::string text;
		FactorySettings settings{};
		for (const auto& block : this->code_blocks)
		{
			text += block->generateCode(settings);
		}
		return text;
	}
};