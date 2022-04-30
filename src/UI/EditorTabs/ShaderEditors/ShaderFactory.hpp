#pragma once

#include <array>

#include <QCheckBox>
#include <QComboBox>
#include <QBoxLayout>
#include <QDoubleValidator>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QLineEdit>
#include <QWidget>

#include "ShaderGenerator/VertexShader.hpp"
#include "ShaderGenerator/FragmentShader.hpp"
#include "../../Spoiler/Spoiler.hpp"
#include "../../../Rendering/DSCS/DataObjects/OpenGLDSCSMesh.hpp"
#include "../../../Rendering/DSCS/DataObjects/OpenGLDSCSMaterial.hpp"
#include "../../../Rendering/DSCS/ShaderSystem/cgGL/cgGLShaderBackend.hpp"
#include "../../../Utils/Hashing.hpp"


template <uint8_t n_boxes>
class ToggleableTextboxesWidget : public QWidget
{
private:
	void toggleTextboxes(bool state)
	{
		for (auto*& box : this->textboxes)
			box->setEnabled(state);
	}
public:
	QCheckBox* checkbox;
	QLabel* label;
	std::array<QLineEdit*, n_boxes> textboxes;

	explicit ToggleableTextboxesWidget(QString label_text, QWidget* parent = nullptr) : QWidget(parent)
	{
		auto layout = new QHBoxLayout;

		this->checkbox = new QCheckBox;
		layout->addWidget(this->checkbox);
		layout->setAlignment(this->checkbox, Qt::AlignLeft);
		this->label = new QLabel(label_text);
		layout->addWidget(this->label);
		layout->setAlignment(this->label, Qt::AlignLeft);

		layout->addStretch();

		for (auto& box : this->textboxes)
		{
			box = new QLineEdit;
			box->setValidator(new QDoubleValidator(0, 100, 4, this));
			box->setMaximumWidth(40);
			layout->addWidget(box);
			layout->setAlignment(box, Qt::AlignRight);
		}

		this->setLayout(layout);
		this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		this->toggleTextboxes(false);
		connect(this->checkbox, &QCheckBox::stateChanged, this, &ToggleableTextboxesWidget::toggleTextboxes);
	}
};

template <uint8_t n_boxes>
class TextboxArrayWidget : public QWidget
{
public:
	QLabel* label;
	std::array<QLineEdit*, n_boxes> textboxes;

	explicit TextboxArrayWidget(QString label_text, QWidget* parent = nullptr) : QWidget(parent)
	{
		auto layout = new QHBoxLayout;

		this->label = new QLabel(label_text);
		layout->addWidget(this->label);
		layout->setAlignment(this->label, Qt::AlignLeft);

		layout->addStretch();

		for (auto& box : this->textboxes)
		{
			box = new QLineEdit;
			box->setValidator(new QDoubleValidator(0, 100, 4, this));
			box->setMaximumWidth(40);
			layout->addWidget(box);
			layout->setAlignment(box, Qt::AlignRight);
		}

		this->setLayout(layout);
		this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	}
};

class TitleWidget : public QWidget
{
public:
	QLabel* label;

	explicit TitleWidget(QString label_text, QWidget* parent = nullptr) : QWidget(parent)
	{
		auto layout = new QHBoxLayout;

		layout->addStretch();
		this->label = new QLabel(label_text);
		layout->setAlignment(this->label, Qt::AlignCenter);
		layout->addWidget(this->label);
		layout->addStretch();

		this->setLayout(layout);
		this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	}
};

class ShaderFactoryUVSettingsWidget : public QWidget
{
	Q_OBJECT;

public:
	TitleWidget* title;
	ToggleableTextboxesWidget<0>* widget_projection;
	ToggleableTextboxesWidget<2>* widget_scrollspeed;
	ToggleableTextboxesWidget<1>* widget_rotation;
	ToggleableTextboxesWidget<2>* widget_offset;
	ToggleableTextboxesWidget<2>* widget_scale;

	explicit ShaderFactoryUVSettingsWidget(QString label_text, QWidget* parent = nullptr) : QWidget(parent)
	{
		auto layout = new QVBoxLayout;

		this->title = new TitleWidget(label_text);
		layout->setAlignment(this->title, Qt::AlignHCenter);
		layout->addWidget(this->title);

		this->widget_projection = new ToggleableTextboxesWidget<0>("Projection");
		layout->addWidget(this->widget_projection);

		this->widget_scrollspeed = new ToggleableTextboxesWidget<2>("Scroll Speed");
		this->widget_rotation    = new ToggleableTextboxesWidget<1>("Rotation");
		this->widget_offset      = new ToggleableTextboxesWidget<2>("Offset");
		this->widget_scale       = new ToggleableTextboxesWidget<2>("Scale");
		layout->addWidget(this->widget_scrollspeed);
		layout->addWidget(this->widget_rotation);
		layout->addWidget(this->widget_offset);
		layout->addWidget(this->widget_scale);

		this->setLayout(layout);
		this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	}
};


class FileComboBox : public QWidget
{
public:
	QComboBox*   combobox;
	QPushButton* open_file_button;

	FileComboBox(QWidget* parent = nullptr) : QWidget(parent)
	{
		auto f_layout = new QHBoxLayout;

		this->combobox = new QComboBox;
		this->open_file_button = new QPushButton;

		f_layout->addWidget(this->combobox);
		f_layout->addWidget(this->open_file_button);

		this->setLayout(f_layout);
		this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	}
};

class ShaderFactoryTextureSlot : public QWidget
{
private:
	void toggle(bool active)
	{
		this->file_combo_box->setEnabled(active);
		this->uv_slot_combobox->setEnabled(active);
	}
public:
	QCheckBox* checkbox;
	QLabel* texture_label;
	FileComboBox* file_combo_box;
	QLabel* uv_slot_label;
	QComboBox* uv_slot_combobox;

	ShaderFactoryTextureSlot(QString label_text, QWidget* parent = nullptr) : QWidget(parent)
	{
		auto _layout = new QHBoxLayout;

		this->checkbox = new QCheckBox;
		this->texture_label = new QLabel(label_text);
		this->file_combo_box = new FileComboBox;
		this->uv_slot_label = new QLabel("UV Slot:");
		this->uv_slot_combobox = new QComboBox;
		this->uv_slot_combobox->addItems({ "1", "2", "3" });

		_layout->addWidget(this->checkbox);
		_layout->addWidget(this->texture_label);
		_layout->addWidget(this->file_combo_box);
		_layout->addWidget(this->uv_slot_label);
		_layout->addWidget(this->uv_slot_combobox);

		this->setLayout(_layout);
		this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

		this->toggle(false);
		connect(this->checkbox, &QCheckBox::stateChanged, this, &ShaderFactoryTextureSlot::toggle);
	}
};

class ShaderFactoryTextureLayerParallaxBox : public QWidget
{
private:
	void toggle(bool active)
	{
		this->bias_x->setEnabled(active);
		this->bias_y->setEnabled(active);
		this->heightmap_combobox->setEnabled(active);
	}
public:
	QCheckBox* checkbox;
	QLabel*    label;
	QLabel*    bias_label;
	QLineEdit* bias_x;
	QLineEdit* bias_y;
	QLabel*    heightmap_label;
	QComboBox* heightmap_combobox;

	ShaderFactoryTextureLayerParallaxBox(QWidget* parent = nullptr) : QWidget(parent)
	{
		auto _layout = new QGridLayout;
		{
			this->checkbox = new QCheckBox;
			this->label = new QLabel("Parallax");

			_layout->addWidget(this->checkbox, 0, 0);
			_layout->addWidget(this->label, 0, 1);

			auto settings_layout = new QVBoxLayout;
			{
				auto bias_settings_layout = new QHBoxLayout;
				{
					this->bias_label = new QLabel("Bias");
					this->bias_x = new QLineEdit;
					this->bias_x->setValidator(new QDoubleValidator(0, 100, 4, this));
					this->bias_y = new QLineEdit;
					this->bias_y->setValidator(new QDoubleValidator(0, 100, 4, this));
					bias_settings_layout->addWidget(this->bias_label);
					bias_settings_layout->addWidget(this->bias_x);
					bias_settings_layout->addWidget(this->bias_y);
				}
				settings_layout->addItem(bias_settings_layout);

				auto heightmap_settings_layout = new QHBoxLayout;
				{
					this->heightmap_label = new QLabel("Heightmap");
					this->heightmap_combobox = new QComboBox;
					this->heightmap_combobox->addItem("Diffuse Texture", "Diffuse Texture");
					this->heightmap_combobox->addItem("Normal Texture", "Normal Texture");
					heightmap_settings_layout->addWidget(this->heightmap_label);
					heightmap_settings_layout->addWidget(this->heightmap_combobox);
				}
				settings_layout->addItem(heightmap_settings_layout);
			}
			_layout->addLayout(settings_layout, 1, 1);
		}
		this->setLayout(_layout);

		this->toggle(false);
		connect(this->checkbox, &QCheckBox::stateChanged, this, &ShaderFactoryTextureLayerParallaxBox::toggle);
	}
};

class ShaderFactoryTextureLayerBumpmapBox : public QWidget
{
private:
	void toggle(bool active)
	{
		this->bump_strength->setEnabled(active);
	}
public:
	QCheckBox* checkbox;
	QLabel* label;
	QLabel* bump_label;
	QLineEdit* bump_strength;

	ShaderFactoryTextureLayerBumpmapBox(QWidget* parent = nullptr) : QWidget(parent)
	{
		auto _layout = new QGridLayout;
		{
			this->checkbox = new QCheckBox;
			this->label = new QLabel("Bump");
			
			_layout->addWidget(this->checkbox, 0, 0);
			_layout->addWidget(this->label, 0, 1);

			auto settings_layout = new QVBoxLayout;
			{
				auto bump_settings_layout = new QHBoxLayout;
				{
					this->bump_label = new QLabel("Strength");
					this->bump_strength = new QLineEdit;
					this->bump_strength->setValidator(new QDoubleValidator(0, 100, 4, this));
					bump_settings_layout->addWidget(this->bump_label);
					bump_settings_layout->addWidget(this->bump_strength);
				}
				settings_layout->addItem(bump_settings_layout);

			}
			_layout->addLayout(settings_layout, 1, 1);
		}
		this->setLayout(_layout);

		this->toggle(false);
		connect(this->checkbox, &QCheckBox::stateChanged, this, &ShaderFactoryTextureLayerBumpmapBox::toggle);
	}
};

class ShaderFactoryTextureLayer1 : public QWidget
{
private:
	void toggleParallax()
	{
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
	}
	void toggleBump()
	{
		this->bumpmap_settings->setEnabled(this->normal_texture_settings->checkbox->isChecked());
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
				this->parallax_settings = new ShaderFactoryTextureLayerParallaxBox;
				normal_mapping_layout->addWidget(this->parallax_settings);
				this->bumpmap_settings = new ShaderFactoryTextureLayerBumpmapBox;
				normal_mapping_layout->addWidget(this->bumpmap_settings);
			}
			_layout->addItem(normal_mapping_layout);

		}
		this->setLayout(_layout);
		this->toggleParallax();
		this->toggleBump();

		connect(this->diffuse_texture_settings->checkbox, &QCheckBox::stateChanged, this, &ShaderFactoryTextureLayer1::toggleParallax);
		connect(this->normal_texture_settings->checkbox, &QCheckBox::stateChanged, this, &ShaderFactoryTextureLayer1::toggleParallax);
		connect(this->normal_texture_settings->checkbox, &QCheckBox::stateChanged, this, &ShaderFactoryTextureLayer1::toggleBump);
	}
};

class TextureMapWidget : public QWidget
{
private:
	void toggle(int active)
	{
		this->maptype->setEnabled(active);
		this->channel->setEnabled(active);
	}
public:
	QCheckBox* checkbox;
	QLabel* label;
	QComboBox* maptype;
	QComboBox* channel;

	TextureMapWidget(const std::string& label_text, QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		QHBoxLayout* _layout = new QHBoxLayout;
		{
			QHBoxLayout* title_layout = new QHBoxLayout;
			{
				this->checkbox = new QCheckBox(this);
				this->label = new QLabel(QString::fromStdString(label_text), this);

				title_layout->addWidget(this->checkbox, 0, Qt::AlignLeft);
				auto spacer = new QSpacerItem(10, 1, QSizePolicy::Fixed, QSizePolicy::Fixed);
				title_layout->addItem(spacer);
				title_layout->addWidget(this->label, 0, Qt::AlignLeft);
			}

			QHBoxLayout* contents_layout = new QHBoxLayout;
			QWidget* contents_widget = new QWidget(this);
			{
				this->maptype = new QComboBox(contents_widget);
				this->maptype->addItem("Diffuse");
				this->maptype->addItem("Normal");
				this->maptype->addItem("Light");

				this->channel = new QComboBox(contents_widget);
				this->channel->addItem("R");
				this->channel->addItem("G");
				this->channel->addItem("B");
				this->channel->addItem("A");

				contents_layout->addWidget(this->maptype, 0, Qt::AlignRight);
				contents_layout->addWidget(this->channel, 0, Qt::AlignRight);
				contents_widget->setLayout(contents_layout);
			}
			_layout->addItem(title_layout);
			_layout->addWidget(contents_widget, 1, Qt::AlignRight);
		}
		this->setLayout(_layout);

		this->toggle(false);
		connect(this->checkbox, &QCheckBox::stateChanged, this, &TextureMapWidget::toggle);
	}
};

class ToggleableCombobox : public QWidget
{
private:
	void toggle(int active)
	{
		this->combobox->setEnabled(active);
	}
public:
	QCheckBox* checkbox;
	QLabel* label;
	QComboBox* combobox;

	ToggleableCombobox(const std::string& label_text, QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		QHBoxLayout* _layout = new QHBoxLayout;
		{
			QHBoxLayout* title_layout = new QHBoxLayout;
			{
				this->checkbox = new QCheckBox(this);
				this->label = new QLabel(QString::fromStdString(label_text), this);
				title_layout->addWidget(this->checkbox, 0, Qt::AlignLeft);
				auto verticalSpacer = new QSpacerItem(10, 1, QSizePolicy::Fixed, QSizePolicy::Fixed);
				title_layout->addItem(verticalSpacer);
				title_layout->addWidget(this->label, 0, Qt::AlignLeft);
			}
			this->combobox = new QComboBox(this);

			_layout->addItem(title_layout);
			_layout->addWidget(this->combobox, 1, Qt::AlignRight);
		}
		this->setLayout(_layout);

		this->toggle(false);
		connect(this->checkbox, &QCheckBox::stateChanged, this, &ToggleableCombobox::toggle);
	}
};

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
	DiffuseColorSettings(QWidget* parent=Q_NULLPTR) : QWidget(parent)
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
};

class SpecularColorSettings : public QWidget
{
public:
	TitleWidget* title_widget;
	SpecularColorSettings(QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto _layout = new QVBoxLayout;
		{
			this->title_widget = new TitleWidget("Specular Color", this);

			_layout->addWidget(this->title_widget);
		}
		this->setLayout(_layout);
	}
};

class ReflectionSettings : public QWidget
{
public:
	TitleWidget* title_widget;
	ReflectionSettings(QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto _layout = new QVBoxLayout;
		{
			this->title_widget = new TitleWidget("Reflection", this);

			_layout->addWidget(this->title_widget);
		}
		this->setLayout(_layout);
	}
};

class IlluminationSettings : public QWidget
{
public:
	TitleWidget* title_widget;
	IlluminationSettings(QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto _layout = new QVBoxLayout;
		{
			this->title_widget = new TitleWidget("Illumination", this);

			_layout->addWidget(this->title_widget);
		}
		this->setLayout(_layout);
	}
};

class GlassMapSettings : public QWidget
{
public:
	TitleWidget* title_widget;
	GlassMapSettings(QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto _layout = new QVBoxLayout;
		{
			this->title_widget = new TitleWidget("Glass Map", this);

			_layout->addWidget(this->title_widget);
		}
		this->setLayout(_layout);
	}
};


class PositionSettings : public QWidget
{
public:
	ToggleableTextboxesWidget<1>* fat;
	ToggleableTextboxesWidget<1>* zbias;
	QCheckBox* billboard_checkbox;
	QCheckBox* billboard_align_y_checkbox;
	PositionSettings(QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto _layout = new QVBoxLayout;
		{
			this->fat = new ToggleableTextboxesWidget<1>("Fat", this);
			this->zbias = new ToggleableTextboxesWidget<1>("Z Bias", this);

			QGridLayout* billboard_layout = new QGridLayout;
			{
				this->billboard_checkbox = new QCheckBox(this);
				billboard_layout->addWidget(this->billboard_checkbox, 0, 0, Qt::AlignLeft);
				billboard_layout->addItem(new QSpacerItem(10, 1, QSizePolicy::Fixed, QSizePolicy::Fixed), 0, 1);
				billboard_layout->addWidget(new QLabel("Billboard"), 0, 2, Qt::AlignLeft);
				billboard_layout->setColumnStretch(0, 0);
				billboard_layout->setColumnStretch(1, 0);
				billboard_layout->setColumnStretch(2, 0);
				billboard_layout->setColumnStretch(3, 1);
				billboard_layout->addItem(new QSpacerItem(10, 1, QSizePolicy::Minimum, QSizePolicy::Expanding), 0, 3);
				billboard_layout->addItem(new QSpacerItem(1, 10, QSizePolicy::Fixed, QSizePolicy::Fixed), 1, 0, 1, 4);
				QHBoxLayout* align_y_layout = new QHBoxLayout;
				{
					this->billboard_align_y_checkbox = new QCheckBox(this);
					align_y_layout->addWidget(this->billboard_align_y_checkbox, 0, Qt::AlignLeft);
					align_y_layout->addItem(new QSpacerItem(10, 1, QSizePolicy::Fixed, QSizePolicy::Fixed));
					align_y_layout->addWidget(new QLabel("Align Y", this), 0, Qt::AlignLeft);
					auto spacer = new QSpacerItem(10, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
					align_y_layout->addItem(spacer);
				}
				billboard_layout->addItem(align_y_layout, 2, 2);
			}
			_layout->addWidget(this->fat);
			_layout->addWidget(this->zbias);
			_layout->addItem(billboard_layout);
		}
		this->setLayout(_layout);
	}
};

struct TextureRefs
{
	typedef std::shared_ptr<Rendering::DataObjects::OpenGLDSCSTexture> Texture;

	std::string c1_texture_name;
	Texture c1_texture = nullptr;

	std::string n1_texture_name;
	Texture n1_texture = nullptr;

	std::string c2_texture_name;
	Texture c2_texture = nullptr;

	std::string n2_texture_name;
	Texture n2_texture = nullptr;

	std::string light_texture_name;
	Texture light_texture = nullptr;

	std::string env_texture_name;
	Texture env_texture = nullptr;
};

class ShaderFactory : public QWidget
{
	Q_OBJECT;

private:
	typedef Rendering::DSCS::DataObjects::OpenGLDSCSMesh Mesh;
	typedef std::shared_ptr<Mesh> MeshPtr;
	typedef std::shared_ptr<Rendering::DataObjects::OpenGLDSCSTexture> TexturePtr;
	typedef std::unordered_map<std::string, TexturePtr> TextureLibrary_t;
	typedef Rendering::DSCS::DataObjects::OpenGLDSCSMaterial Material;
	typedef std::shared_ptr<Material> MaterialPtr;
	typedef std::unique_ptr<Rendering::ShaderBackends::cgGLShaderBackend> ShaderBackend_t;
	typedef Rendering::DSCS::AnimationBuffer AnimBuf_t;

	ShaderFactoryTextureLayer1* texture_layer_1;
	ShaderFactoryTextureLayer1* texture_layer_2;
	ShaderFactoryUVSettingsWidget* uv_settings_1;
	ShaderFactoryUVSettingsWidget* uv_settings_2;
	ShaderFactoryUVSettingsWidget* uv_settings_3;
	DiffuseColorSettings* diffuse_color_settings;
	SpecularColorSettings* specular_color_settings;
	ReflectionSettings* reflection_settings;
	IlluminationSettings* illumination_settings;
	GlassMapSettings* glassmap_settings;
	PositionSettings* position_settings;

	AnimBuf_t& animation_buffer;
	MeshPtr selected_mesh = nullptr;
	MaterialPtr selected_material = nullptr;
	MaterialPtr active_local_material = nullptr;
	TextureLibrary_t& texture_library;
	ShaderBackend_t& shader_backend;
	std::unordered_map<std::string, MaterialPtr> local_material_cache;

	void updateTexturesOn(QComboBox* combobox)
	{
		combobox->clear();
		for (const auto& [texture_name, texture] : this->texture_library)
		{
			combobox->addItem(QString::fromStdString(texture_name), QString::fromStdString(texture_name));
		}
	}

	void updateAvailableTextures()
	{
		this->updateTexturesOn(this->texture_layer_1->diffuse_texture_settings->file_combo_box->combobox);
		this->updateTexturesOn(this->texture_layer_1->normal_texture_settings->file_combo_box->combobox);
		this->updateTexturesOn(this->texture_layer_2->diffuse_texture_settings->file_combo_box->combobox);
		this->updateTexturesOn(this->texture_layer_2->normal_texture_settings->file_combo_box->combobox);
	}

	void createTexSettings(FactorySettings& settings, Sampler& sampler, std::string& texname, TexturePtr& texture, const ShaderFactoryTextureSlot& tex_ui)
	{
		sampler.enabled = true;
		sampler.uv_slot = tex_ui.uv_slot_combobox->currentIndex();
		settings.uv_slots[sampler.uv_slot].enabled = true;
		sampler.combined_channels = true; // Change later
		texname = tex_ui.file_combo_box->combobox->currentText().toStdString();
		texture = this->texture_library.at(texname);
	}

	void createUVSettings(TexSlot& uv_slot, const ShaderFactoryUVSettingsWidget& uv_ui)
	{
		uv_slot.is_projection = uv_ui.widget_projection->checkbox->isChecked();
		uv_slot.scroll = uv_ui.widget_scrollspeed->checkbox->isChecked();
		uv_slot.offset = uv_ui.widget_offset->checkbox->isChecked();
		uv_slot.rotation = uv_ui.widget_rotation->checkbox->isChecked();
		uv_slot.scale = uv_ui.widget_scale->checkbox->isChecked();
	}

	void createParallaxSettings(FactorySettings& settings)
	{
		if (this->texture_layer_1->parallax_settings->checkbox->isChecked() && this->texture_layer_1->normal_texture_settings->checkbox->isChecked())
		{
			settings.parallax = true;
			settings.view_matrix = true;
			settings.use_tangents = true;
			auto heightmap = this->texture_layer_1->parallax_settings->heightmap_combobox->currentText();
			if (heightmap == "Diffuse Texture")
				settings.parallax_heightmap = MapType::DIFFUSE;
			else if (heightmap == "Normal Texture")
				settings.parallax_heightmap = MapType::NORMAL;
			else
				throw std::exception("Error: parallax maptype not diffuse or normal.");
			settings.parallax_heightmap_channel = RGBAChannel::A; // Make this configurable
		}
	}

	void createBumpSettings(FactorySettings& settings)
	{
		if (this->texture_layer_1->bumpmap_settings->checkbox->isChecked() && this->texture_layer_1->normal_texture_settings->checkbox->isChecked())
		{
			settings.use_tangents = true;
		}
	}

	void createSettingsFromUI(FactorySettings& settings, TextureRefs& textures)
	{
		// Load textures
		if (const auto& tex_ui = this->texture_layer_1->diffuse_texture_settings; tex_ui->checkbox->isChecked())
			this->createTexSettings(settings, settings.texlayer_1.colorsampler, textures.c1_texture_name, textures.c1_texture, *tex_ui);
		if (const auto& tex_ui = this->texture_layer_1->normal_texture_settings; tex_ui->checkbox->isChecked())
			this->createTexSettings(settings, settings.texlayer_1.normalsampler, textures.n1_texture_name, textures.n1_texture, *tex_ui);
		if (const auto& tex_ui = this->texture_layer_2->diffuse_texture_settings; tex_ui->checkbox->isChecked())
			this->createTexSettings(settings, settings.texlayer_2.colorsampler, textures.c2_texture_name, textures.c2_texture, *tex_ui);
		if (const auto& tex_ui = this->texture_layer_2->normal_texture_settings; tex_ui->checkbox->isChecked())
			this->createTexSettings(settings, settings.texlayer_2.normalsampler, textures.n2_texture_name, textures.n2_texture, *tex_ui);
		// Handle UV adjustments
		this->createUVSettings(settings.uv_slots[0], *this->uv_settings_1);
		this->createUVSettings(settings.uv_slots[1], *this->uv_settings_2);
		this->createUVSettings(settings.uv_slots[2], *this->uv_settings_3);

		// Parallax
		this->createParallaxSettings(settings);
		this->createBumpSettings(settings);

	}

	void assignTextureReferences(MaterialPtr& material, TextureRefs& textures)
	{
		if (textures.c1_texture)
		{
			material->setTextureBuffer(0x32, textures.c1_texture->getBufferID());
			material->setTextureName(0x32, textures.c1_texture_name);
		}
		if (textures.n1_texture)
		{
			material->setTextureBuffer(0x35, textures.n1_texture->getBufferID());
			material->setTextureName(0x35, textures.n1_texture_name);
		}
		if (textures.c2_texture)
		{
			material->setTextureBuffer(0x44, textures.c2_texture->getBufferID());
			material->setTextureName(0x44, textures.c2_texture_name);
		}
		if (textures.n2_texture)
		{
			material->setTextureBuffer(0x45, textures.n2_texture->getBufferID());
			material->setTextureName(0x45, textures.n2_texture_name);
		}
		if (textures.light_texture)
		{
			material->setTextureBuffer(0x43, textures.light_texture->getBufferID());
			material->setTextureName(0x43, textures.light_texture_name);
		}
	}

	void assignDefaultValues(MaterialPtr& material)
	{
		material->setUniformValue(0x33, { 1.0f, 1.0f, 1.0f, 1.0f }); // DiffuseColor
	}

	void readbackUISettings()
	{
		for (const auto& [id, texture] : this->selected_material->texture_refs)
		{
			QComboBox* cbox = nullptr;
			switch (id)
			{
			case 0x32:
				cbox = this->texture_layer_1->diffuse_texture_settings->file_combo_box->combobox;
				break;
			case 0x35:
				cbox = this->texture_layer_1->normal_texture_settings->file_combo_box->combobox;
				break;
			case 0x44:
				cbox = this->texture_layer_2->diffuse_texture_settings->file_combo_box->combobox;
				break;
			case 0x45:
				cbox = this->texture_layer_2->normal_texture_settings->file_combo_box->combobox;
				break;
			default:
				return;
			}
			cbox->setCurrentText(QString::fromStdString(texture->img_name));
		}
	}

	void regenerateMaterial()
	{
		FactorySettings settings;
		TextureRefs textures;
		this->createSettingsFromUI(settings, textures);
		// Sort out remaining inputs to the factory
		for (const auto& attr : this->selected_mesh->mesh.vertex_attributes)	
		{
			if (attr.attribute_type == FileFormats::DSCS::GeomFile::VertexAttributeType::WeightedBoneID)
			{
				const auto& num_idxs = attr.num_elements;
				settings.num_vertex_weights = num_idxs;
				if (num_idxs > 0)
				{
					settings.use_skeleton = true;
					settings.use_weights = true;
				}
				else
				{
					settings.use_skeleton = true;
					settings.use_weights = false;
				}
			}
		}

		createSettingInputs(settings);
		auto vshader_text = generateVertexShader(settings);
		auto fshader_text = generateFragmentShader(settings);
		std::shared_ptr<Rendering::ShaderObjects::cgGLShaderObject> shader;
		try
		{
			shader = this->shader_backend->createShaderProgram(vshader_text, fshader_text);
			this->shader_backend->checkBackendForCgError("Compiling shaders...");
		}
		catch (const std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
			std::cout << vshader_text << std::endl;
			std::cout << fshader_text << std::endl;
			return;
		}

		MaterialPtr new_material = std::make_shared<Material>(shader);
		if (this->selected_material)
		{
			new_material->name = this->selected_material->name;
			new_material->name_hash = this->selected_material->name_hash;
			new_material->opengl_settings = this->selected_material->opengl_settings;
		}
		else
		{
			new_material->name = "New Material";
			new_material->name_hash = dscsNameHash(new_material->name);
		}
		new_material->initShaderUniforms(this->animation_buffer.uniform_dispatch_buffer);
		this->assignTextureReferences(new_material, textures);
		this->assignDefaultValues(new_material);

		this->active_local_material = new_material;
		emit this->overwriteCurrentMaterial(new_material);
	}

	void placeInSpoiler(const QString& title, QWidget* widget, QLayout* layout)
	{
		auto* spoiler = new Spoiler(title, 300, this);
		spoiler->setContentLayout(*widget->layout());
		layout->addWidget(spoiler);
	}

signals:
	void overwriteCurrentMaterial(MaterialPtr material_ptr);
public:
	explicit ShaderFactory(TextureLibrary_t& texlib, ShaderBackend_t& shader_backend, Rendering::DSCS::AnimationBuffer& animation_buffer, QWidget* parent = nullptr)
		: QWidget(parent)
		, texture_library{ texlib }
		, shader_backend{ shader_backend }
		, animation_buffer{ animation_buffer }
	{

		auto _layout = new QVBoxLayout;
		{
			// Put in the Position manip / Billboarding here

			auto compile_button = new QPushButton("Compile");
			_layout->addWidget(compile_button);
			connect(compile_button, &QPushButton::clicked, this, &ShaderFactory::regenerateMaterial);
			this->texture_layer_1 = new ShaderFactoryTextureLayer1("Texture Layer 1");
			this->texture_layer_2 = new ShaderFactoryTextureLayer1("Texture Layer 2");

			_layout->addWidget(this->texture_layer_1);
			_layout->addWidget(this->texture_layer_2);


			auto uv_settings_layout = new QHBoxLayout;
			{
				this->uv_settings_1 = new ShaderFactoryUVSettingsWidget("UV Map 1");
				this->uv_settings_2 = new ShaderFactoryUVSettingsWidget("UV Map 2");
				this->uv_settings_3 = new ShaderFactoryUVSettingsWidget("UV Map 3");
				uv_settings_layout->addWidget(this->uv_settings_1);
				uv_settings_layout->addWidget(this->uv_settings_2);
				uv_settings_layout->addWidget(this->uv_settings_3);
			}
			_layout->addItem(uv_settings_layout);


			auto contribs_layout = new QHBoxLayout;
			{
				this->diffuse_color_settings = new DiffuseColorSettings(this);
				contribs_layout->addWidget(this->diffuse_color_settings);
			}
			_layout->addItem(contribs_layout);

			this->specular_color_settings = new SpecularColorSettings(this);
			_layout->addWidget(this->specular_color_settings);

			this->reflection_settings = new ReflectionSettings(this);
			_layout->addWidget(this->reflection_settings);

			this->illumination_settings = new IlluminationSettings(this);
			_layout->addWidget(this->illumination_settings);

			this->glassmap_settings = new GlassMapSettings(this);
			_layout->addWidget(this->glassmap_settings);

			this->position_settings = new PositionSettings(this);
			//placeInSpoiler("Position Adjust", this->position_settings, _layout);
			_layout->addWidget(this->position_settings);
		}
		this->setLayout(_layout);

		this->updateAvailableTextures();
	}

	/*
	Update methods for the data on this widget
	*/
	void updateSelectedMesh(MeshPtr mesh_ptr)
	{
		this->selected_mesh = mesh_ptr;
	}

	void updateSelectedMaterial(MaterialPtr material_ptr)
	{
		this->selected_material = material_ptr;
		this->updateAvailableTextures(); // This should update in other places
		this->readbackUISettings();
	}

};