#pragma once

#include "BaseWidgets/TextboxArrayWidget.hpp"
#include "BaseWidgets/ToggleableCombobox.hpp"
#include "BaseWidgets/TitleWidget.hpp"
#include "TextureMap.hpp"
#include "TextureSlot.hpp"

class DiffuseColorSettings : public QWidget
{
public:
	TitleWidget* title_widget;
	TextboxArrayWidget<4>* diffuse_color_widget;
	ToggleableCombobox* vertex_colors_widget;
	TextureMapWidget* transparency_map_widget;
	TextureMapWidget* diffuse_map_widget;
	TextureMapWidget* diffuse_map_widget_layer_2;
	ShaderFactoryTextureSlot* light_sampler;
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
			this->light_sampler = new ShaderFactoryTextureSlot("LightSampler (remember to do strength and power)", this);


			_layout->addWidget(this->title_widget, 0, Qt::AlignTop);
			_layout->addWidget(this->diffuse_color_widget, 0, Qt::AlignTop);
			_layout->addWidget(this->vertex_colors_widget, 0, Qt::AlignTop);
			_layout->addWidget(this->transparency_map_widget, 0, Qt::AlignTop);
			_layout->addWidget(this->diffuse_map_widget, 0, Qt::AlignTop);
			_layout->addWidget(this->diffuse_map_widget_layer_2, 0, Qt::AlignTop);
			_layout->addWidget(this->light_sampler, 0, Qt::AlignTop);
		}
		this->setLayout(_layout);
	}

	void toggleTransparencyMap(int active)
	{
		this->transparency_map_widget->checkbox->setEnabled(active);
	}
	void toggleDiffuseStrMap(int active)
	{
		this->diffuse_map_widget->checkbox->setEnabled(active);
	}
	void toggleOverlayDiffuseStrMap(int active)
	{
		this->diffuse_map_widget_layer_2->checkbox->setEnabled(active);
	}
};
