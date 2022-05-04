#pragma once

#include "BaseWidgets/TitleWidget.hpp"
#include "BaseWidgets/ToggleableTextboxes.hpp"

class ShaderFactoryUVSettingsWidget : public QWidget
{
	Q_OBJECT;

public:
	TitleWidget* title;
	ToggleableTextboxesWidget<0>* widget_projection;
	ToggleableTextboxesWidget<2>* widget_scrollspeed;
	ToggleableTextboxesWidget<1>* widget_rotation;
	ToggleableTextboxesWidget<2>* widget_offset;
	ToggleableTextboxesWidget<2>* widget_scale;

	explicit ShaderFactoryUVSettingsWidget(const QString& label_text, QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto layout = new QVBoxLayout;

		this->title = new TitleWidget(label_text);
		layout->setAlignment(this->title, Qt::AlignHCenter);
		layout->addWidget(this->title);

		this->widget_projection = new ToggleableTextboxesWidget<0>("Projection");
		layout->addWidget(this->widget_projection);

		this->widget_scrollspeed = new ToggleableTextboxesWidget<2>("Scroll Speed");
		this->widget_rotation = new ToggleableTextboxesWidget<1>("Rotation");
		this->widget_offset = new ToggleableTextboxesWidget<2>("Offset");
		this->widget_scale = new ToggleableTextboxesWidget<2>("Scale");
		layout->addWidget(this->widget_scrollspeed);
		layout->addWidget(this->widget_rotation);
		layout->addWidget(this->widget_offset);
		layout->addWidget(this->widget_scale);

		this->setLayout(layout);
		this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	}
};
