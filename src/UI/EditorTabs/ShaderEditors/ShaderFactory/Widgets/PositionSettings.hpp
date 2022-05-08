#pragma once

#include "BaseWidgets/TitleWidget.hpp"
#include "BaseWidgets/ToggleableTextboxes.hpp"

class PositionSettings : public QWidget
{
public:
	TitleWidget* title_widget;
	ToggleableTextboxesWidget<0>* calculate_binormal;
	ToggleableTextboxesWidget<1>* fat;
	ToggleableTextboxesWidget<1>* zbias;
	CheckableWidget* billboard_checkbox;
	CheckableWidget* billboard_align_y_checkbox;
	PositionSettings(QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto _layout = new QVBoxLayout;
		{
			this->title_widget = new TitleWidget("Vertex Adjust", this);
			this->calculate_binormal = new ToggleableTextboxesWidget<0>("Calculate Binormal", this);
			this->fat = new ToggleableTextboxesWidget<1>("Fat", this);
			this->zbias = new ToggleableTextboxesWidget<1>("Depth Bias", this);

			this->billboard_checkbox = new CheckableWidget("Billboard", this);
			QHBoxLayout* align_y_layout = new QHBoxLayout;
			{
				this->billboard_align_y_checkbox = new CheckableWidget("Align Y", this);
			}
			align_y_layout->addWidget(billboard_align_y_checkbox, 0, 0);
			this->billboard_checkbox->setContents(align_y_layout);

			_layout->addWidget(this->title_widget);
			_layout->addWidget(this->calculate_binormal);
			_layout->addWidget(this->fat);
			_layout->addWidget(this->zbias);
			_layout->addWidget(this->billboard_checkbox);
		}
		this->setLayout(_layout);
	}
};
