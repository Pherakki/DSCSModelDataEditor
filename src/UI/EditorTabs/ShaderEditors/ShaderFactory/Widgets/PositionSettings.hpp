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
	QCheckBox* billboard_checkbox;
	QCheckBox* billboard_align_y_checkbox;
	PositionSettings(QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto _layout = new QVBoxLayout;
		{
			this->title_widget = new TitleWidget("Vertex Adjust", this);
			this->calculate_binormal = new ToggleableTextboxesWidget<0>("Calculate Binormal", this);
			this->fat = new ToggleableTextboxesWidget<1>("Fat", this);
			this->zbias = new ToggleableTextboxesWidget<1>("Depth Bias", this);

			QGridLayout* billboard_layout = new QGridLayout;
			{
				this->billboard_checkbox = new QCheckBox(this);
				billboard_layout->addWidget(this->billboard_checkbox, 0, 0, Qt::AlignLeft);
				billboard_layout->addItem(new QSpacerItem(10, 1, QSizePolicy::Fixed, QSizePolicy::Fixed), 0, 1);
				billboard_layout->addWidget(new QLabel("Billboard"), 0, 2, Qt::AlignLeft);
				billboard_layout->setColumnStretch(0, 0);
				billboard_layout->setColumnStretch(1, 0);
				billboard_layout->setColumnStretch(2, 0);
				billboard_layout->setColumnStretch(3, 1);
				billboard_layout->addItem(new QSpacerItem(10, 1, QSizePolicy::Minimum, QSizePolicy::Expanding), 0, 3);
				billboard_layout->addItem(new QSpacerItem(1, 10, QSizePolicy::Fixed, QSizePolicy::Fixed), 1, 0, 1, 4);
				QHBoxLayout* align_y_layout = new QHBoxLayout;
				{
					this->billboard_align_y_checkbox = new QCheckBox(this);
					align_y_layout->addWidget(this->billboard_align_y_checkbox, 0, Qt::AlignLeft);
					align_y_layout->addItem(new QSpacerItem(10, 1, QSizePolicy::Fixed, QSizePolicy::Fixed));
					align_y_layout->addWidget(new QLabel("Align Y", this), 0, Qt::AlignLeft);
					auto spacer = new QSpacerItem(10, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
					align_y_layout->addItem(spacer);
				}
				billboard_layout->addItem(align_y_layout, 2, 2);
			}
			_layout->addWidget(this->title_widget);
			_layout->addWidget(this->calculate_binormal);
			_layout->addWidget(this->fat);
			_layout->addWidget(this->zbias);
			_layout->addItem(billboard_layout);
		}
		this->setLayout(_layout);
	}
};
