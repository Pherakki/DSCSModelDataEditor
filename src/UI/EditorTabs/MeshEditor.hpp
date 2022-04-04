#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QListWidget>

#include "../Types.hpp"
#include "../../Rendering/DSCS/DataObjects/OpenGLDSCSMesh.hpp"

class MeshEditorTab : public QWidget
{
    Q_OBJECT
private:
    typedef Rendering::DSCS::DataObjects::OpenGLDSCSModel Model;
    typedef std::shared_ptr<Model> ModelPtr;
    typedef Rendering::DSCS::DataObjects::OpenGLDSCSMesh Mesh;
    typedef std::shared_ptr<Mesh> MeshPtr;
    typedef Rendering::DSCS::DataObjects::OpenGLDSCSMaterial Material;
    typedef std::shared_ptr<Material> MaterialPtr;

    ModelPtr    selected_model = nullptr;

public:
    MeshEditorTab(QWidget* parent = Q_NULLPTR) : QWidget(parent)
    {
        auto mesh_list = new QListWidget(this);
    };
};