#pragma once

#include <array>
#include <sstream>

#include <QCheckBox>
#include <QComboBox>
#include <QCryptographicHash>
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
#include "Widgets/TextureLayer2.hpp"
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
	uint32_t c1_texture = 0;

	std::string n1_texture_name;
	uint32_t n1_texture = 0;

	std::string c2_texture_name;
	uint32_t c2_texture = 0;

	std::string n2_texture_name;
	uint32_t n2_texture = 0;

	std::string light_texture_name;
	uint32_t light_texture = 0;

	std::string env_texture_name;
	uint32_t env_texture = 0;
	bool isSphere = false;

	std::string clut_texture_name;
	uint32_t clut_texture = 0;
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
	typedef std::unordered_map<std::string, TexturePtr> TextureSubLibrary_t;
	typedef Rendering::DSCS::DataObjects::OpenGLDSCSMaterial Material;
	typedef std::shared_ptr<Material> MaterialPtr;
	typedef std::unique_ptr<Rendering::ShaderBackends::cgGLShaderBackend> ShaderBackend_t;
	typedef Rendering::DSCS::AnimationBuffer AnimBuf_t;

	QLabel* matching_shader = new QLabel(this);
	ShaderFactoryTextureLayer1* texture_layer_1;
	ShaderFactoryTextureLayer2* texture_layer_2;
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
	ShaderBackend_t& shader_backend;
	SelectedObjectReferences& selected_objects;
	TabMaterialsLibrary& tab_materials;
	bool setting_update_in_progress = false;

	bool anyLayer1SamplerEnabled()
	{
		return this->texture_layer_1->diffuse_texture_settings->isActive()
			|| this->texture_layer_1->normal_texture_settings->isActive()
			|| this->diffuse_color_settings->light_sampler->isActive();
	}

	void updateTexturesOn(QComboBox* combobox, TextureLibrary::TextureSubLibrary_t& tex_sublib)
	{
		combobox->clear();
		for (const auto& [texture_name, texture] : tex_sublib)
		{
			combobox->addItem(QString::fromStdString(texture_name), QString::fromStdString(texture_name));
		}
	}

	void update2DTexturesOn(QComboBox* combobox)
	{
		this->updateTexturesOn(combobox, this->selected_objects.texture_library.textures_2d);
	}
	void updateCLUTTexturesOn(QComboBox* combobox)
	{
		this->updateTexturesOn(combobox, this->selected_objects.texture_library.textures_clut);
	}
	void updateLumTexturesOn(QComboBox* combobox)
	{
		this->updateTexturesOn(combobox, this->selected_objects.texture_library.textures_luminance);
	}
	void updateCubeTexturesOn(QComboBox* combobox)
	{
		this->updateTexturesOn(combobox, this->selected_objects.texture_library.textures_cube);
	}

	void updateAvailableTextures()
	{
		this->update2DTexturesOn(this->texture_layer_1->diffuse_texture_settings->file_combo_box->combobox);
		this->update2DTexturesOn(this->texture_layer_1->normal_texture_settings->file_combo_box->combobox);
		this->update2DTexturesOn(this->texture_layer_2->diffuse_texture_settings->file_combo_box->combobox);
		this->update2DTexturesOn(this->texture_layer_2->normal_texture_settings->file_combo_box->combobox);
		this->updateLumTexturesOn(this->diffuse_color_settings->light_sampler->file_combo_box->combobox);
		this->updateCLUTTexturesOn(this->illumination_settings->clut->file_combo_box->combobox);
		if (this->reflection_settings->isSphereMap())
			this->update2DTexturesOn(this->reflection_settings->env_texture->file_combo_box->combobox);
		else
			this->updateCubeTexturesOn(this->reflection_settings->env_texture->file_combo_box->combobox);
	}

	void createTexSettings(FactorySettings& settings, Sampler& sampler, std::string& texname, uint32_t& texture, const ShaderFactoryTextureSlot& tex_ui, TextureLibrary::TextureSubLibrary_t& sublib)
	{
		sampler.enabled = true;
		sampler.uv_slot = tex_ui.uv_slot_combobox->currentIndex();
		settings.uv_slots[sampler.uv_slot].enabled = true;
		sampler.combined_channels = true; // Change later
		texname = tex_ui.file_combo_box->combobox->currentText().toStdString();
		texture = sublib.at(texname);
	}

	void createTexSettings2D(FactorySettings& settings, Sampler& sampler, std::string& texname, uint32_t& texture, const ShaderFactoryTextureSlot& tex_ui)
	{
		this->createTexSettings(settings, sampler, texname, texture, tex_ui, this->selected_objects.texture_library.textures_2d);
	}

	void createTexSettingsLum(FactorySettings& settings, Sampler& sampler, std::string& texname, uint32_t& texture, const ShaderFactoryTextureSlot& tex_ui)
	{
		this->createTexSettings(settings, sampler, texname, texture, tex_ui, this->selected_objects.texture_library.textures_luminance);
	}

	void createUVSettings(TexSlot& uv_slot, const ShaderFactoryUVSettingsWidget& uv_ui)
	{
		uv_slot.is_projection = uv_ui.widget_projection->isActive();
		uv_slot.scroll = uv_ui.widget_scrollspeed->isActive();
		uv_slot.offset = uv_ui.widget_offset->isActive();
		uv_slot.rotation = uv_ui.widget_rotation->isActive();
		uv_slot.scale = uv_ui.widget_scale->isActive();
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
				throw std::runtime_error("Error: parallax maptype not diffuse or normal.");
			settings.parallax_heightmap_channel = RGBAChannel::A; // Make this configurable
		}
	}

	void createDiffuseColorSettings(FactorySettings& settings)
	{
		if (this->diffuse_color_settings->vertex_colors_widget->isActive())
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
				throw std::runtime_error("Unknown vertex color setting");
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
		settings.fat = this->position_settings->fat->isActive();
		settings.zbias = this->position_settings->zbias->isActive();
		settings.is_billboard = this->position_settings->billboard_checkbox->isActive();
		settings.align_with_y = this->position_settings->billboard_align_y_checkbox->isActive();
	}

	void createSettingsFromUI(FactorySettings& settings, TextureRefs& textures)
	{
		// Load textures
		if (const auto& tex_ui = this->texture_layer_1->diffuse_texture_settings; tex_ui->isActive())
			this->createTexSettings2D(settings, settings.texlayer_1.colorsampler, textures.c1_texture_name, textures.c1_texture, *tex_ui);
		if (const auto& tex_ui = this->texture_layer_1->normal_texture_settings; tex_ui->isActive())
			this->createTexSettings2D(settings, settings.texlayer_1.normalsampler, textures.n1_texture_name, textures.n1_texture, *tex_ui);
		if (const auto& tex_ui = this->texture_layer_2->diffuse_texture_settings; tex_ui->isActive())
			this->createTexSettings2D(settings, settings.texlayer_2.colorsampler, textures.c2_texture_name, textures.c2_texture, *tex_ui);
		if (const auto& tex_ui = this->texture_layer_2->normal_texture_settings; tex_ui->isActive())
			this->createTexSettings2D(settings, settings.texlayer_2.normalsampler, textures.n2_texture_name, textures.n2_texture, *tex_ui);
		if (const auto& tex_ui = this->diffuse_color_settings->light_sampler; tex_ui->isActive())
			this->createTexSettingsLum(settings, settings.lightsampler, textures.light_texture_name, textures.light_texture, *tex_ui);
		if (const auto& tex_ui = this->illumination_settings->clut; tex_ui->isActive())
		{
			auto texname = tex_ui->file_combo_box->combobox->currentText().toStdString();
			textures.clut_texture_name = texname;
			textures.clut_texture = this->selected_objects.texture_library.textures_clut.at(texname);
		}
		if (const auto& tex_ui = this->reflection_settings->env_texture; tex_ui->isActive())
		{
			auto texname = tex_ui->file_combo_box->combobox->currentText().toStdString();
			textures.env_texture_name = texname;
			if (this->reflection_settings->isSphereMap())
				textures.env_texture = this->selected_objects.texture_library.textures_2d.at(texname);
			else
				textures.env_texture = this->selected_objects.texture_library.textures_cube.at(texname);
			textures.isSphere = this->reflection_settings->isSphereMap();
		}

		// Handle texure later specific things
		settings.texlayer_1_distort = this->texture_layer_1->distortion->isActive();
		settings.texlayer_2.colorsampler.enabled = this->texture_layer_2->overlay_strength->isActive();

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
			material->setTextureBuffer(0x32, textures.c1_texture);
			material->setTextureName(0x32, textures.c1_texture_name);
		}
		if (textures.n1_texture)
		{
			material->setTextureBuffer(0x35, textures.n1_texture);
			material->setTextureName(0x35, textures.n1_texture_name);
		}
		if (textures.light_texture)
		{
			material->setTextureBuffer(0x43, textures.light_texture);
			material->setTextureName(0x43, textures.light_texture_name);
		}
		if (textures.c2_texture)
		{
			material->setTextureBuffer(0x44, textures.c2_texture);
			material->setTextureName(0x44, textures.c2_texture_name);
		}
		if (textures.n2_texture)
		{
			material->setTextureBuffer(0x45, textures.n2_texture);
			material->setTextureName(0x45, textures.n2_texture_name);
		}
		if (textures.clut_texture)
		{
			material->setTextureBuffer(0x48, textures.clut_texture);
			material->setTextureName(0x48, textures.clut_texture_name);
		}
		if (textures.env_texture)
		{
			if (textures.isSphere)
			{
				material->setTextureBuffer(0x8E, textures.env_texture);
				material->setTextureName(0x8E, textures.env_texture_name);
			}
			else
			{
				material->setTextureBuffer(0x3A, textures.env_texture);
				material->setTextureName(0x3A, textures.env_texture_name);
			}
		}
	}

	float safeStof(const QString& value) const noexcept
	{

		float f_value;
		try
		{
			f_value = std::stof(value.toStdString());
		}
		catch (const std::exception& ex)
		{
			f_value = 0;
		}
		return f_value;
	}

	auto sanitiseTextChanged(const std::vector<EditorTextbox*>& textboxes)
	{
		std::array<float, 4> out{};
		for (size_t i = 0; i < textboxes.size(); ++i)
			out[i] = this->safeStof(textboxes[i]->text());
		return out;
	}

	//auto sanitiseTextChanged(EditorTextbox* textbox)
	//{
	//	return this->safeStof(textbox->text());
	//}

	void assignUniformValues(MaterialPtr& material)
	{
		//auto& material = this->selected_objects.getEditableSelectedMaterialResource().getEditableFactoryMaterial();

		{ // DiffuseColor
			auto& t = this->diffuse_color_settings->diffuse_color_widget->textboxes;
			material->setUniformValue(0x33, this->sanitiseTextChanged(t));
		}
		{ // Bumpiness
			auto& t = this->texture_layer_1->bumpmap_settings->bump_strength->textboxes;
			material->setUniformValue(0x36, this->sanitiseTextChanged(t));
		}
		{ // SpecularStrength
			auto& t = this->specular_color_settings->specular_strength->textboxes;
			material->setUniformValue(0x38, this->sanitiseTextChanged(t));
		}
		{ // SpecularPower
			auto& t = this->specular_color_settings->specular_power->textboxes;
			material->setUniformValue(0x39, this->sanitiseTextChanged(t));
		}
		{ // ReflectionStrength
			auto& t = this->reflection_settings->reflection_strength->textboxes;
			material->setUniformValue(0x3B, this->sanitiseTextChanged(t));
		}
		{ // FresnelMin
			auto& t = this->reflection_settings->fresnel_min->textboxes;
			material->setUniformValue(0x3C, this->sanitiseTextChanged(t));
		}
		{ // FresnelExp
			auto& t = this->reflection_settings->fresnel_exp->textboxes;
			material->setUniformValue(0x3D, this->sanitiseTextChanged(t));
		}
		{ // SurfaceColor
			auto& t = this->illumination_settings->surface_color->textboxes;
			material->setUniformValue(0x3E, this->sanitiseTextChanged(t));
		}
		{ // FuzzySpecColor
			auto& t = this->illumination_settings->fuzzy_spec_color->textboxes;
			material->setUniformValue(0x3F, this->sanitiseTextChanged(t));
		}
		{ // SubColor
			auto& t = this->illumination_settings->sub_color->textboxes;
			material->setUniformValue(0x40, this->sanitiseTextChanged(t));
		}
		{ // RollOff
			auto& t = this->illumination_settings->rolloff->textboxes;
			material->setUniformValue(0x41, this->sanitiseTextChanged(t));
		}
		{ // VelvetStrength
			auto& t = this->illumination_settings->velvet_strength->textboxes;
			material->setUniformValue(0x42, this->sanitiseTextChanged(t));
		}
		{ // OverlayBumpiness
			auto& t = this->texture_layer_2->bumpmap_settings->bump_strength->textboxes;
			material->setUniformValue(0x46, this->sanitiseTextChanged(t));
		}
		{ // OverlayStrength
			auto& t = this->texture_layer_2->overlay_strength->textboxes;
			material->setUniformValue(0x47, this->sanitiseTextChanged(t));
		}
		{ // GlassStrength
			auto& t = this->glassmap_settings->glass_strength->textboxes;
			material->setUniformValue(0x4B, this->sanitiseTextChanged(t));
		}
		{ // Curvature
			auto& t = this->glassmap_settings->curvature->textboxes;
			material->setUniformValue(0x4C, this->sanitiseTextChanged(t));
		}
		{ // UpsideDown
			//auto& t = this->texture_layer_2->bumpmap_settings->bump_strength;
			//material->setUniformValue(0x4D, this->sanitiseTextChanged(t));
		}
		{ // ParallaxBiasX
			auto& t = this->texture_layer_1->parallax_settings->bias_xy->textboxes[0];
			material->setUniformValue(0x4F, this->sanitiseTextChanged({ t }));
		}
		{ // ParallaxBiasY
			auto& t = this->texture_layer_1->parallax_settings->bias_xy->textboxes[1];
			material->setUniformValue(0x50, this->sanitiseTextChanged({ t }));
		}
		{ // ScrollSpeedSet1
			auto& t = this->uv_settings_1->widget_scrollspeed->textboxes;
			material->setUniformValue(0x55, this->sanitiseTextChanged(t));
		}
		{ // ScrollSpeedSet2
			auto& t = this->uv_settings_2->widget_scrollspeed->textboxes;
			material->setUniformValue(0x58, this->sanitiseTextChanged(t));
		}
		{ // ScrollSpeedSet3
			auto& t = this->uv_settings_3->widget_scrollspeed->textboxes;
			material->setUniformValue(0x5B, this->sanitiseTextChanged(t));
		} 
		{ // OffsetSet1
			auto& t = this->uv_settings_1->widget_offset->textboxes;
			material->setUniformValue(0x5E, this->sanitiseTextChanged(t));
		}
		{ // OffsetSet2
			auto& t = this->uv_settings_2->widget_offset->textboxes;
			material->setUniformValue(0x61, this->sanitiseTextChanged(t));
		}
		{ // OffsetSet3
			auto& t = this->uv_settings_3->widget_offset->textboxes;
			material->setUniformValue(0x74, this->sanitiseTextChanged(t));
		}
		{ // RotationSet1
			auto& t = this->uv_settings_1->widget_rotation->textboxes;
			material->setUniformValue(0x78, this->sanitiseTextChanged(t));
		}
		{ // RotationSet2
			auto& t = this->uv_settings_2->widget_rotation->textboxes;
			material->setUniformValue(0x7B, this->sanitiseTextChanged(t));
		}
		{ // RotationSet3
			auto& t = this->uv_settings_3->widget_rotation->textboxes;
			material->setUniformValue(0x7E, this->sanitiseTextChanged(t));
		}
		{ // ScaleSet1
			auto& t = this->uv_settings_1->widget_scale->textboxes;
			material->setUniformValue(0x81, this->sanitiseTextChanged(t));
		}
		{ // ScaleSet2
			auto& t = this->uv_settings_2->widget_scale->textboxes;
			material->setUniformValue(0x84, this->sanitiseTextChanged(t));
		}
		{ // ScaleSet3
			auto& t = this->uv_settings_3->widget_scale->textboxes;
			material->setUniformValue(0x87, this->sanitiseTextChanged(t));
		}
		{ // DistortionStrength
			auto& t = this->texture_layer_1->distortion->textboxes;
			material->setUniformValue(0x64, this->sanitiseTextChanged(t));
		}
		{ // LightMapPower
			auto& t = this->diffuse_color_settings->light_power_textbox->textboxes;
			material->setUniformValue(0x71, this->sanitiseTextChanged(t));
		}
		{ // LightMapStrength
			auto& t = this->diffuse_color_settings->light_strength_textbox->textboxes;
			material->setUniformValue(0x72, this->sanitiseTextChanged(t));
		}
		{ // Fat
			auto& t = this->position_settings->fat->textboxes;
			material->setUniformValue(0x77, this->sanitiseTextChanged(t));
		}
		{ // ZBias
			auto& t = this->position_settings->zbias->textboxes;
			material->setUniformValue(0x8D, this->sanitiseTextChanged(t));
		}

	}

	template <typename T>
	inline void readbackUniformToUIBase(T* textbox, float val)
	{
		std::stringstream stream;
		stream << std::fixed << std::setprecision(textbox->getNumDigits()) << val;
		textbox->clear();
		textbox->insert(QString::fromStdString(stream.str()));
	}


	void readbackUniformToUI(const MaterialPtr& material, size_t idx, const std::vector<EditorTextbox*>& textboxes)
	{
		auto& buf = material->local_uniform_buffer;
		for (size_t i = 0; i < textboxes.size(); ++i)
		{
			auto& textbox = textboxes[i];
			this->readbackUniformToUIBase(textbox, buf[idx][i]);
			textbox->setCursorPosition(0);
		}
	}

	void readbackUniformValues(const MaterialPtr& material)
	{
		{ // DiffuseColor
			auto& t = this->diffuse_color_settings->diffuse_color_widget->textboxes;
			this->readbackUniformToUI(material, 0x33, t);
		}
		{ // Bumpiness
			auto& t = this->texture_layer_1->bumpmap_settings->bump_strength->textboxes;
			this->readbackUniformToUI(material, 0x36, t);
		}
		{ // SpecularStrength
			auto& t = this->specular_color_settings->specular_strength->textboxes;
			this->readbackUniformToUI(material, 0x38, t);
		}
		{ // SpecularPower
			auto& t = this->specular_color_settings->specular_power->textboxes;
			this->readbackUniformToUI(material, 0x39, t);
		}
		{ // ReflectionStrength
			auto& t = this->reflection_settings->reflection_strength->textboxes;
			this->readbackUniformToUI(material, 0x3B, t);
		}
		{ // FresnelMin
			auto& t = this->reflection_settings->fresnel_min->textboxes;
			this->readbackUniformToUI(material, 0x3C, t);
		}
		{ // FresnelExp
			auto& t = this->reflection_settings->fresnel_exp->textboxes;
			this->readbackUniformToUI(material, 0x3D, t);
		}
		{ // SurfaceColor
			auto& t = this->illumination_settings->surface_color->textboxes;
			this->readbackUniformToUI(material, 0x3E, t);
		}
		{ // FuzzySpecColor
			auto& t = this->illumination_settings->fuzzy_spec_color->textboxes;
			this->readbackUniformToUI(material, 0x3F, t);
		}
		{ // SubColor
			auto& t = this->illumination_settings->sub_color->textboxes;
			this->readbackUniformToUI(material, 0x40, t);
		}
		{ // RollOff
			auto& t = this->illumination_settings->rolloff->textboxes;
			this->readbackUniformToUI(material, 0x41, t);
		}
		{ // VelvetStrength
			auto& t = this->illumination_settings->velvet_strength->textboxes;
			this->readbackUniformToUI(material, 0x42, t);
		}
		{ // OverlayBumpiness
			auto& t = this->texture_layer_2->bumpmap_settings->bump_strength->textboxes;
			this->readbackUniformToUI(material, 0x46, t);
		}
		{ // OverlayStrength
			auto& t = this->texture_layer_2->overlay_strength->textboxes;
			this->readbackUniformToUI(material, 0x47, t);
		}
		{ // GlassStrength
			auto& t = this->glassmap_settings->glass_strength->textboxes;
			this->readbackUniformToUI(material, 0x4B, t);
		}
		{ // Curvature
			auto& t = this->glassmap_settings->curvature->textboxes;
			this->readbackUniformToUI(material, 0x4C, t);
		}
		{ // UpsideDown
			//auto& t = this->texture_layer_2->bumpmap_settings->bump_strength;
			//this->readbackUniformToUI(material, 0x4D, t);
		}
		{ // ParallaxBiasX
			auto& t = this->texture_layer_1->parallax_settings->bias_xy->textboxes[0];
			this->readbackUniformToUI(material, 0x4F, { t });
		}
		{ // ParallaxBiasY
			auto& t = this->texture_layer_1->parallax_settings->bias_xy->textboxes[1];
			this->readbackUniformToUI(material, 0x50, { t });
		}
		{ // ScrollSpeedSet1
			auto& t = this->uv_settings_1->widget_scrollspeed->textboxes;
			this->readbackUniformToUI(material, 0x55, t);
		}
		{ // ScrollSpeedSet2
			auto& t = this->uv_settings_2->widget_scrollspeed->textboxes;
			this->readbackUniformToUI(material, 0x58, t);
		}
		{ // ScrollSpeedSet3
			auto& t = this->uv_settings_3->widget_scrollspeed->textboxes;
			this->readbackUniformToUI(material, 0x5B, t);
		}
		{ // OffsetSet1
			auto& t = this->uv_settings_1->widget_offset->textboxes;
			this->readbackUniformToUI(material, 0x5E, t);
		}
		{ // OffsetSet2
			auto& t = this->uv_settings_2->widget_offset->textboxes;
			this->readbackUniformToUI(material, 0x61, t);
		}
		{ // OffsetSet3
			auto& t = this->uv_settings_3->widget_offset->textboxes;
			this->readbackUniformToUI(material, 0x74, t);
		}
		{ // RotationSet1
			auto& t = this->uv_settings_1->widget_rotation->textboxes;
			this->readbackUniformToUI(material, 0x78, t);
		}
		{ // RotationSet2
			auto& t = this->uv_settings_2->widget_rotation->textboxes;
			this->readbackUniformToUI(material, 0x7B, t);
		}
		{ // RotationSet3
			auto& t = this->uv_settings_3->widget_rotation->textboxes;
			this->readbackUniformToUI(material, 0x7E, t);
		}
		{ // ScaleSet1
			auto& t = this->uv_settings_1->widget_scale->textboxes;
			this->readbackUniformToUI(material, 0x81, t);
		}
		{ // ScaleSet2
			auto& t = this->uv_settings_2->widget_scale->textboxes;
			this->readbackUniformToUI(material, 0x84, t);
		}
		{ // ScaleSet3
			auto& t = this->uv_settings_3->widget_scale->textboxes;
			this->readbackUniformToUI(material, 0x87, t);
		}
		{ // DistortionStrength
			auto& t = this->texture_layer_1->distortion->textboxes;
			material->setUniformValue(0x64, this->sanitiseTextChanged(t));
		}
		{ // LightMapPower
			auto& t = this->diffuse_color_settings->light_power_textbox->textboxes;
			this->readbackUniformToUI(material, 0x71, t);
		}
		{ // LightMapStrength
			auto& t = this->diffuse_color_settings->light_strength_textbox->textboxes;
			this->readbackUniformToUI(material, 0x72, t);
		}
		{ // Fat
			auto& t = this->position_settings->fat->textboxes;
			this->readbackUniformToUI(material, 0x77, t);
		}
		{ // ZBias
			auto& t = this->position_settings->zbias->textboxes;
			this->readbackUniformToUI(material, 0x8D, t);
		}

	}


	void readbackUISettings()
	{
		auto& selected_material = this->selected_objects.getEditableSelectedMaterialResource().getFactoryMaterial();
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
		this->readbackUniformValues(selected_material);
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
		bool found_weight = false;
		for (const auto& attr : selected_mesh->mesh.vertex_attributes)	
		{
			if (attr.attribute_type == FileFormats::DSCS::GeomFile::VertexAttributeType::WeightedBoneID)
			{
				const auto& num_idxs = attr.num_elements;
				settings.num_vertex_weights = num_idxs;
				settings.use_skeleton = true;
				settings.use_weights = true;
				found_weight = true;
				break;
				
			}
		}
		if (!found_weight)
		{
			for (const auto& attr : selected_mesh->mesh.vertex_attributes)
			{
				if (attr.attribute_type == FileFormats::DSCS::GeomFile::VertexAttributeType::Position)
				{
					const auto& num_idxs = attr.num_elements;
					if (num_idxs == 4)
						settings.num_vertex_weights = 1;
					else
						settings.num_vertex_weights = 0;		
					settings.use_skeleton = true;
					settings.use_weights = false;
					break;
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
		this->assignUniformValues(curr_material);

		if (material_resource.isFactoryActive())
			material_resource.updateMainMaterial(this->animation_buffer);

		this->setting_update_in_progress = false;

		auto& shader_hashes = this->selected_objects.getShaderHashes();
		auto factory_hash = this->hashShader();
		if (shader_hashes.contains(factory_hash))
			this->matching_shader->setText(QString::fromStdString("Matches: " + shader_hashes.at(factory_hash)[0]));
		else
			this->matching_shader->setText("No matching shader.");
	}

	void activateMaterial()
	{
		this->selected_objects.getEditableSelectedMaterialResource().activateFactory();
		this->regenerateMaterial();
	}

	void placeInSpoiler(const QString& title, QWidget* widget, QLayout* layout)
	{
		auto* spoiler = new Spoiler(title, 300, this);
		spoiler->setContentLayout(*widget->layout());
		layout->addWidget(spoiler);
	}

	bool uvSlotInUse(ShaderFactoryTextureSlot*& slot, size_t idx)
	{
		return slot->isActive() && (slot->uv_slot_combobox->currentIndex() == idx);
	}

	bool getUVSlot(size_t idx)
	{
		return this->uvSlotInUse(this->texture_layer_1->diffuse_texture_settings, idx)
			|| this->uvSlotInUse(this->texture_layer_1->normal_texture_settings, idx)
			|| this->uvSlotInUse(this->texture_layer_2->diffuse_texture_settings, idx)
			|| this->uvSlotInUse(this->texture_layer_2->normal_texture_settings, idx)
			|| this->uvSlotInUse(this->diffuse_color_settings->light_sampler, idx);
	}

	void blockUISignals(bool active)
	{
		this->uv_settings_1->blockSignals(active);
		this->uv_settings_2->blockSignals(active);
		this->uv_settings_3->blockSignals(active);
		this->texture_layer_1->blockSignals(active);
		this->texture_layer_2->blockSignals(active);
		this->diffuse_color_settings->blockSignals(active);
		this->specular_color_settings->blockSignals(active);
		this->illumination_settings->blockSignals(active);
		this->reflection_settings->blockSignals(active);
		this->glassmap_settings->blockSignals(active);
		this->position_settings->blockSignals(active);

		this->uv_settings_1->widget_scrollspeed->textboxes[0]->blockSignals(active);
		this->uv_settings_1->widget_scrollspeed->textboxes[1]->blockSignals(active);
		this->uv_settings_1->widget_offset->textboxes[0]->blockSignals(active);
		this->uv_settings_1->widget_offset->textboxes[1]->blockSignals(active);
		this->uv_settings_1->widget_rotation->textboxes[0]->blockSignals(active);
		this->uv_settings_1->widget_scale->textboxes[0]->blockSignals(active);
		this->uv_settings_1->widget_scale->textboxes[1]->blockSignals(active);
		this->uv_settings_2->widget_scrollspeed->textboxes[0]->blockSignals(active);
		this->uv_settings_2->widget_scrollspeed->textboxes[1]->blockSignals(active);
		this->uv_settings_2->widget_offset->textboxes[0]->blockSignals(active);
		this->uv_settings_2->widget_offset->textboxes[1]->blockSignals(active);
		this->uv_settings_2->widget_rotation->textboxes[0]->blockSignals(active);
		this->uv_settings_2->widget_scale->textboxes[0]->blockSignals(active);
		this->uv_settings_2->widget_scale->textboxes[1]->blockSignals(active);
		this->uv_settings_3->widget_scrollspeed->textboxes[0]->blockSignals(active);
		this->uv_settings_3->widget_scrollspeed->textboxes[1]->blockSignals(active);
		this->uv_settings_3->widget_offset->textboxes[0]->blockSignals(active);
		this->uv_settings_3->widget_offset->textboxes[1]->blockSignals(active);
		this->uv_settings_3->widget_rotation->textboxes[0]->blockSignals(active);
		this->uv_settings_3->widget_scale->textboxes[0]->blockSignals(active);
		this->uv_settings_3->widget_scale->textboxes[1]->blockSignals(active);

		this->texture_layer_1->parallax_settings->bias_xy->textboxes[0]->blockSignals(active);
		this->texture_layer_1->parallax_settings->bias_xy->textboxes[1]->blockSignals(active);
		this->texture_layer_1->distortion->textboxes[0]->blockSignals(active);
		this->texture_layer_1->bumpmap_settings->bump_strength->textboxes[0]->blockSignals(active);
		this->texture_layer_2->bumpmap_settings->bump_strength->textboxes[0]->blockSignals(active);
		this->texture_layer_2->overlay_strength->textboxes[0]->blockSignals(active);
		this->diffuse_color_settings->diffuse_color_widget->textboxes[0]->blockSignals(active);
		this->diffuse_color_settings->diffuse_color_widget->textboxes[1]->blockSignals(active);
		this->diffuse_color_settings->diffuse_color_widget->textboxes[2]->blockSignals(active);
		this->diffuse_color_settings->diffuse_color_widget->textboxes[3]->blockSignals(active);
		this->diffuse_color_settings->light_power_textbox->textboxes[0]->blockSignals(active);
		this->diffuse_color_settings->light_strength_textbox->textboxes[0]->blockSignals(active);
		this->specular_color_settings->specular_power->textboxes[0]->blockSignals(active);
		this->specular_color_settings->specular_strength->textboxes[0]->blockSignals(active);
		this->reflection_settings->reflection_strength->textboxes[0]->blockSignals(active);
		this->reflection_settings->fresnel_exp->textboxes[0]->blockSignals(active);
		this->reflection_settings->fresnel_min->textboxes[0]->blockSignals(active);
		this->illumination_settings->surface_color->textboxes[0]->blockSignals(active);
		this->illumination_settings->surface_color->textboxes[1]->blockSignals(active);
		this->illumination_settings->surface_color->textboxes[2]->blockSignals(active);
		this->illumination_settings->fuzzy_spec_color->textboxes[0]->blockSignals(active);
		this->illumination_settings->fuzzy_spec_color->textboxes[1]->blockSignals(active);
		this->illumination_settings->fuzzy_spec_color->textboxes[2]->blockSignals(active);
		this->illumination_settings->sub_color->textboxes[0]->blockSignals(active);
		this->illumination_settings->sub_color->textboxes[1]->blockSignals(active);
		this->illumination_settings->sub_color->textboxes[2]->blockSignals(active);
		this->illumination_settings->rolloff->textboxes[0]->blockSignals(active);
		this->illumination_settings->velvet_strength->textboxes[0]->blockSignals(active);
		this->position_settings->zbias->textboxes[0]->blockSignals(active);
		this->position_settings->fat->textboxes[0]->blockSignals(active);
	}

	void updateUI()
	{
		this->blockUISignals(true);
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

		this->texture_layer_2->diffuse_texture_settings->setEnabled(this->texture_layer_1->diffuse_texture_settings->isActive());

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

		this->specular_color_settings->specular_power->setEnabled(!this->illumination_settings->clut->isActive());

		this->regenerateMaterial();
		this->blockUISignals(false);
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
		connectUIUpdate(this->specular_color_settings);
		connectUIUpdate(this->reflection_settings);
		connectUIUpdate(this->illumination_settings);
		connectUIUpdate(this->glassmap_settings);
		connectUIUpdate(this->position_settings);
	}


	void hookUniformValueUpdate(size_t idx, const std::vector<EditorTextbox*>& textboxes)
	{
		for (auto& textbox : textboxes)
		{
			connect(textbox, &EditorTextbox::textChanged, this, [this, idx, &textbox]()
				{
					auto& material_resource = this->selected_objects.getEditableSelectedMaterialResource();
					auto& material = material_resource.getFactoryMaterial();
					material->setUniformValue(idx, this->sanitiseTextChanged({ textbox }));
					if (material_resource.isFactoryActive())
						material_resource.updateMainMaterialBuffer(idx, this->sanitiseTextChanged({ textbox }));
				}
			);
		}
	}


	void hookUniformUpdates()
	{
		{ // DiffuseColor
			auto& t = this->diffuse_color_settings->diffuse_color_widget->textboxes;
			this->hookUniformValueUpdate(0x33, t);
		}
		{ // Bumpiness
			auto& t = this->texture_layer_1->bumpmap_settings->bump_strength->textboxes;
			this->hookUniformValueUpdate(0x36, t);
		}
		{ // SpecularStrength
			auto& t = this->specular_color_settings->specular_strength->textboxes;
			this->hookUniformValueUpdate(0x38, t);
		}
		{ // SpecularPower
			auto& t = this->specular_color_settings->specular_power->textboxes;
			this->hookUniformValueUpdate(0x39, t);
		}
		{ // ReflectionStrength
			auto& t = this->reflection_settings->reflection_strength->textboxes;
			this->hookUniformValueUpdate(0x3B, t);
		}
		{ // FresnelMin
			auto& t = this->reflection_settings->fresnel_min->textboxes;
			this->hookUniformValueUpdate(0x3C, t);
		}
		{ // FresnelExp
			auto& t = this->reflection_settings->fresnel_exp->textboxes;
			this->hookUniformValueUpdate(0x3D, t);
		}
		{ // SurfaceColor
			auto& t = this->illumination_settings->surface_color->textboxes;
			this->hookUniformValueUpdate(0x3E, t);
		}
		{ // FuzzySpecColor
			auto& t = this->illumination_settings->fuzzy_spec_color->textboxes;
			this->hookUniformValueUpdate(0x3F, t);
		}
		{ // SubColor
			auto& t = this->illumination_settings->sub_color->textboxes;
			this->hookUniformValueUpdate(0x40, t);
		}
		{ // RollOff
			auto& t = this->illumination_settings->rolloff->textboxes;
			this->hookUniformValueUpdate(0x41, t);
		}
		{ // VelvetStrength
			auto& t = this->illumination_settings->velvet_strength->textboxes;
			this->hookUniformValueUpdate(0x42, t);
		}
		{ // OverlayBumpiness
			auto& t = this->texture_layer_2->bumpmap_settings->bump_strength->textboxes;
			this->hookUniformValueUpdate(0x46, t);
		}
		{ // OverlayStrength
			auto& t = this->texture_layer_2->overlay_strength->textboxes;
			this->hookUniformValueUpdate(0x47, t);
		}
		{ // GlassStrength
			auto& t = this->glassmap_settings->glass_strength->textboxes;
			this->hookUniformValueUpdate(0x4B, t);
		}
		{ // Curvature
			auto& t = this->glassmap_settings->curvature->textboxes;
			this->hookUniformValueUpdate(0x4C, t);
		}
		{ // UpsideDown
			//auto& t = this->texture_layer_2->bumpmap_settings->bump_strength;
			//this->hookUniformValueUpdate(0x4D, t);
		}
		{ // ParallaxBiasX
			auto& t = this->texture_layer_1->parallax_settings->bias_xy->textboxes[0];
			this->hookUniformValueUpdate(0x4F, { t });
		}
		{ // ParallaxBiasY
			auto& t = this->texture_layer_1->parallax_settings->bias_xy->textboxes[1];
			this->hookUniformValueUpdate(0x50, { t });
		}
		{ // ScrollSpeedSet1
			auto& t = this->uv_settings_1->widget_scrollspeed->textboxes;
			this->hookUniformValueUpdate(0x55, t);
		}
		{ // ScrollSpeedSet2
			auto& t = this->uv_settings_2->widget_scrollspeed->textboxes;
			this->hookUniformValueUpdate(0x58, t);
		}
		{ // ScrollSpeedSet3
			auto& t = this->uv_settings_3->widget_scrollspeed->textboxes;
			this->hookUniformValueUpdate(0x5B, t);
		}
		{ // OffsetSet1
			auto& t = this->uv_settings_1->widget_offset->textboxes;
			this->hookUniformValueUpdate(0x5E, t);
		}
		{ // OffsetSet2
			auto& t = this->uv_settings_2->widget_offset->textboxes;
			this->hookUniformValueUpdate(0x61, t);
		}
		{ // OffsetSet3
			auto& t = this->uv_settings_3->widget_offset->textboxes;
			this->hookUniformValueUpdate(0x74, t);
		}
		{ // RotationSet1
			auto& t = this->uv_settings_1->widget_rotation->textboxes;
			this->hookUniformValueUpdate(0x78, t);
		}
		{ // RotationSet2
			auto& t = this->uv_settings_2->widget_rotation->textboxes;
			this->hookUniformValueUpdate(0x7B, t);
		}
		{ // RotationSet3
			auto& t = this->uv_settings_3->widget_rotation->textboxes;
			this->hookUniformValueUpdate(0x7E, t);
		}
		{ // ScaleSet1
			auto& t = this->uv_settings_1->widget_scale->textboxes;
			this->hookUniformValueUpdate(0x81, t);
		}
		{ // ScaleSet2
			auto& t = this->uv_settings_2->widget_scale->textboxes;
			this->hookUniformValueUpdate(0x84, t);
		}
		{ // ScaleSet3
			auto& t = this->uv_settings_3->widget_scale->textboxes;
			this->hookUniformValueUpdate(0x87, t);
		}
		{ // DistortionStrength
			auto& t = this->texture_layer_1->distortion->textboxes;
			this->hookUniformValueUpdate(0x64, t);
		}
		{ // LightMapPower
			auto& t = this->diffuse_color_settings->light_power_textbox->textboxes;
			this->hookUniformValueUpdate(0x71, t);
		}
		{ // LightMapStrength
			auto& t = this->diffuse_color_settings->light_strength_textbox->textboxes;
			this->hookUniformValueUpdate(0x72, t);
		}
		{ // Fat
			auto& t = this->position_settings->fat->textboxes;
			this->hookUniformValueUpdate(0x77, t);
		}
		{ // ZBias
			auto& t = this->position_settings->zbias->textboxes;
			this->hookUniformValueUpdate(0x8D, t);
		}

	}

	template<typename func>
	void applyOnTextboxes()
	{
		func(0x33, this->diffuse_color_settings->diffuse_color_widget->textboxes);       // DiffuseColor
		func(0x36, this->texture_layer_1->bumpmap_settings->bump_strength->textboxes);   // Bumpiness
		func(0x38, this->specular_color_settings->specular_strength->textboxes);         // SpecularStrength
		func(0x39, this->specular_color_settings->specular_power->textboxes);            // SpecularPower
		func(0x3B, this->reflection_settings->reflection_strength->textboxes);           // ReflectionStrength
		func(0x3C, this->reflection_settings->fresnel_min->textboxes);                   // FresnelMin
		func(0x3D, this->reflection_settings->fresnel_exp->textboxes);                   // FresnelExp
		func(0x3E, this->illumination_settings->surface_color->textboxes);               // SurfaceColor
		func(0x3F, this->illumination_settings->fuzzy_spec_color->textboxes);            // FuzzySpecColor
		func(0x40, this->illumination_settings->sub_color->textboxes);                   // SubColor
		func(0x41, this->illumination_settings->rolloff->textboxes);                     // RollOff
		func(0x42, this->illumination_settings->velvet_strength->textboxes);             // VelvetStrength
		func(0x46, this->texture_layer_2->bumpmap_settings->bump_strength->textboxes);   // OverlayBumpiness
		func(0x47, this->texture_layer_2->overlay_strength->textboxes);                  // OverlayStrength
		func(0x4B, this->glassmap_settings->glass_strength->textboxes);                  // GlassStrength
		func(0x4C, this->glassmap_settings->curvature->textboxes);                       // Curvature
		//func(0x4D, this->texture_layer_2->bumpmap_settings->bump_strength);              // UpsideDown
		func(0x4F, { this->texture_layer_1->parallax_settings->bias_xy->textboxes[0] }); // ParallaxBiasX
		func(0x50, { this->texture_layer_1->parallax_settings->bias_xy->textboxes[1] }); // ParallaxBiasY
		func(0x55, this->uv_settings_1->widget_scrollspeed->textboxes);                  // ScrollSpeedSet1
		func(0x58, this->uv_settings_2->widget_scrollspeed->textboxes);                  // ScrollSpeedSet2
		func(0x5B, this->uv_settings_3->widget_scrollspeed->textboxes);                  // ScrollSpeedSet3
		func(0x5E, this->uv_settings_1->widget_offset->textboxes);                       // OffsetSet1
		func(0x61, this->uv_settings_2->widget_offset->textboxes);                       // OffsetSet2
		func(0x74, this->uv_settings_3->widget_offset->textboxes);                       // OffsetSet3
		func(0x78, this->uv_settings_1->widget_rotation->textboxes);                     // RotationSet1
		func(0x7B, this->uv_settings_2->widget_rotation->textboxes);                     // RotationSet2
		func(0x7E, this->uv_settings_3->widget_rotation->textboxes);                     // RotationSet3
		func(0x81, this->uv_settings_1->widget_scale->textboxes);                        // ScaleSet1
		func(0x84, this->uv_settings_2->widget_scale->textboxes);                        // ScaleSet2
		func(0x87, this->uv_settings_3->widget_scale->textboxes);                        // ScaleSet3
		func(0x64, this->texture_layer_1->distortion->textboxes);                        // DistortionStrength
		func(0x71, this->diffuse_color_settings->light_power_textbox->textboxes);        // LightMapPower
		func(0x72, this->diffuse_color_settings->light_strength_textbox->textboxes);     // LightMapStrength
		func(0x77, this->position_settings->fat->textboxes);                             // Fat
		func(0x8D, this->position_settings->zbias->textboxes);                           // ZBias
	}

public:
	explicit ShaderFactory(SelectedObjectReferences& sor, TabMaterialsLibrary& tab_materials, ShaderBackend_t& shader_backend, Rendering::DSCS::AnimationBuffer& animation_buffer, QWidget* parent = nullptr)
		: QWidget(parent)
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
			_layout->addWidget(this->matching_shader);
			this->matching_shader->setTextInteractionFlags(Qt::TextSelectableByMouse);
			connect(compile_button, &QPushButton::clicked, this, &ShaderFactory::activateMaterial);
			this->texture_layer_1 = new ShaderFactoryTextureLayer1("Texture Layer 1");
			this->texture_layer_2 = new ShaderFactoryTextureLayer2("Texture Layer 2");

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

		this->hookUniformUpdates();

		connect(&this->selected_objects, &SelectedObjectReferences::selectedMaterialUpdated, this, &ShaderFactory::updateReadbackSettings);
	}


	void updateReadbackSettings()
	{
		this->blockUISignals(true);
		this->updateAvailableTextures(); // This should update in other places
		this->readbackUISettings();

		this->blockUISignals(false);
	}

	std::string hashShader()
	{
		auto& mat = this->selected_objects.getEditableSelectedMaterialResource().getFactoryMaterial();
		auto md5 = QCryptographicHash(QCryptographicHash::Algorithm::Md5);
		auto vp_hash = md5.hash(mat->shader->vertex_source.c_str(), QCryptographicHash::Algorithm::Md5).toHex().toStdString();
		auto fp_hash = md5.hash(mat->shader->fragment_source.c_str(), QCryptographicHash::Algorithm::Md5).toHex().toStdString();
		return vp_hash + fp_hash;
	}

};
