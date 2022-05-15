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
	TextboxArrayWidget<1>* reflection_strength = new TextboxArrayWidget<1>("Reflection Strength", this);
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

			this->reflection_strength->textboxes[0]->setText("0.0");

			this->fresnel = new CheckableWidget("Fresnel", this);
			auto fresnel_layout = new QVBoxLayout;
			{
				fresnel_layout->addWidget(this->fresnel_min);
				fresnel_layout->addWidget(this->fresnel_exp);
			}
			this->fresnel->setContents(fresnel_layout);

			contents_layout->addWidget(this->env_texture);
			contents_layout->addWidget(this->reflection_strength);
			contents_layout->addWidget(this->fresnel);
		}
		this->setContents(contents_layout);

		connect(this->checkbox, &QCheckBox::stateChanged, this, &ReflectionSettings::toggle);
	}

	EnvMapType getMapType()
	{
		return static_cast<EnvMapType>(this->env_texture->combobox->currentData().toInt());
	}

	bool isSphereMap()
	{
		return this->getMapType() == EnvMapType::SphereMap;
	}
private:
	void toggle(bool active)
	{
		this->blockSignals(true);
		this->env_texture->toggle(active);
		this->fresnel->toggle(active);
		this->reflection_strength->setEnabled(active);
		this->blockSignals(false);

	}
};
