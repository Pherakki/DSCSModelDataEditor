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
#include "../../../Rendering/DSCS/DataObjects/OpenGLDSCSMaterial.hpp"
#include "../../../Rendering/DSCS/ShaderSystem/cgGL/cgGLShaderBackend.hpp"
#include "../../../Utils/Hashing.hpp"


template <uint8_t n_boxes>
class ToggleableTextboxesWidget : public QWidget
{

private:
	QCheckBox* checkbox;
	QLabel* label;
	std::array<QLineEdit*, n_boxes> textboxes;

public:
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
	}
};

class TitleWidget : public QWidget
{
private:
	QLabel* label;

public:
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

private:
	TitleWidget* title;
	ToggleableTextboxesWidget<0>* widget_projection;
	ToggleableTextboxesWidget<2>* widget_scrollspeed;
	ToggleableTextboxesWidget<1>* widget_rotation;
	ToggleableTextboxesWidget<2>* widget_offset;
	ToggleableTextboxesWidget<2>* widget_scale;

public:
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
	}
};

class ShaderFactoryTextureLayerParallaxBox : public QWidget
{
private:
	QCheckBox* checkbox;
	QLabel*    label;
	QLabel*    bias_label;
	QLineEdit* bias_x;
	QLineEdit* bias_y;
	QLabel*    heightmap_label;
	QComboBox* heightmap_combobox;

public:
	ShaderFactoryTextureLayerParallaxBox(QWidget* parent = nullptr) : QWidget(parent)
	{
		auto _layout = new QVBoxLayout;
		{
			auto title_layout = new QHBoxLayout;
			{
				this->checkbox = new QCheckBox;
				this->label = new QLabel("Parallax");
				title_layout->addWidget(this->checkbox);
				title_layout->addWidget(this->label);
			}
			_layout->addItem(title_layout);

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
					heightmap_settings_layout->addWidget(this->heightmap_label);
					heightmap_settings_layout->addWidget(this->heightmap_combobox);
				}
				settings_layout->addItem(heightmap_settings_layout);
			}
			_layout->addItem(settings_layout);
		}
		this->setLayout(_layout);
	}
};

class ShaderFactoryTextureLayerBumpmapBox : public QWidget
{
private:
	QCheckBox* checkbox;
	QLabel* label;
	QLabel* bump_label;
	QLineEdit* bump_strength;

public:
	ShaderFactoryTextureLayerBumpmapBox(QWidget* parent = nullptr) : QWidget(parent)
	{
		auto _layout = new QVBoxLayout;
		{
			auto title_layout = new QHBoxLayout;
			{
				this->checkbox = new QCheckBox;
				this->label = new QLabel("Bump");
				title_layout->addWidget(this->checkbox);
				title_layout->addWidget(this->label);
			}
			_layout->addItem(title_layout);

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
			_layout->addItem(settings_layout);
		}
		this->setLayout(_layout);
	}
};

class ShaderFactoryTextureLayer1 : public QWidget
{
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
	}
};

class ShaderFactory : public QWidget
{
	Q_OBJECT;

private:
	typedef std::unordered_map<std::string, std::shared_ptr<Rendering::DataObjects::OpenGLDSCSTexture>> TextureLibrary_t;
	typedef Rendering::DSCS::DataObjects::OpenGLDSCSMaterial Material;
	typedef std::shared_ptr<Material> MaterialPtr;
	typedef std::unique_ptr<Rendering::ShaderBackends::cgGLShaderBackend> ShaderBackend_t;
	typedef Rendering::DSCS::AnimationBuffer AnimBuf_t;

	ShaderFactoryTextureLayer1* texture_layer_1;
	ShaderFactoryTextureLayer1* texture_layer_2;
	ShaderFactoryUVSettingsWidget* uv_settings_1;
	ShaderFactoryUVSettingsWidget* uv_settings_2;
	ShaderFactoryUVSettingsWidget* uv_settings_3;

	FactorySettings factory_settings;
	AnimBuf_t& animation_buffer;
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

	void regenerateMaterial()
	{
		auto vshader_text = generateVertexShader(this->factory_settings);
		auto fshader_text = generateFragmentShader(this->factory_settings);
		auto shader = this->shader_backend->createShaderProgram(vshader_text, fshader_text);
		try
		{
			this->shader_backend->checkBackendForCgError("Compiling shaders...");
		}
		catch (const std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
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
		this->active_local_material = new_material;
		emit this->overwriteCurrentMaterial(new_material);
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

			auto misc_settings_layout = new QHBoxLayout;
			{
				auto l1 = new QLabel("Diffuse/Spec/Samplers");
				auto l2 = new QLabel("Reflection/Glass map");
				auto l3 = new QLabel("Illumination");
				misc_settings_layout->addWidget(l1);
				misc_settings_layout->addWidget(l2);
				misc_settings_layout->addWidget(l3);
			}
			_layout->addItem(misc_settings_layout);
		}
		this->setLayout(_layout);

		this->updateAvailableTextures();
	}

	/*
	Update methods for the data on this widget
	*/
	void updateSelectedMaterial(MaterialPtr material_ptr)
	{
		this->selected_material = material_ptr;
	}

};