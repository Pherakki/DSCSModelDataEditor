#pragma once

#include "BaseWidgets/CheckableWidget.hpp"
#include "BaseWidgets/TextboxArrayWidget.hpp"
#include "TextureSlot.hpp"


class ReflectionSettings : public CheckableWidget
{
public:
	static enum class EnvMapType
	{
		CubeMap,
		SphereMap
	};

	ShaderFactoryComboboxTextureSlot* env_texture;
	CheckableWidget* fresnel;
	TextboxArrayWidget<1>* fresnel_min;
	TextboxArrayWidget<1>* fresnel_exp;

	ReflectionSettings(QWidget* parent = Q_NULLPTR) 
		: CheckableWidget("Reflection", parent)
		, fresnel_min{ new TextboxArrayWidget<1>("Min", this) }
		, fresnel_exp{ new TextboxArrayWidget<1>("Exp", this) }
	{
		auto contents_layout = new QVBoxLayout;
		{
			this->env_texture = new ShaderFactoryComboboxTextureSlot(this);
			this->env_texture->combobox->addItem("Cube Map", static_cast<int>(EnvMapType::CubeMap));
			this->env_texture->combobox->addItem("Sphere Map", static_cast<int>(EnvMapType::SphereMap));

			this->fresnel = new CheckableWidget("Fresnel", this);
			auto fresnel_layout = new QVBoxLayout;
			{
				fresnel_layout->addWidget(this->fresnel_min);
				fresnel_layout->addWidget(this->fresnel_exp);
			}
			this->fresnel->setContents(fresnel_layout);

			contents_layout->addWidget(this->env_texture);
			contents_layout->addWidget(this->fresnel);
		}
		this->setContents(contents_layout);

		connect(this->checkbox, &QCheckBox::stateChanged, this->env_texture, &ShaderFactoryComboboxTextureSlot::setEnabled);
		connect(this->checkbox, &QCheckBox::stateChanged, this->env_texture, &ShaderFactoryComboboxTextureSlot::toggle);
	}

	EnvMapType getMapType()
	{
		return static_cast<EnvMapType>(this->env_texture->combobox->currentData().toInt());
	}

	bool isSphereMap()
	{
		return this->getMapType() == EnvMapType::SphereMap;
	}
};
