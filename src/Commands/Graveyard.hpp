#pragma once

#include <iostream>
#include <deque>
#include <memory>
#include <unordered_map>
#include <utility>

namespace Commands
{
	class Tombstone
	{
	public:
		~Tombstone() { std::cout << "Tombstone destructor called!" << std::endl; }
		virtual void resurrect() = 0;
	};

	class Graveyard
	{
	private:
		typedef uint16_t TombstoneID_t;
		std::unordered_map<TombstoneID_t, std::unique_ptr<Tombstone>> tombstones;
		std::deque<TombstoneID_t> free_plots;
	public:
		TombstoneID_t addTombstone(std::unique_ptr<Tombstone>&& tombstone)
		{
			TombstoneID_t id = this->tombstones.size();
			while (this->free_plots.size())
			{
				TombstoneID_t possible_id = this->free_plots.front();
				this->free_plots.pop_front();
				if (possible_id < id)
				{
					id = possible_id;
					break;
				}
			}

			this->tombstones[id] = std::move(tombstone);
		}

		void removeTombstone(TombstoneID_t id)
		{
			this->tombstones.erase(id);
			if (id < this->tombstones.size())
				this->free_plots.push_back(id);
		}

		void resurrectTombstone(TombstoneID_t id)
		{
			this->tombstones[id]->resurrect();
			this->removeTombstone(id);
		}
	};
}