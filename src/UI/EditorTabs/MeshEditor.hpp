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
    QListWidget* mesh_list;

public:
    MeshEditorTab(QWidget* parent = Q_NULLPTR) : QWidget(parent)
    {
        this->mesh_list = new QListWidget(this);
    };


    void updateMeshList()
    {
        this->mesh_list->clear();
        if (this->selected_model)
        {
            for (size_t i = 0; i < this->selected_model->meshes.size(); ++i)
            {
                auto list_item = new QListWidgetItem(QString::fromStdString("Mesh " + std::to_string(i)));
                this->mesh_list->addItem(list_item);
            }
        }
    }
public slots:
    void updateSelectedModel(ModelPtr model_ptr)
    {
        this->selected_model = model_ptr;
        this->updateMeshList();
    }
};