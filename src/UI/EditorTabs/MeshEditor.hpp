#pragma once

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>

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
    QListWidget* material_list;

public:
    MeshEditorTab(QWidget* parent = Q_NULLPTR) : QWidget(parent)
    {
        auto layout = new QHBoxLayout(this);
        this->mesh_list = new QListWidget(this);
        this->material_list = new QListWidget(this);
        layout->addWidget(this->mesh_list);
        layout->addWidget(this->material_list);
        this->setLayout(layout);
    };

    void updateDataList()
    {
        this->mesh_list->clear();
        this->material_list->clear();
        if (this->selected_model)
        {
            for (size_t i = 0; i < this->selected_model->meshes.size(); ++i)
            {
                auto list_item = new QListWidgetItem(QString::fromStdString("Mesh " + std::to_string(i)));
                this->mesh_list->addItem(list_item);
            }
            for (size_t i = 0; i < this->selected_model->materials.size(); ++i)
            {
                auto list_item = new QListWidgetItem(QString::fromStdString(this->selected_model->materials[i]->name));
                this->material_list->addItem(list_item);
            }
        }
    }

public slots:
    void updateSelectedModel(ModelPtr model_ptr)
    {
        this->selected_model = model_ptr;
        this->updateDataList();
    }
};