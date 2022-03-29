#pragma once

#include <iostream>

#include <vector>
#include <Qt>
#include <QCursor>

namespace Input
{
	class InputHandler
	{
	public:
		const QPoint& getMouseDelta() { return this->mouse_delta; }
		const QPoint& getMousePos()  { return this->mouse_position; }
		void updateMousePosition(QPoint pos) { this->mouse_delta = pos - this->mouse_position; this->mouse_position = pos; };
		void registerKey(int key_id) { this->active_keys.push_back(key_id); };
		void unregisterKey(int key_id) { this->active_keys.erase(std::remove_if(this->active_keys.begin(), this->active_keys.end(), [key_id](int const& key) { return key == key_id; }), this->active_keys.end());; };
		void printActiveKeys()
		{
			for (auto& key : this->active_keys)
				std::cout << key << " ";
			std::cout << std::endl;
		}

		// Various actions - should make these configurable
		bool shouldRotateCamera()    { return this->isKeyActive(Qt::RightButton); }
		bool shouldTranslateCamera() { return (this->isKeyActive(Qt::Key_Shift) && this->isKeyActive(Qt::RightButton)); }
	private:
		QPoint mouse_delta;
		QPoint mouse_position;
		std::vector<uint32_t> active_keys;
		bool isKeyActive(int key) { return std::find(std::begin(this->active_keys), std::end(this->active_keys), key) != this->active_keys.end(); };

	};
}