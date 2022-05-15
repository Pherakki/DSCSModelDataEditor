#pragma once

#include <QCheckBox>
#include <QLabel>
#include <QWidget>
#include <QValidator>

#include "BaseWidgets/TextboxArrayWidget.hpp"

class ShaderFactoryTextureLayerBumpmapBox : public QWidget
{
public:
	void toggle(bool active)
	{
		this->setEnabled(active);
		this->bump_strength->setEnabled(active);
	}

	QCheckBox* checkbox;
	QLabel* label;
	TextboxArrayWidget<1>* bump_strength = new TextboxArrayWidget<1>("Bump Strength", this);

	ShaderFactoryTextureLayerBumpmapBox(QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto settings_layout = new QVBoxLayout;
		{
			auto bump_settings_layout = new QHBoxLayout;
			{
				bump_settings_layout->addWidget(this->bump_strength);
			}
			settings_layout->addItem(bump_settings_layout);

		}
		this->setLayout(settings_layout);

		this->toggle(false);
	}
};
