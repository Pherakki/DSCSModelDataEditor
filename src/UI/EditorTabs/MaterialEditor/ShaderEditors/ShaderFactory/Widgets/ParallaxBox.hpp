#pragma once

#include <QComboBox>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

#include "BaseWidgets/CheckableWidget.hpp"

class ShaderFactoryTextureLayerParallaxBox : public CheckableWidget
{
	Q_OBJECT;
public:
	QLabel* bias_label;
	QLineEdit* bias_x;
	QLineEdit* bias_y;
	QLabel* heightmap_label;
	QComboBox* heightmap_combobox;

	ShaderFactoryTextureLayerParallaxBox(QWidget* parent = Q_NULLPTR) : CheckableWidget("Parallax", parent)
	{

		auto settings_layout = new QVBoxLayout;
		{
			auto bias_settings_layout = new QHBoxLayout;
			{
				this->bias_label = new QLabel("Bias");
				this->bias_x = new QLineEdit;
				this->bias_x->setValidator(new QDoubleValidator(0, 100, 4, this));
				this->bias_y = new QLineEdit;
				this->bias_y->setValidator(new QDoubleValidator(0, 100, 4, this));
				bias_settings_layout->addWidget(this->bias_label);
				bias_settings_layout->addWidget(this->bias_x);
				bias_settings_layout->addWidget(this->bias_y);
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
