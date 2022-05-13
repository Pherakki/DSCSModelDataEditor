#pragma once

#include <iostream> // Remove after debugging
#include <QObject>

#include "../Rendering/DSCS/DataObjects/OpenGLDSCSModel.hpp"
#include "UI/EditorTabs/MaterialEditor//ShaderEditors/TabMaterials.hpp"


class MaterialResource
{
private:
	typedef Rendering::DSCS::DataObjects::OpenGLDSCSMaterial Material;
	typedef std::shared_ptr<Material> MaterialPtr;

	MaterialPtr active_material;
	TabMaterials tab_materials;

public:
	MaterialResource(const MaterialPtr& material_ptr)
	{
		this->active_material = material_ptr;
		this->tab_materials.prebuilt_material = nullptr; // Need a material clone function...
		this->tab_materials.factory_material = nullptr; // Set a default when you can
		this->tab_materials.custom_code_material = nullptr; // Set a default when you can
	}

	const auto getMaterial() const noexcept { return this->active_material; }
	const auto getPrebuiltMaterial() const noexcept { return this->tab_materials.prebuilt_material; }
	const auto getFactoryMaterial() const noexcept { return this->tab_materials.factory_material; }
	const auto getCustomCodeMaterial() const noexcept { return this->tab_materials.custom_code_material; }
};

class SelectedObjectReferences : public QObject
{
	Q_OBJECT
protected:
	typedef Rendering::DSCS::DataObjects::OpenGLDSCSModel Model;
	typedef std::shared_ptr<Model> ModelPtr;
	typedef Rendering::DSCS::DataObjects::OpenGLDSCSMesh Mesh;
	typedef std::shared_ptr<Mesh> MeshPtr;
	typedef Rendering::DSCS::DataObjects::OpenGLDSCSMaterial Material;
	typedef std::shared_ptr<Material> MaterialPtr;

	ModelPtr    selected_model    = nullptr;
	MeshPtr     selected_mesh     = nullptr;
	MaterialPtr selected_material = nullptr;

	//const ModelList_t& model_library;
	std::unordered_map<std::string, MaterialResource> material_resources;

	
public:
	SelectedObjectReferences() = default;
	SelectedObjectReferences(
		const ModelPtr& model, 
		const MeshPtr& mesh, 
		const MaterialPtr& material)
		: QObject{}
		, selected_model{model}
		, selected_mesh{mesh}
		, selected_material{material}
	{}

	const ModelPtr&    getSelectedModel   () const noexcept { return this->selected_model;    }
	const MeshPtr&     getSelectedMesh    () const noexcept { return this->selected_mesh;     }
	const MaterialPtr& getSelectedMaterial() const noexcept { return this->selected_material; }

	MaterialPtr& getEditableSelectedMaterial() noexcept { return this->selected_material; }

	void registerNewModel(const ModelPtr& model)
	{
		for (const auto& mat : model->materials)
		{
			this->material_resources.insert({ mat->name, MaterialResource{mat} });
		}
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
