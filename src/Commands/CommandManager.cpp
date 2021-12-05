#include "CommandManager.hpp"


namespace Commands
{
	void CommandManager::addCommand(std::unique_ptr<ICommand> command)
	{
		// Execute command...
		command->execute();
		// Push it to the stacks
		this->undo_stack.push_back(std::move(command));
		this->cleanupStack(this->undo_stack);
		this->redo_stack.clear();
	}

	void CommandManager::addCommand(std::unique_ptr<IIrreversibleCommand> command)
	{
		this->undo_stack.clear();
		this->redo_stack.clear();
		command->execute();
	}

	void CommandManager::undo()
	{
		// Undo command...
		this->undo_stack.back()->unexecute();
		// Move it to the redo stack
		this->redo_stack.push_back(std::move(this->undo_stack.back()));
		this->undo_stack.pop_back();
		this->cleanupStack(this->redo_stack);
	}

	void CommandManager::redo()
	{
		// Redo command...
		this->redo_stack.back()->execute();
		// Move it to the redo stack
		this->undo_stack.push_back(std::move(this->redo_stack.back()));
		this->redo_stack.pop_back();
		this->cleanupStack(this->undo_stack);
	}

	void CommandManager::cleanupStack(ICommandDeque_t& stack)
	{
		if (stack.size() > this->maximum_size)
		{
			for (size_t i = stack.size(); i > this->maximum_size; --i)
				stack.pop_front();
		}
	}
}