#pragma once

#include<deque>
#include<memory>

#include "ICommand.hpp"

namespace Commands
{
	class CommandManager
	{
	private:
		typedef std::deque<std::unique_ptr<ICommand>> ICommandDeque_t;

		uint8_t maximum_size;
		// Implement as a deque so we can pop elements from the start if the stacks get too long
		ICommandDeque_t undo_stack;
		ICommandDeque_t redo_stack;
		void cleanupStack(ICommandDeque_t& stack);
	public:
		CommandManager() { this->maximum_size = 50; };
		void addCommand(std::unique_ptr<ICommand> command);
		void addCommand(std::unique_ptr<IIrreversibleCommand> command);
		void undo();
		void redo();
	};
}