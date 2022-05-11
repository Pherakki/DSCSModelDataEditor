#pragma once

#include "BaseWidgets/TitleWidget.hpp"

class GlassMapSettings : public QWidget
{
public:
	TitleWidget* title_widget;
	GlassMapSettings(QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto _layout = new QVBoxLayout;
		{
			this->title_widget = new TitleWidget("Glass Map", this);

			_layout->addWidget(this->title_widget);
		}
		this->setLayout(_layout);
	}
};
