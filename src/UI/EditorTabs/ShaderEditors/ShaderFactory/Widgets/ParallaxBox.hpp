#pragma once

#include <QComboBox>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

class ShaderFactoryTextureLayerParallaxBox : public QWidget
{
	Q_OBJECT;
private:
	void toggle(bool active)
	{
		this->bias_x->setEnabled(active);
		this->bias_y->setEnabled(active);
		this->heightmap_combobox->setEnabled(active);
	}
public:
	QCheckBox* checkbox;
	QLabel* label;
	QLabel* bias_label;
	QLineEdit* bias_x;
	QLineEdit* bias_y;
	QLabel* heightmap_label;
	QComboBox* heightmap_combobox;

	ShaderFactoryTextureLayerParallaxBox(QWidget* parent = nullptr) : QWidget(parent)
	{
		auto _layout = new QGridLayout;
		{
			this->checkbox = new QCheckBox;
			this->label = new QLabel("Parallax");

			_layout->addWidget(this->checkbox, 0, 0);
			_layout->addWidget(this->label, 0, 1);

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
			_layout->addLayout(settings_layout, 1, 1);
		}
		this->setLayout(_layout);

		this->toggle(false);
		connect(this->checkbox, &QCheckBox::stateChanged, this, &ShaderFactoryTextureLayerParallaxBox::toggle);

		connect(this->checkbox, &QCheckBox::stateChanged, this, &ShaderFactoryTextureLayerParallaxBox::settingsUpdated);
		connect(this->heightmap_combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ShaderFactoryTextureLayerParallaxBox::settingsUpdated);
	}

signals:
	void settingsUpdated(bool);
};
