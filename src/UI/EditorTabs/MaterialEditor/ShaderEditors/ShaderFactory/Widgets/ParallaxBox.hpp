#pragma once

#include <QComboBox>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>

#include "BaseWidgets/CheckableWidget.hpp"
#include "BaseWidgets/TextboxArrayWidget.hpp"

class ShaderFactoryTextureLayerParallaxBox : public CheckableWidget
{
	Q_OBJECT;
public:
	QLabel* bias_label;
	TextboxArrayWidget<2>* bias_xy = new TextboxArrayWidget<2>("Bias", this);
	QLabel* heightmap_label;
	QComboBox* heightmap_combobox;

	ShaderFactoryTextureLayerParallaxBox(QWidget* parent = Q_NULLPTR) : CheckableWidget("Parallax", parent)
	{

		auto settings_layout = new QVBoxLayout;
		{
			auto bias_settings_layout = new QHBoxLayout;
			{
				bias_settings_layout->addWidget(this->bias_xy);
			}
			settings_layout->addItem(bias_settings_layout);

			auto heightmap_settings_layout = new QHBoxLayout;
			{
				this->heightmap_label = new QLabel("Heightmap");
				this->heightmap_combobox = new QComboBox;
				this->heightmap_combobox->addItem("Diffuse Texture", "Diffuse Texture");
				this->heightmap_combobox->addItem("Normal Texture", "Normal Texture");
				heightmap_settings_layout->addWidget(this->heightmap_label);
				heightmap_settings_layout->addWidget(this->heightmap_combobox);
			}
			settings_layout->addItem(heightmap_settings_layout);
		}
		this->setContents(settings_layout);

		this->toggle(false);

		connect(this->checkbox, &QCheckBox::stateChanged, this, &ShaderFactoryTextureLayerParallaxBox::settingsUpdated);
		connect(this->heightmap_combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ShaderFactoryTextureLayerParallaxBox::settingsUpdated);
	}

signals:
	void settingsUpdated(bool);
};
