#pragma once

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QValidator>

class ShaderFactoryTextureLayerBumpmapBox : public QWidget
{
private:
	void toggle(bool active)
	{
		this->bump_strength->setEnabled(active);
	}
public:
	QCheckBox* checkbox;
	QLabel* label;
	QLabel* bump_label;
	QLineEdit* bump_strength;

	ShaderFactoryTextureLayerBumpmapBox(QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto _layout = new QGridLayout;
		{
			this->checkbox = new QCheckBox;
			this->label = new QLabel("Bump");

			_layout->addWidget(this->checkbox, 0, 0);
			_layout->addWidget(this->label, 0, 1);

			auto settings_layout = new QVBoxLayout;
			{
				auto bump_settings_layout = new QHBoxLayout;
				{
					this->bump_label = new QLabel("Strength");
					this->bump_strength = new QLineEdit;
					this->bump_strength->setValidator(new QDoubleValidator(0, 100, 4, this));
					bump_settings_layout->addWidget(this->bump_label);
					bump_settings_layout->addWidget(this->bump_strength);
				}
				settings_layout->addItem(bump_settings_layout);

			}
			_layout->addLayout(settings_layout, 1, 1);
		}
		this->setLayout(_layout);

		this->toggle(false);
		connect(this->checkbox, &QCheckBox::stateChanged, this, &ShaderFactoryTextureLayerBumpmapBox::toggle);
	}
};
