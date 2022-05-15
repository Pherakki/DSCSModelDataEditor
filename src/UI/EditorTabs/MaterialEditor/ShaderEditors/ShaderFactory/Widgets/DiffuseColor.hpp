#pragma once

#include <QBoxLayout>

#include "BaseWidgets/TextboxArrayWidget.hpp"
#include "BaseWidgets/ToggleableCombobox.hpp"
#include "BaseWidgets/TitleWidget.hpp"
#include "TextureMap.hpp"
#include "TextureSlot.hpp"

class DiffuseColorSettings : public QWidget
{
	Q_OBJECT
public:
	TitleWidget* title_widget;
	TextboxArrayWidget<4>* diffuse_color_widget;
	ToggleableCombobox* vertex_colors_widget;
	TextureMapWidget* transparency_map_widget;
	TextureMapWidget* diffuse_map_widget;
	TextureMapWidget* diffuse_map_widget_layer_2;
	ShaderFactoryTextureSlot* light_sampler;
	TextboxArrayWidget<1>* light_strength_textbox = new TextboxArrayWidget<1>("Light Strength", this);
	TextboxArrayWidget<1>* light_power_textbox = new TextboxArrayWidget<1>("Light Power", this);
	QWidget* light_input_widget;
	DiffuseColorSettings(QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto _layout = new QVBoxLayout;
		{
			this->title_widget = new TitleWidget("Diffuse Color", this);
			this->diffuse_color_widget = new TextboxArrayWidget<4>("Base Color", this);
			this->vertex_colors_widget = new ToggleableCombobox("Vertex Colors", this);
			this->vertex_colors_widget->combobox->addItem("RGBA");
			this->vertex_colors_widget->combobox->addItem("RGB");
			this->transparency_map_widget = new TextureMapWidget("Transparency Map", this);
			this->diffuse_map_widget = new TextureMapWidget("Diffuse Strength Map", this);
			this->diffuse_map_widget_layer_2 = new TextureMapWidget("Overlay Diffuse Strength Map", this);
			this->light_sampler = new ShaderFactoryTextureSlot("LightSampler", this);

			this->light_input_widget = new QWidget;
			auto light_input_layout = new QHBoxLayout;
			light_input_layout->addWidget(this->light_strength_textbox);
			light_input_layout->addWidget(this->light_power_textbox);
			light_input_widget->setLayout(light_input_layout);

			_layout->addWidget(this->title_widget, 0, Qt::AlignTop);
			_layout->addWidget(this->diffuse_color_widget, 0, Qt::AlignTop);
			_layout->addWidget(this->vertex_colors_widget, 0, Qt::AlignTop);
			_layout->addWidget(this->transparency_map_widget, 0, Qt::AlignTop);
			_layout->addWidget(this->diffuse_map_widget, 0, Qt::AlignTop);
			_layout->addWidget(this->diffuse_map_widget_layer_2, 0, Qt::AlignTop);
			_layout->addWidget(this->light_sampler, 0, Qt::AlignTop);
			_layout->addWidget(light_input_widget, 0, Qt::AlignTop);

			this->light_input_widget->setEnabled(false);
			this->transparency_map_widget->setEnabled(false);
			this->diffuse_map_widget->setEnabled(false);
			this->diffuse_map_widget_layer_2->setEnabled(false);

			connect(this->light_sampler->checkbox, &QCheckBox::stateChanged, this, &DiffuseColorSettings::toggleLightInputs);

			connect(this->vertex_colors_widget, &ToggleableCombobox::settingsUpdated, this, &DiffuseColorSettings::settingsUpdated);
			connect(this->transparency_map_widget, &TextureMapWidget::settingsUpdated, this, &DiffuseColorSettings::settingsUpdated);
			connect(this->diffuse_map_widget, &TextureMapWidget::settingsUpdated, this, &DiffuseColorSettings::settingsUpdated);
			connect(this->diffuse_map_widget_layer_2, &TextureMapWidget::settingsUpdated, this, &DiffuseColorSettings::settingsUpdated);
			connect(this->light_sampler, &ShaderFactoryTextureSlot::settingsUpdated, this, &DiffuseColorSettings::settingsUpdated);

		}
		this->setLayout(_layout);
	}

private slots:
	void toggleLightInputs(bool toggle)
	{
		this->blockSignals(true);
		this->light_input_widget->setEnabled(toggle);
		this->blockSignals(false);
	}
signals:
	void settingsUpdated(bool);
};
