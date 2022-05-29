#pragma once

#include <QWidget>

#include "BaseWidgets/ToggleableTextboxes.hpp

class TextureBlending : public QWidget
{
	Q_OBJECT;

private:
	ToggleableTextboxesWidget<1> overlay_strength;

};
