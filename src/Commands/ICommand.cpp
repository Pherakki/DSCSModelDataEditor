#include "ICommand.hpp"


namespace Commands
{
	IIrreversibleCommand::IIrreversibleCommand(const std::string& name, Graveyard& graveyard) : ICommand(name, graveyard)
	{
		// Need to pop a dialog box that warns about the command being irreversible
	}
}
