#pragma once

#include "BaseWidgets/TitleWidget.hpp"
#include "BumpmapBox.hpp"
#include "ParallaxBox.hpp"
#include "TextureSlot.hpp"

class ShaderFactoryTextureLayer1 : public QWidget
{
	Q_OBJECT;
private:
	void toggleParallax()
	{
		this->blockSignals(true);
		QString ctex = "Diffuse Texture";
		bool activate = false;
		auto*& hbox = this->parallax_settings->heightmap_combobox;
		if (this->diffuse_texture_settings->checkbox->isChecked())
		{
			if (int idx = hbox->findText(ctex); idx == -1)
				hbox->insertItem(0, ctex, ctex);
		}
		else
		{
			this->parallax_settings->heightmap_combobox->removeItem(0);
		}

		QString ntex = "Normal Texture";
		if (this->normal_texture_settings->checkbox->isChecked())
		{
			activate = true;
			if (int idx = hbox->findText(ntex); idx == -1)
				hbox->insertItem(1, ntex, ntex);
		}
		else
		{
			this->parallax_settings->heightmap_combobox->removeItem(hbox->findText(ntex));
		}
		this->parallax_settings->setEnabled(activate);

		this->blockSignals(false);
	}
	void toggleBump()
	{
		this->bumpmap_settings->toggle(this->normal_texture_settings->checkbox->isChecked());
	}


public:
	TitleWidget* title_widget;
	ShaderFactoryTextureSlot* diffuse_texture_settings;
	ShaderFactoryTextureSlot* normal_texture_settings;
	ShaderFactoryTextureLayerParallaxBox* parallax_settings;
	ShaderFactoryTextureLayerBumpmapBox* bumpmap_settings;

	ShaderFactoryTextureLayer1(QString title_text, QWidget* parent = nullptr) : QWidget(parent)
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
				this->parallax_settings = new ShaderFactoryTextureLayerParallaxBox;
				normal_mapping_layout->addWidget(this->parallax_settings);
			}
			_layout->addItem(normal_mapping_layout);

		}
		this->setLayout(_layout);
		this->toggleParallax();
		this->toggleBump();

		connect(this->diffuse_texture_settings->checkbox, &QCheckBox::stateChanged, this, &ShaderFactoryTextureLayer1::toggleParallax);
		connect(this->normal_texture_settings->checkbox, &QCheckBox::stateChanged, this, &ShaderFactoryTextureLayer1::toggleParallax);
		connect(this->normal_texture_settings->checkbox, &QCheckBox::stateChanged, this, &ShaderFactoryTextureLayer1::toggleBump);

		connect(this->diffuse_texture_settings, &ShaderFactoryTextureSlot::settingsUpdated, this, &ShaderFactoryTextureLayer1::settingsUpdated);
		connect(this->normal_texture_settings, &ShaderFactoryTextureSlot::settingsUpdated, this, &ShaderFactoryTextureLayer1::settingsUpdated);
		connect(this->parallax_settings, &ShaderFactoryTextureLayerParallaxBox::settingsUpdated, this, &ShaderFactoryTextureLayer1::settingsUpdated);
	}

signals:
	void settingsUpdated(bool);
};
