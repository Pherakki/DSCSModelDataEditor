#pragma once

#include "TextureSlot.hpp"

class FresnelReflectionWidget : public QWidget
{

};

class ReflectionSettings : public QWidget
{
public:
	ShaderFactoryTextureSlot* env_texture;
	//TitleWidget* title_widget;
	ReflectionSettings(QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto _layout = new QGridLayout;
		{
			//this->title_widget = new TitleWidget("Reflection", this);

			this->env_texture = new ShaderFactoryTextureSlot("Reflection Texture", this);

			_layout->addWidget(this->env_texture, 0, 1);
			_layout->addWidget(this->env_texture->checkbox, 0, 0);
		}
		this->setLayout(_layout);
	}
};
