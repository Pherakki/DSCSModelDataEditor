#pragma once

#include <string>

#include "Graveyard.hpp"

namespace Commands
{
	class ICommand
	{
	public:
		ICommand(const std::string& name, Graveyard& graveyard) : graveyard_ref{ graveyard } { this->message = name; };
		ICommand(const std::string& name, Graveyard&& graveyard) = delete;  // Makes sure nobody accidently assigns a temporary graveyard
		//virtual ~ICommand() = 0;
		virtual void execute() = 0;
		virtual void unexecute() = 0;
		virtual std::string getStringRepresentation() { return this->message; };
	private:
		std::string message;
		Graveyard& graveyard_ref;
	};

	class IIrreversibleCommand : public ICommand
	{
	public:
		IIrreversibleCommand(const std::string& name, Graveyard& graveyard);
	};
}