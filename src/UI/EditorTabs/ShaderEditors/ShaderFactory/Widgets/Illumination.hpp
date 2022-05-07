#pragma once

#include "TextureSlot.hpp"
#include "BaseWidgets/CheckableWidget.hpp"
#include "BaseWidgets/TextboxArrayWidget.hpp"
#include "BaseWidgets/TitleWidget.hpp"

class IlluminationSettings : public QWidget
{
public:
	TitleWidget* title_widget;
	CheckableWidget* receive_lamp;
	CheckableWidget* velvet;
	ShaderFactoryTextureSlotNoUV* clut;

	IlluminationSettings(QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto _layout = new QVBoxLayout;
		{
			this->title_widget = new TitleWidget("Illumination", this);

			this->receive_lamp = new CheckableWidget("Receive Lamp", this);
			{
				auto receive_lamp_contents = new QVBoxLayout;
				this->velvet = new CheckableWidget("Velvet", this);
				auto velvet_textboxes = new QVBoxLayout;
				{
					auto surface_color = new TextboxArrayWidget<3>("Surface Color", this);
					auto fuzzy_spec_color = new TextboxArrayWidget<3>("Fuzzy Spec Color", this);
					auto sub_color = new TextboxArrayWidget<3>("Sub Color", this);
					auto rolloff = new TextboxArrayWidget<1>("RollOff", this);
					auto velvet_strength = new TextboxArrayWidget<1>("Velvet Strength", this);

					velvet_textboxes->addWidget(surface_color);
					velvet_textboxes->addWidget(fuzzy_spec_color);
					velvet_textboxes->addWidget(sub_color);
					velvet_textboxes->addWidget(rolloff);
					velvet_textboxes->addWidget(velvet_strength);

					this->velvet->setContents(velvet_textboxes);
				}
				
				this->clut = new ShaderFactoryTextureSlotNoUV("CLUT", this);

				receive_lamp_contents->addWidget(this->velvet);
				receive_lamp_contents->addWidget(this->clut);

				this->receive_lamp->setContents(receive_lamp_contents);
			}


			_layout->addWidget(this->title_widget);
			_layout->addWidget(this->receive_lamp);
		}
		this->setLayout(_layout);
	}
};
