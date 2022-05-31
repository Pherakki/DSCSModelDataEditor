#pragma once

#include "BaseWidgets/TitleWidget.hpp"
#include "BumpmapBox.hpp"
#include "TextureSlot.hpp"

class ShaderFactoryTextureLayer2 : public QWidget
{
	Q_OBJECT;
private:
	void toggleBump()
	{
		this->bumpmap_settings->toggle(this->normal_texture_settings->checkbox->isChecked());
	}


public:
	TitleWidget* title_widget;
	ShaderFactoryTextureSlot* diffuse_texture_settings;
	ShaderFactoryTextureSlot* normal_texture_settings;
	ShaderFactoryTextureLayerBumpmapBox* bumpmap_settings;
	ToggleableTextboxesWidget<1>* overlay_strength;

	ShaderFactoryTextureLayer2(QString title_text, QWidget* parent = nullptr) : QWidget(parent)
	{
		auto _layout = new QVBoxLayout;
		{
			this->title_widget = new TitleWidget(title_text);
			this->diffuse_texture_settings = new ShaderFactoryTextureSlot("Diffuse Texture");
			this->normal_texture_settings = new ShaderFactoryTextureSlot("Normal Texture ");

			_layout->addWidget(this->title_widget);
			_layout->addWidget(this->diffuse_texture_settings);
			_layout->addWidget(this->normal_texture_settings);

			auto normal_mapping_layout = new QHBoxLayout;
			{
				this->bumpmap_settings = new ShaderFactoryTextureLayerBumpmapBox;
				normal_mapping_layout->addWidget(this->bumpmap_settings);
			}
			_layout->addItem(normal_mapping_layout);
			this->overlay_strength = new ToggleableTextboxesWidget<1>("Overlay Strength", this);
			_layout->addWidget(this->overlay_strength);

		}
		this->setLayout(_layout);
		this->toggleBump();

		connect(this->normal_texture_settings->checkbox, &QCheckBox::stateChanged, this, &ShaderFactoryTextureLayer2::toggleBump);

		connect(this->diffuse_texture_settings, &ShaderFactoryTextureSlot::settingsUpdated, this, &ShaderFactoryTextureLayer2::settingsUpdated);
		connect(this->normal_texture_settings, &ShaderFactoryTextureSlot::settingsUpdated, this, &ShaderFactoryTextureLayer2::settingsUpdated);
		connect(this->overlay_strength, &ToggleableTextboxesWidget<1>::settingsUpdated, this, &ShaderFactoryTextureLayer2::settingsUpdated);
	}

signals:
	void settingsUpdated(bool);
};
