#pragma once

#include <array>

#include <QCheckBox>
#include <QComboBox>
#include <QBoxLayout>
#include <QDoubleValidator>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QWidget>

#include "ShaderGenerator/VertexShader.hpp"
#include "ShaderGenerator/FragmentShader.hpp"
#include "../TabMaterials.hpp"
#include "../../../../Spoiler/Spoiler.hpp"
#include "../../../../../Rendering/DSCS/DataObjects/OpenGLDSCSModel.hpp"
#include "../../../../../Rendering/DSCS/DataObjects/OpenGLDSCSMesh.hpp"
#include "../../../../../Rendering/DSCS/DataObjects/OpenGLDSCSMaterial.hpp"
#include "../../../../../Rendering/DSCS/ShaderSystem/cgGL/cgGLShaderBackend.hpp"
#include "../../../../../Utils/Hashing.hpp"
#include "UI/SelectedObjectReferences.hpp"

#include "Widgets/BumpmapBox.hpp"
#include "Widgets/DiffuseColor.hpp"
#include "Widgets/GlassMap.hpp"
#include "Widgets/Illumination.hpp"
#include "Widgets/ParallaxBox.hpp"
#include "Widgets/PositionSettings.hpp"
#include "Widgets/Reflection.hpp"
#include "Widgets/SpecularColor.hpp"
#include "Widgets/TextureLayer1.hpp"
#include "Widgets/TextureMap.hpp"
#include "Widgets/TextureSlot.hpp"
#include "Widgets/UVSlot.hpp"
#include "Widgets/BaseWidgets/FileCombobox.hpp"
#include "Widgets/BaseWidgets/TextboxArrayWidget.hpp"
#include "Widgets/BaseWidgets/TitleWidget.hpp"
#include "Widgets/BaseWidgets/ToggleableCombobox.hpp"
#include "Widgets/BaseWidgets/ToggleableTextboxes.hpp"



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
	bool isSphere = false;

	std::string clut_texture_name;
	Texture clut_texture = nullptr;
};

class ShaderFactory : public QWidget
{
	Q_OBJECT;

private:
	typedef Rendering::DSCS::DataObjects::OpenGLDSCSModel Model;
	typedef std::shared_ptr<Model> ModelPtr;
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
	MaterialPtr active_local_material = nullptr;
	TextureLibrary_t& texture_library;
	ShaderBackend_t& shader_backend;
	SelectedObjectReferences& selected_objects;
	TabMaterialsLibrary& tab_materials;
	bool setting_update_in_progress = false;

	bool anyLayer1SamplerEnabled()
	{
		return this->texture_layer_1->diffuse_texture_settings->checkbox->isChecked()
			|| this->texture_layer_1->normal_texture_settings->checkbox->isChecked()
			|| this->diffuse_color_settings->light_sampler->checkbox->isChecked();
	}

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
		this->updateTexturesOn(this->diffuse_color_settings->light_sampler->file_combo_box->combobox);
		this->updateTexturesOn(this->illumination_settings->clut->file_combo_box->combobox);
		this->updateTexturesOn(this->reflection_settings->env_texture->file_combo_box->combobox);
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
		if (this->texture_layer_1->parallax_settings->isActive())
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

	void createDiffuseColorSettings(FactorySettings& settings)
	{
		if (this->diffuse_color_settings->vertex_colors_widget->checkbox->isChecked())
		{
			settings.use_vertex_colors = true;
			auto vertex_color_setting = this->diffuse_color_settings->vertex_colors_widget->combobox->currentText();
			if (vertex_color_setting == "RGBA")
			{
				settings.layer_1_vertex_rgb = true;
				settings.layer_1_vertex_alpha = true;
			}
			else if (vertex_color_setting == "RGB")
			{
				settings.layer_1_vertex_rgb = true;
			}
			else
			{
				throw std::exception("Unknown vertex color setting");
			}
		}

		if (this->diffuse_color_settings->transparency_map_widget->isActive())
		{
			settings.transparency_map.enabled = true;
			settings.transparency_map.type = this->diffuse_color_settings->transparency_map_widget->getMapType();
			settings.transparency_map.channel = this->diffuse_color_settings->transparency_map_widget->getChannel();
		}
		if (this->diffuse_color_settings->diffuse_map_widget->isActive())
		{
			settings.diffuse_map.enabled = true;
			settings.diffuse_map.type = this->diffuse_color_settings->diffuse_map_widget->getMapType();
			settings.diffuse_map.channel = this->diffuse_color_settings->diffuse_map_widget->getChannel();
		}
		if (this->diffuse_color_settings->diffuse_map_widget_layer_2->isActive())
		{
			settings.layer_2_diffuse_map.enabled = true;
			settings.layer_2_diffuse_map.type = this->diffuse_color_settings->diffuse_map_widget_layer_2->getMapType();
			settings.layer_2_diffuse_map.channel = this->diffuse_color_settings->diffuse_map_widget_layer_2->getChannel();
		}
	}

	void createSpecularColorSettings(FactorySettings& settings)
	{
		settings.specular_input = this->specular_color_settings->checkbox->isChecked();
		if (this->specular_color_settings->specular_map->isActive())
		{
			settings.specular_map.enabled = true;
			settings.specular_map.type = this->specular_color_settings->specular_map->getMapType();
			settings.specular_map.channel = this->specular_color_settings->specular_map->getChannel();
		}
	}

	void createIlluminationSettings(FactorySettings& settings)
	{
		settings.dir_light_1 = this->illumination_settings->receive_lamp->isActive();
		if (settings.dir_light_1)
		{
			settings.use_velvet = this->illumination_settings->velvet->isActive();
			settings.use_clut = this->illumination_settings->clut->isActive();
		}
		settings.use_ambient = this->illumination_settings->receive_ambient->isActive();
		settings.use_ambient_gndsky = this->illumination_settings->receive_sky->isActive();

		settings.fog = this->illumination_settings->receive_fog->isActive();
		settings.fog_height = this->illumination_settings->receive_fog->isActive();

		settings.use_obscure = this->illumination_settings->obscure->isActive();
		settings.ambient_occlusion.enabled = this->illumination_settings->ambient_occlusion_map->isActive();
		settings.ambient_occlusion.type = this->illumination_settings->ambient_occlusion_map->getMapType();
		settings.ambient_occlusion.channel = this->illumination_settings->ambient_occlusion_map->getChannel();
		settings.receive_shadows = this->illumination_settings->receive_shadow_map->isActive();
	}

	void createReflectionSettings(FactorySettings& settings)
	{
		settings.use_reflection = this->reflection_settings->isActive();
		settings.refl_sphere_map = this->reflection_settings->isSphereMap();
		settings.use_fresnel = this->reflection_settings->fresnel->isActive();
	}

	void createBumpSettings(FactorySettings& settings)
	{
		if (this->texture_layer_1->normal_texture_settings->isActive())
		{
			settings.use_tangents = true;
		}
	}


	void createVertexSettings(FactorySettings& settings)
	{
		settings.calculate_binormal = this->position_settings->calculate_binormal;
		settings.fat = this->position_settings->fat->checkbox->isChecked();
		settings.zbias = this->position_settings->zbias->checkbox->isChecked();
		settings.is_billboard = this->position_settings->billboard_checkbox->isActive();
		settings.align_with_y = this->position_settings->billboard_align_y_checkbox->isActive();
	}

	void createSettingsFromUI(FactorySettings& settings, TextureRefs& textures)
	{
		// Load textures
		if (const auto& tex_ui = this->texture_layer_1->diffuse_texture_settings; tex_ui->isActive())
			this->createTexSettings(settings, settings.texlayer_1.colorsampler, textures.c1_texture_name, textures.c1_texture, *tex_ui);
		if (const auto& tex_ui = this->texture_layer_1->normal_texture_settings; tex_ui->isActive())
			this->createTexSettings(settings, settings.texlayer_1.normalsampler, textures.n1_texture_name, textures.n1_texture, *tex_ui);
		if (const auto& tex_ui = this->texture_layer_2->diffuse_texture_settings; tex_ui->isActive())
			this->createTexSettings(settings, settings.texlayer_2.colorsampler, textures.c2_texture_name, textures.c2_texture, *tex_ui);
		if (const auto& tex_ui = this->texture_layer_2->normal_texture_settings; tex_ui->isActive())
			this->createTexSettings(settings, settings.texlayer_2.normalsampler, textures.n2_texture_name, textures.n2_texture, *tex_ui);
		if (const auto& tex_ui = this->diffuse_color_settings->light_sampler; tex_ui->isActive())
			this->createTexSettings(settings, settings.lightsampler, textures.light_texture_name, textures.light_texture, *tex_ui);
		if (const auto& tex_ui = this->illumination_settings->clut; tex_ui->isActive())
		{
			auto texname = tex_ui->file_combo_box->combobox->currentText().toStdString();
			textures.clut_texture_name = texname;
			textures.clut_texture = this->texture_library.at(texname);
		}
		if (const auto& tex_ui = this->reflection_settings->env_texture; tex_ui->isActive())
		{
			auto texname = tex_ui->file_combo_box->combobox->currentText().toStdString();
			textures.env_texture_name = texname;
			textures.env_texture = this->texture_library.at(texname);
			textures.isSphere = this->reflection_settings->isSphereMap();
		}

		// Handle UV adjustments
		this->createUVSettings(settings.uv_slots[0], *this->uv_settings_1);
		this->createUVSettings(settings.uv_slots[1], *this->uv_settings_2);
		this->createUVSettings(settings.uv_slots[2], *this->uv_settings_3);

		// Parallax
		this->createParallaxSettings(settings);
		this->createBumpSettings(settings);

		// Various Color Contributions
		this->createDiffuseColorSettings(settings);
		this->createSpecularColorSettings(settings);
		this->createIlluminationSettings(settings);
		this->createReflectionSettings(settings);

		// Vertex
		this->createVertexSettings(settings);

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
		if (textures.light_texture)
		{
			material->setTextureBuffer(0x43, textures.light_texture->getBufferID());
			material->setTextureName(0x43, textures.light_texture_name);
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
		if (textures.clut_texture)
		{
			material->setTextureBuffer(0x48, textures.clut_texture->getBufferID());
			material->setTextureName(0x48, textures.clut_texture_name);
		}
		if (textures.env_texture)
		{
			if (textures.isSphere)
			{
				material->setTextureBuffer(0x8E, textures.env_texture->getBufferID());
				material->setTextureName(0x8E, textures.env_texture_name);
			}
			else
			{
				material->setTextureBuffer(0x3A, textures.env_texture->getBufferID());
				material->setTextureName(0x3A, textures.env_texture_name);
			}
		}
	}

	void assignDefaultValues(MaterialPtr& material)
	{
		material->setUniformValue(0x33, { 1.0f, 1.0f, 1.0f, 1.0f }); // DiffuseColor
	}

	void readbackUISettings()
	{
		auto& selected_material = this->selected_objects.getSelectedMaterial();
		for (const auto& [id, texture] : selected_material->texture_refs)
		{
			if (texture->texture_type == CG_SAMPLER2D)
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
				case 0x43:
					cbox = this->diffuse_color_settings->light_sampler->file_combo_box->combobox;
					break;
				case 0x44:
					cbox = this->texture_layer_2->diffuse_texture_settings->file_combo_box->combobox;
					break;
				case 0x45:
					cbox = this->texture_layer_2->normal_texture_settings->file_combo_box->combobox;
					break;
				case 0x48:
					cbox = this->illumination_settings->clut->file_combo_box->combobox;
					break;
				default:
					return;
				}
				cbox->setCurrentText(QString::fromStdString(texture->img_name));
			}
		}
	}

	void regenerateMaterial()
	{
		FactorySettings settings;
		TextureRefs textures;
		this->createSettingsFromUI(settings, textures);

		// Need to figure out a way to do this with the new resource system...
		// Probably need to store a list of Mesh references on the MaterialResource..?
		auto& selected_mesh = this->selected_objects.getSelectedMesh();
		// Sort out remaining inputs to the factory
		for (const auto& attr : selected_mesh->mesh.vertex_attributes)	
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

		auto& material_resource = this->selected_objects.getEditableSelectedMaterialResource();
		auto& curr_material = material_resource.getEditableFactoryMaterial();
		curr_material->replaceShader(shader, this->animation_buffer.uniform_dispatch_buffer);
		this->assignTextureReferences(curr_material, textures);
		this->assignDefaultValues(curr_material);

		this->setActiveMaterialAsSelected(); // This should get linked to buttons...

		this->setting_update_in_progress = false;
	}

	void setActiveMaterialAsSelected()
	{
		auto& material_resource = this->selected_objects.getEditableSelectedMaterialResource();

		FactorySettings settings;
		TextureRefs textures;
		this->createSettingsFromUI(settings, textures);

		// Need to automate this by giving Materials an ability to copy data from each other...
		auto& curr_material = material_resource.getEditableMaterial();
		curr_material->replaceShader(material_resource.getFactoryMaterial()->shader, this->animation_buffer.uniform_dispatch_buffer);
		this->assignTextureReferences(curr_material, textures);
		this->assignDefaultValues(curr_material);

		emit this->selected_objects.setSelectedMaterial(this->selected_objects.getSelectedMaterial());
	}

	void placeInSpoiler(const QString& title, QWidget* widget, QLayout* layout)
	{
		auto* spoiler = new Spoiler(title, 300, this);
		spoiler->setContentLayout(*widget->layout());
		layout->addWidget(spoiler);
	}

	bool uvSlotInUse(ShaderFactoryTextureSlot*& slot, size_t idx)
	{
		return slot->checkbox->isChecked() && (slot->uv_slot_combobox->currentIndex() == idx);
	}

	bool getUVSlot(size_t idx)
	{
		return this->uvSlotInUse(this->texture_layer_1->diffuse_texture_settings, idx)
			|| this->uvSlotInUse(this->texture_layer_1->normal_texture_settings, idx)
			|| this->uvSlotInUse(this->texture_layer_2->diffuse_texture_settings, idx)
			|| this->uvSlotInUse(this->texture_layer_2->normal_texture_settings, idx)
			|| this->uvSlotInUse(this->diffuse_color_settings->light_sampler, idx);
	}

	void blockUISignals()
	{
		this->uv_settings_1->blockSignals(true);
		this->uv_settings_2->blockSignals(true);
		this->uv_settings_3->blockSignals(true);
		this->texture_layer_1->blockSignals(true);
		this->texture_layer_2->blockSignals(true);
		this->diffuse_color_settings->blockSignals(true);
		this->specular_color_settings->blockSignals(true);
		this->illumination_settings->blockSignals(true);
		this->reflection_settings->blockSignals(true);
		this->glassmap_settings->blockSignals(true);
		this->position_settings->blockSignals(true);
	}

	void unblockUISignals()
	{
		this->uv_settings_1->blockSignals(false);
		this->uv_settings_2->blockSignals(false);
		this->uv_settings_3->blockSignals(false);
		this->texture_layer_1->blockSignals(false);
		this->texture_layer_2->blockSignals(false);
		this->diffuse_color_settings->blockSignals(false);
		this->specular_color_settings->blockSignals(false);
		this->illumination_settings->blockSignals(false);
		this->reflection_settings->blockSignals(false);
		this->glassmap_settings->blockSignals(false);
		this->position_settings->blockSignals(false);
	}

	void updateUI()
	{
		this->blockUISignals();
		std::cout << "UPDATE UI" << std::endl;
		if (this->setting_update_in_progress)
			return;
		this->setting_update_in_progress = true;


		// Get the UV slots in use
		this->uv_settings_1->toggle(getUVSlot(0));
		this->uv_settings_2->toggle(getUVSlot(1));
		this->uv_settings_3->toggle(getUVSlot(2));

		// Diffuse 1
		if (this->texture_layer_1->diffuse_texture_settings->isActive())
		{
			this->diffuse_color_settings->transparency_map_widget->addDiffuseMap();
			this->diffuse_color_settings->diffuse_map_widget->addDiffuseMap();
			this->specular_color_settings->specular_map->addDiffuseMap();
			this->illumination_settings->ambient_occlusion_map->addDiffuseMap();
		}
		else
		{
			this->diffuse_color_settings->transparency_map_widget->removeDiffuseMap();
			this->diffuse_color_settings->diffuse_map_widget->removeDiffuseMap();
			this->specular_color_settings->specular_map->removeDiffuseMap();
			this->illumination_settings->ambient_occlusion_map->removeDiffuseMap();
		}

		// Normal 1
		if (this->texture_layer_1->normal_texture_settings->isActive())
		{
			this->diffuse_color_settings->transparency_map_widget->addNormalMap();
			this->diffuse_color_settings->diffuse_map_widget->addNormalMap();
			this->specular_color_settings->specular_map->addNormalMap();
			this->illumination_settings->ambient_occlusion_map->addNormalMap();
		}
		else
		{
			this->diffuse_color_settings->transparency_map_widget->removeNormalMap();
			this->diffuse_color_settings->diffuse_map_widget->removeNormalMap();
			this->specular_color_settings->specular_map->removeNormalMap();
			this->illumination_settings->ambient_occlusion_map->removeNormalMap();
		}

		// Light
		if (this->diffuse_color_settings->light_sampler->isActive())
		{
			this->specular_color_settings->specular_map->addLightMap();
			this->illumination_settings->ambient_occlusion_map->addLightMap();
		}
		else
		{
			this->specular_color_settings->specular_map->removeLightMap();
			this->illumination_settings->ambient_occlusion_map->removeLightMap();
		}

		// Diffuse 2
		if (this->texture_layer_2->diffuse_texture_settings->isActive())
		{
			this->diffuse_color_settings->diffuse_map_widget_layer_2->addDiffuseMap<true>();
		}
		else
		{
			this->diffuse_color_settings->diffuse_map_widget_layer_2->removeDiffuseMap();
		}

		// Normal 2
		if (this->texture_layer_2->normal_texture_settings->isActive())
		{
			this->diffuse_color_settings->diffuse_map_widget_layer_2->addNormalMap<true>();
		}
		else
		{
			this->diffuse_color_settings->diffuse_map_widget_layer_2->removeNormalMap();
		}
		
		this->diffuse_color_settings->transparency_map_widget->setActive();
		this->diffuse_color_settings->diffuse_map_widget->setActive();
		this->diffuse_color_settings->diffuse_map_widget_layer_2->setActive();

		this->regenerateMaterial();
		this->unblockUISignals();
	}

	template <typename T>
	auto getSettingsUpdated(T* widget)
	{
		return &std::remove_reference<decltype(*widget)>::type::settingsUpdated;
	}

	template <typename T>
	auto connectUIUpdate(T* widget)
	{
		return connect(widget, getSettingsUpdated(widget), this, &ShaderFactory::updateUI);
	}

	void connectUI()
	{
		connectUIUpdate(this->texture_layer_1);
		connectUIUpdate(this->texture_layer_2);
		connectUIUpdate(this->uv_settings_1);
		connectUIUpdate(this->uv_settings_2);
		connectUIUpdate(this->uv_settings_3);
		connectUIUpdate(this->diffuse_color_settings);
		// connectUIUpdate(this->specular_color_settings);
		// connectUIUpdate(this->reflection_settings);
		// connectUIUpdate(this->illumination_settings);
		// connectUIUpdate(this->glassmap_settings);
		// connectUIUpdate(this->position_settings);
	}



public:
	explicit ShaderFactory(SelectedObjectReferences& sor, TabMaterialsLibrary& tab_materials, TextureLibrary_t& texlib, ShaderBackend_t& shader_backend, Rendering::DSCS::AnimationBuffer& animation_buffer, QWidget* parent = nullptr)
		: QWidget(parent)
		, texture_library{ texlib }
		, shader_backend{ shader_backend }
		, animation_buffer{ animation_buffer }
		, selected_objects{ sor }
		, tab_materials{ tab_materials }
	{
		auto _layout = new QVBoxLayout;
		{
			// Put in the Position manip / Billboarding here

			auto compile_button = new QPushButton("Set Active");
			_layout->addWidget(compile_button);
			connect(compile_button, &QPushButton::clicked, this, &ShaderFactory::setActiveMaterialAsSelected);
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
		this->connectUI();

		this->updateAvailableTextures();

		connect(&this->selected_objects, &SelectedObjectReferences::selectedMaterialUpdated, this, &ShaderFactory::updateReadbackSettings);
	}


	void updateReadbackSettings()
	{
		this->blockUISignals();
		this->updateAvailableTextures(); // This should update in other places
		this->readbackUISettings();

		this->unblockUISignals();
	}

};