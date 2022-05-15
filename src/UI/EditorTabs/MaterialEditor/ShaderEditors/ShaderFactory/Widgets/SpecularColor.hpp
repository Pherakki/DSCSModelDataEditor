#pragma once

#include "BaseWidgets/TitleWidget.hpp"
#include "BaseWidgets/ToggleableCombobox.hpp"
#include "TextureMap.hpp"

class SpecularColorSettings : public QWidget
{
	Q_OBJECT
public:
	QCheckBox* checkbox;
	TitleWidget* title_widget;
	TextureMapWidget* specular_map;
	//ToggleableCombobox* overlay_channel;
	TextboxArrayWidget<1>* specular_power = new TextboxArrayWidget<1>("Specular Power", this);
	TextboxArrayWidget<1>* specular_strength = new TextboxArrayWidget<1>("Specular Strength", this);
	SpecularColorSettings(QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto _layout = new QGridLayout;
		{
			this->checkbox = new QCheckBox(this);
			this->title_widget = new TitleWidget("Specular Color", this);

			_layout->addWidget(this->checkbox, 0, 0);
			_layout->addWidget(this->title_widget, 0, 1);

			auto settings_layout = new QVBoxLayout;
			{
				this->specular_power->textboxes[0]->setText("0.0");
				this->specular_strength->textboxes[0]->setText("0.0");
				this->specular_map = new TextureMapWidget("Specular Map", this);
				//this->overlay_channel = new ToggleableCombobox("Overlay Channel", this);

				settings_layout->addWidget(this->specular_power);
				settings_layout->addWidget(this->specular_strength);
				settings_layout->addWidget(this->specular_map);
				//settings_layout->addWidget(this->overlay_channel);
			}
			_layout->addItem(settings_layout, 1, 1);
		}
		this->setLayout(_layout);

		connect(this->checkbox, &QCheckBox::stateChanged, this, &SpecularColorSettings::settingsUpdated);
		connect(this->specular_map, &TextureMapWidget::settingsUpdated, this, &SpecularColorSettings::settingsUpdated);
	}
signals:
	void settingsUpdated(bool);
};
