#pragma once

#include "../Rendering/DSCS/DataObjects/OpenGLDSCSModel.hpp"

class SelectedObjectReferences
{
protected:
	typedef Rendering::DSCS::DataObjects::OpenGLDSCSModel Model;
	typedef std::shared_ptr<Model> ModelPtr;
	typedef Rendering::DSCS::DataObjects::OpenGLDSCSMesh Mesh;
	typedef std::shared_ptr<Mesh> MeshPtr;
	typedef Rendering::DSCS::DataObjects::OpenGLDSCSMaterial Material;
	typedef std::shared_ptr<Material> MaterialPtr;

	ModelPtr& selected_model;
	MeshPtr& selected_mesh;
	MaterialPtr& selected_material;
public:
	SelectedObjectReferences(const ModelPtr& model, const MeshPtr& mesh, const MaterialPtr& material)
		: selected_model{model}
		, selected_mesh{mesh}
		, selected_material{material}
	{}

	const ModelPtr&    getSelectedModel()    const noexcept { return this->selected_model; }
	const MeshPtr&     getSelectedMesh()     const noexcept { return this->selected_mesh; }
	const MaterialPtr& getSelectedMaterial() const noexcept { return this->selected_material; }
};
