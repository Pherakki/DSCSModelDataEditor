#pragma once

#include <iostream> // Remove after debugging
#include <QObject>

#include "Rendering/DSCS/Renderer.hpp"
#include "Rendering/DSCS/DataObjects/OpenGLDSCSModel.hpp"
#include "UI/EditorTabs/MaterialEditor//ShaderEditors/TabMaterials.hpp"

class MaterialResource
{
private:
	typedef Rendering::DSCS::AnimationBuffer AnimationBuffer;
	typedef Rendering::DSCS::DataObjects::OpenGLDSCSMaterial Material;
	typedef std::shared_ptr<Material> MaterialPtr;
	typedef Rendering::ShaderObjects::cgGLShaderObject Shader;
	typedef std::shared_ptr<Shader> ShaderPtr;

	MaterialPtr main_material;
	MaterialPtr active_material;
	TabMaterials tab_materials;

public:
	MaterialResource(const MaterialPtr& material_ptr, const AnimationBuffer& anim_buffer)
	{
		this->main_material = material_ptr;

		this->tab_materials.prebuilt_material = std::make_shared<Material>(material_ptr->shader, anim_buffer.uniform_dispatch_buffer);
		this->tab_materials.factory_material = std::make_shared<Material>(material_ptr->shader, anim_buffer.uniform_dispatch_buffer);
		this->tab_materials.custom_code_material = std::make_shared<Material>(material_ptr->shader, anim_buffer.uniform_dispatch_buffer);

		this->active_material = this->tab_materials.prebuilt_material;

	}

	const auto& getMaterial()           const noexcept { return this->main_material; }
	const auto& getPrebuiltMaterial()   const noexcept { return this->tab_materials.prebuilt_material; }
	const auto& getFactoryMaterial()    const noexcept { return this->tab_materials.factory_material; }
	const auto& getCustomCodeMaterial() const noexcept { return this->tab_materials.custom_code_material; }
	auto& getEditableMaterial()           noexcept { return this->main_material; } // This needs to go in favour of automatic updates
	auto& getEditablePrebuiltMaterial()   noexcept { return this->tab_materials.prebuilt_material; }
	auto& getEditableFactoryMaterial()    noexcept { return this->tab_materials.factory_material; }
	auto& getEditableCustomCodeMaterial() noexcept { return this->tab_materials.custom_code_material; }

	const bool isPrebuiltActive()   const noexcept { return this->active_material == this->tab_materials.prebuilt_material; }
	const bool isFactoryActive()    const noexcept { return this->active_material == this->tab_materials.factory_material; }
	const bool isCustomCodeActive() const noexcept { return this->active_material == this->tab_materials.custom_code_material; }

	const void activatePrebuilt()   noexcept { this->active_material = this->tab_materials.prebuilt_material; }
	const void activateFactory()    noexcept { this->active_material = this->tab_materials.factory_material; }
	const void activateCustomCode() noexcept { this->active_material = this->tab_materials.custom_code_material; }

	const void updateMainMaterialBuffer(uint8_t idx, const std::array<float, 4>& value)
	{
		for (auto i=0; i < 4; ++i)
			this->main_material->local_uniform_buffer[idx][i] = value[i];
	}

	const void updateMainMaterial(const AnimationBuffer& anim_buffer)
	{
		this->main_material->replaceShader(this->active_material->shader, anim_buffer.uniform_dispatch_buffer);
		this->main_material->local_uniform_buffer = this->active_material->local_uniform_buffer;
		for (auto& [tex_id, tex] : this->active_material->texture_refs)
		{
			this->main_material->setTextureBuffer(tex_id, tex->buffer_id);
		}
	}

	// Need to instead update the shader held on the active material
	//const void setPrebuiltMaterialActive  () noexcept { this->active_material = this->tab_materials.prebuilt_material   ; }
	//const void setFactoryMaterialActive   () noexcept { this->active_material = this->tab_materials.factory_material    ; }
	//const void setCustomCodeMaterialActive() noexcept { this->active_material = this->tab_materials.custom_code_material; }
	//const void setPrebuiltMaterialShader  (const ShaderPtr& shader, const AnimationBuffer& anim_buffer) { this->tab_materials.prebuilt_material   ->replaceShader(shader, anim_buffer.uniform_dispatch_buffer); }
	//const void setFactoryMaterialShader   (const ShaderPtr& shader, const AnimationBuffer& anim_buffer) { this->tab_materials.factory_material    ->replaceShader(shader, anim_buffer.uniform_dispatch_buffer); }
	//const void setCustomCodeMaterialShader(const ShaderPtr& shader, const AnimationBuffer& anim_buffer) { this->tab_materials.custom_code_material->replaceShader(shader, anim_buffer.uniform_dispatch_buffer); }
};

class TextureResource
{
private:
	typedef Rendering::DSCS::DataObjects::ShaderUniforms::AbstractcgGLTextureReference Texture;
	typedef std::shared_ptr<Texture> TexturePtr;
	typedef Rendering::DSCS::DataObjects::ShaderUniforms::Tex2DUniform Texture2D;
	typedef std::shared_ptr<Texture2D> Texture2DPtr;
	typedef Rendering::DSCS::DataObjects::ShaderUniforms::TexCubeUniform TextureCube;
	typedef std::shared_ptr<TextureCube> TextureCubePtr;
	typedef Rendering::DSCS::DataObjects::ShaderUniforms::CLUTTex2DUniform TextureCLUT;
	typedef std::shared_ptr<TextureCLUT> TextureCLUTPtr;

	TexturePtr texture_file;
public:
	TextureResource() {}
	//Texture2DPtr as2D() {}
	//TextureCubePtr asCube() {}
	//TextureCLUTPtr asCLUT() {}
};

class SelectedObjectReferences : public QObject
{
	Q_OBJECT
protected:
	typedef Rendering::DSCS::AnimationBuffer AnimationBuffer;
	typedef Rendering::DSCS::DataObjects::OpenGLDSCSModel Model;
	typedef std::shared_ptr<Model> ModelPtr;
	typedef Rendering::DSCS::DataObjects::OpenGLDSCSMesh Mesh;
	typedef std::shared_ptr<Mesh> MeshPtr;
	typedef Rendering::DSCS::DataObjects::OpenGLDSCSMaterial Material;
	typedef std::shared_ptr<Material> MaterialPtr;

	ModelPtr    selected_model    = nullptr;
	MeshPtr     selected_mesh     = nullptr;
	MaterialPtr selected_material = nullptr;

	AnimationBuffer& anim_buffer;

	//const ModelList_t& model_library;
	std::unordered_map<MaterialPtr, MaterialResource> material_resources;
	std::unordered_map<std::string, TextureResource> texture_resources;
	// Add library for animations

	
public:
	SelectedObjectReferences(AnimationBuffer& anim_buffer)
		: anim_buffer{ anim_buffer }
	{}

	SelectedObjectReferences(
		const ModelPtr& model, 
		const MeshPtr& mesh, 
		const MaterialPtr& material,
		AnimationBuffer& anim_buffer)
		: QObject{}
		, selected_model{model}
		, selected_mesh{mesh}
		, selected_material{material}
		, anim_buffer{anim_buffer}
	{}

	const ModelPtr&    getSelectedModel   () const noexcept { return this->selected_model;    }
	const MeshPtr&     getSelectedMesh    () const noexcept { return this->selected_mesh;     }
	const MaterialPtr& getSelectedMaterial() const noexcept { return this->selected_material; }


	MaterialPtr& getEditableSelectedMaterial() noexcept { return this->selected_material; }
	MaterialResource& getEditableSelectedMaterialResource()
	{
		return this->material_resources.at(this->selected_material);
	}

	void registerNewModel(const ModelPtr& model)
	{
		for (const auto& mat : model->materials)
			this->registerNewMaterial(mat);
	}

	void registerNewMaterial(const MaterialPtr& material)
	{
		this->material_resources.insert({ material, MaterialResource{material, this->anim_buffer} });
	}

	void setSelectedModel(const ModelPtr& model)
	{
		this->selected_model = model;    
		std::cout << "Selected Model Updated" << std::endl; 
		emit this->selectedModelUpdated();
	}

	void setSelectedMesh(const MeshPtr& mesh)
	{
		this->selected_mesh = mesh;
		std::cout << "Selected Mesh Updated" << std::endl;
		emit this->selectedMeshUpdated();
	}

	void setSelectedMaterial(const MaterialPtr& material)
	{
		this->selected_material = material;
		std::cout << "Selected Material Updated" << std::endl;
		//this->selected_tab_materials = &this->tab_materials_library.at(material);
		emit this->selectedMaterialUpdated();
	}


signals:
	void selectedModelUpdated();
signals:
	void selectedMeshUpdated();
signals:
	void selectedMaterialUpdated();
};
