#pragma once

#include "TextureMap.hpp"
#include "TextureSlot.hpp"
#include "BaseWidgets/CheckableWidget.hpp"
#include "BaseWidgets/TextboxArrayWidget.hpp"
#include "BaseWidgets/TitleWidget.hpp"

class IlluminationSettings : public QWidget
{
	Q_OBJECT
public:
	TitleWidget* title_widget;
	CheckableWidget* receive_lamp;
	CheckableWidget* receive_ambient;
	CheckableWidget* receive_sky;
	CheckableWidget* obscure;
	TextureMapWidget* ambient_occlusion_map;
	CheckableWidget* receive_shadow_map;
	CheckableWidget* receive_fog;
	CheckableWidget* receive_fog_height;
	CheckableWidget* velvet;
	ShaderFactoryTextureSlotNoUV* clut;
	TextboxArrayWidget<3>* surface_color = new TextboxArrayWidget<3>("Surface Color", this);
	TextboxArrayWidget<3>* fuzzy_spec_color = new TextboxArrayWidget<3>("Fuzzy Spec Color", this);
	TextboxArrayWidget<3>* sub_color = new TextboxArrayWidget<3>("Sub Color", this);
	TextboxArrayWidget<1>* rolloff = new TextboxArrayWidget<1>("RollOff", this);
	TextboxArrayWidget<1>* velvet_strength = new TextboxArrayWidget<1>("Velvet Strength", this);

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

			this->receive_ambient = new CheckableWidget("Receive Ambient Light", this);
			{

				auto receive_ambient_contents = new QVBoxLayout;
				this->receive_sky = new CheckableWidget("Receive Sky", this);
				// I think SkyDir and GroundColor are Global variables
				//auto receive_sky_textboxes = new QVBoxLayout;
				//{
				//	auto sky_dir = new TextboxArrayWidget<3>("Sky Dir", this);
				//	auto ground_color = new TextboxArrayWidget<3>("Ground Color", this);

				//	receive_sky_textboxes->addWidget(sky_dir);
				//	receive_sky_textboxes->addWidget(ground_color);

				//	this->receive_sky->setContents(receive_sky_textboxes);
				//}

				receive_ambient_contents->addWidget(this->receive_sky);

				this->receive_ambient->setContents(receive_ambient_contents);
			}

			this->obscure = new CheckableWidget("Shadows", this);
			{

				auto receive_ambient_contents = new QVBoxLayout;
				this->ambient_occlusion_map = new TextureMapWidget("Ambient Occlusion Map", this);
				this->receive_shadow_map = new CheckableWidget("Receive Shadow Map", this);
				receive_ambient_contents->addWidget(this->ambient_occlusion_map);
				receive_ambient_contents->addWidget(this->receive_shadow_map);

				this->obscure->setContents(receive_ambient_contents);
			}

			this->receive_fog = new CheckableWidget("Receive Fog", this);
			{

				auto receive_fog_contents = new QVBoxLayout;

				this->receive_fog_height = new CheckableWidget("Use Height", this);

				receive_fog_contents->addWidget(this->receive_fog_height);

				this->receive_fog->setContents(receive_fog_contents);
			}

			_layout->addWidget(this->title_widget);
			_layout->addWidget(this->receive_lamp);
			_layout->addWidget(this->receive_ambient);
			_layout->addWidget(this->obscure);
			_layout->addWidget(this->receive_fog);
		}
		this->setLayout(_layout);

		connect(this->receive_lamp, &CheckableWidget::stateChanged, this, &IlluminationSettings::settingsUpdated);
		connect(this->receive_ambient, &CheckableWidget::stateChanged, this, &IlluminationSettings::settingsUpdated);
		connect(this->receive_sky, &CheckableWidget::stateChanged, this, &IlluminationSettings::settingsUpdated);
		connect(this->obscure, &CheckableWidget::stateChanged, this, &IlluminationSettings::settingsUpdated);
		connect(this->ambient_occlusion_map, &TextureMapWidget::settingsUpdated, this, &IlluminationSettings::settingsUpdated);

		connect(this->receive_shadow_map, &CheckableWidget::stateChanged, this, &IlluminationSettings::settingsUpdated);
		connect(this->receive_fog, &CheckableWidget::stateChanged, this, &IlluminationSettings::settingsUpdated);
		connect(this->receive_fog_height, &CheckableWidget::stateChanged, this, &IlluminationSettings::settingsUpdated);
		connect(this->velvet, &CheckableWidget::stateChanged, this, &IlluminationSettings::settingsUpdated);
		connect(this->clut, &ShaderFactoryTextureSlotNoUV::settingsUpdated, this, &IlluminationSettings::settingsUpdated);
	}
signals:
	void settingsUpdated(bool);
};
