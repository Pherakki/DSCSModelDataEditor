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
#include "../../../../../Renderer/src/DSCS/DataObjects/OpenGLDSCSModel.hpp"
#include "../../../../../Renderer/src/DSCS/DataObjects/OpenGLDSCSMesh.hpp"
#include "../../../../../Renderer/src/DSCS/DataObjects/OpenGLDSCSMaterial.hpp"
#include "../../../../../Renderer/src/DSCS/ShaderSystem/cgGL/cgGLShaderBackend.hpp"
#include "../../../../../Renderer/src/Utils/Hashing.hpp"
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

// This feels a bit like a god-object.
// It's doing too many jobs at once.
// Need to identify where lines can be drawn between various
// jobs and split this out into multiple objects.
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
	bool setting_update_in_progress = false;

	bool anyLayer1SamplerEnabled()
	{
		return this->texture_layer_1->diffuse_texture_settings->isActive()
			|| this->texture_layer_1->normal_texture_settings->isActive()
			|| this->diffuse_color_settings->light_sampler->isActive();
	}

	// Shader Update Chain
	void blockUISignals(bool active);
	void updateUI();
	bool uvSlotInUse(ShaderFactoryTextureSlot*& slot, size_t idx);
	bool getUVSlot(size_t idx);
	void regenerateMaterial();
	void assignTextureReferences(MaterialPtr& material, TextureRefs& textures);
	void createSettingsFromUI(FactorySettings& settings, TextureRefs& textures);
	void createTexSettings(FactorySettings& settings, Sampler& sampler, std::string& texname, uint32_t& texture, const ShaderFactoryTextureSlot& tex_ui, TextureLibrary::TextureSubLibrary_t& sublib);
	void createTexSettings2D(FactorySettings& settings, Sampler& sampler, std::string& texname, uint32_t& texture, const ShaderFactoryTextureSlot& tex_ui);
	void createTexSettingsLum(FactorySettings& settings, Sampler& sampler, std::string& texname, uint32_t& texture, const ShaderFactoryTextureSlot& tex_ui);
	void createUVSettings(TexSlot& uv_slot, const ShaderFactoryUVSettingsWidget& uv_ui);
	void createParallaxSettings(FactorySettings& settings);
	void createDiffuseColorSettings(FactorySettings& settings);
	void createSpecularColorSettings(FactorySettings& settings);
	void createIlluminationSettings(FactorySettings& settings);
	void createReflectionSettings(FactorySettings& settings);
	void createBumpSettings(FactorySettings& settings);
	void createVertexSettings(FactorySettings& settings);

	// UI Readback Functions
	void updateReadbackSettings();
	void readbackUISettings();
	void createUIFromSettings();

	// Texture Update Functions
	void updateTexturesOn(QComboBox* combobox, TextureLibrary::TextureSubLibrary_t& tex_sublib);
	void update2DTexturesOn(QComboBox* combobox);
	void updateCLUTTexturesOn(QComboBox* combobox);
	void updateLumTexturesOn(QComboBox* combobox);
	void updateCubeTexturesOn(QComboBox* combobox);
	void updateAvailableTextures();

	// UI Functions
	void activateMaterial();
	void placeInSpoiler(const QString& title, QWidget* widget, QLayout* layout);
	template <typename T>
	auto getSettingsUpdated(T* widget);
	template <typename T>
	auto connectUIUpdate(T* widget);
	void connectUI();

	// Utility Functions
	static float safeStof(const QString& value) noexcept;
	static std::array<float, 4> sanitiseTextChanged(const std::vector<EditorTextbox*>& textboxes);
	static float                sanitiseTextChanged(EditorTextbox* textbox);

	// Functions used by applyOnTextboxes
	void assignUniformValues(MaterialPtr& material);
	void readbackUniformValues(const MaterialPtr& material);
	void hookUniformValueUpdate(size_t idx, EditorTextbox* textbox);
	void hookUniformValueUpdate(size_t idx, const std::vector<EditorTextbox*>& textboxes);
	void hookUniformUpdates();

	template<typename T>
	void applyOnTextboxes(T&& func);

public:
	explicit ShaderFactory(SelectedObjectReferences& sor, ShaderBackend_t& shader_backend, Rendering::DSCS::AnimationBuffer& animation_buffer, QWidget* parent = nullptr)
		: QWidget(parent)
		, shader_backend{ shader_backend }
		, animation_buffer{ animation_buffer }
		, selected_objects{ sor }
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

	// Public Utility Functions
	std::string hashShader();
};

template<typename T>
void ShaderFactory::applyOnTextboxes(T&& func)
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
	func(0x4F, this->texture_layer_1->parallax_settings->bias_xy->textboxes[0]);     // ParallaxBiasX
	func(0x50, this->texture_layer_1->parallax_settings->bias_xy->textboxes[1]);     // ParallaxBiasY
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

// ########## //
// UI Updates //
// ########## //

template <typename T>
auto ShaderFactory::getSettingsUpdated(T* widget)
{
	// Gets a function pointer to a function called "settingsUpdated" on the type T
	// Will cause a compile error if no function exists on T, which is what should happen
	return &std::remove_reference<decltype(*widget)>::type::settingsUpdated;
}

template <typename T>
auto ShaderFactory::connectUIUpdate(T* widget)
{
	return connect(widget, getSettingsUpdated(widget), this, &ShaderFactory::updateUI);
}
