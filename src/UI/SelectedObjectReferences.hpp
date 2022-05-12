#pragma once

#include <iostream> // Remove after debugging
#include <QObject>

#include "../Rendering/DSCS/DataObjects/OpenGLDSCSModel.hpp"

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
public:
	SelectedObjectReferences() {}
	SelectedObjectReferences(const ModelPtr& model, const MeshPtr& mesh, const MaterialPtr& material)
		: selected_model{model}
		, selected_mesh{mesh}
		, selected_material{material}
	{}

	const ModelPtr&    getSelectedModel   () const noexcept { return this->selected_model;    }
	const MeshPtr&     getSelectedMesh    () const noexcept { return this->selected_mesh;     }
	const MaterialPtr& getSelectedMaterial() const noexcept { return this->selected_material; }

	MaterialPtr& getEditableSelectedMaterial() noexcept { return this->selected_material; }

	void setSelectedModel   (const ModelPtr&    model   ) { this->selected_model    = model;    std::cout << "Selected Model Updated"    << std::endl; emit this->selectedModelUpdated(); }
	void setSelectedMesh    (const MeshPtr&     mesh    ) { this->selected_mesh     = mesh;     std::cout << "Selected Mesh Updated"     << std::endl; emit this->selectedMeshUpdated();     }
	void setSelectedMaterial(const MaterialPtr& material) { this->selected_material = material; std::cout << "Selected Material Updated" << std::endl; emit this->selectedMaterialUpdated(); }


signals:
	void selectedModelUpdated();
signals:
	void selectedMeshUpdated();
signals:
	void selectedMaterialUpdated();
};
