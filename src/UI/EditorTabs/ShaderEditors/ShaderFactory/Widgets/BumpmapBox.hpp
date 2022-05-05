#pragma once

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QValidator>

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
	QLabel* bump_label;
	QLineEdit* bump_strength;

	ShaderFactoryTextureLayerBumpmapBox(QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto settings_layout = new QVBoxLayout;
		{
			auto bump_settings_layout = new QHBoxLayout;
			{
				this->bump_label = new QLabel("Bump Strength");
				this->bump_strength = new QLineEdit;
				this->bump_strength->setValidator(new QDoubleValidator(0, 100, 4, this));
				this->bump_strength->setText("0.0");
				bump_settings_layout->addWidget(this->bump_label);
				bump_settings_layout->addWidget(this->bump_strength);
			}
			settings_layout->addItem(bump_settings_layout);

		}
		this->setLayout(settings_layout);

		this->toggle(false);
	}
};
