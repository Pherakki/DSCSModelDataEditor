#pragma once

#include "BaseWidgets/TitleWidget.hpp"

class IlluminationSettings : public QWidget
{
public:
	TitleWidget* title_widget;
	IlluminationSettings(QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto _layout = new QVBoxLayout;
		{
			this->title_widget = new TitleWidget("Illumination", this);

			_layout->addWidget(this->title_widget);
		}
		this->setLayout(_layout);
	}
};
