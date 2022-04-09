#pragma once

#include <unordered_map>

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

    ModelPtr selected_model = nullptr;
    MeshPtr  selected_mesh = nullptr;
    QListWidget* mesh_list;
    QListWidget* material_list;
    std::unordered_map<MeshPtr, QListWidgetItem*> mesh_lookup;
    std::unordered_map<QListWidgetItem*, MeshPtr> mesh_reverse_lookup;
    std::unordered_map<MaterialPtr, QListWidgetItem*> material_lookup;
    std::unordered_map<QListWidgetItem* , MaterialPtr> material_reverse_lookup;

public:
    MeshEditorTab(QWidget* parent = Q_NULLPTR) : QWidget(parent)
    {
        auto layout = new QHBoxLayout(this);
        this->mesh_list = new QListWidget(this);
        this->material_list = new QListWidget(this);
        layout->addWidget(this->mesh_list);
        layout->addWidget(this->material_list);
        this->setLayout(layout);

        connect(this->mesh_list, &QListWidget::currentItemChanged, this, &MeshEditorTab::selectMesh);
        connect(this->material_list, &QListWidget::currentItemChanged, this, &MeshEditorTab::selectMaterial);
    };

    void updateDataList()
    {
        this->mesh_list->clear();
        this->material_list->clear();
        this->mesh_lookup.clear();
        this->material_lookup.clear();
        this->material_reverse_lookup.clear();
        if (this->selected_model)
        {
            for (size_t i = 0; i < this->selected_model->meshes.size(); ++i)
            {
                auto& mesh = this->selected_model->meshes[i];
                auto list_item = new QListWidgetItem(QString::fromStdString("Mesh " + std::to_string(i)));
                this->mesh_list->addItem(list_item);
                
                this->mesh_lookup[mesh] = list_item;
                this->mesh_reverse_lookup[list_item] = mesh;
            }
            for (size_t i = 0; i < this->selected_model->materials.size(); ++i)
            {
                auto& material = this->selected_model->materials[i];
                auto list_item = new QListWidgetItem(QString::fromStdString(material->name));
                this->material_list->addItem(list_item);

                this->material_lookup[material] = list_item;
                this->material_reverse_lookup[list_item] = material;
            }
        }

    }

signals:
    void meshSelectionUpdated(MeshPtr mesh_ptr);
signals:
    void materialSelectionUpdated(MaterialPtr material_ptr);
public slots:
    /*
    Update methods for the data on this widget
    */
    void updateSelectedModel(ModelPtr model_ptr)
    {
        this->selected_model = model_ptr;
        this->updateDataList();
    }

    void updateSelectedMesh(MeshPtr mesh_ptr)
    {
        this->mesh_list->setCurrentItem(this->mesh_lookup.at(mesh_ptr));
        this->selected_mesh = mesh_ptr;
    }

    void updateSelectedMaterial(MaterialPtr material_ptr)
    {
        this->material_list->setCurrentItem(this->material_lookup.at(material_ptr));
    }

    /*
    Relay methods up to the central signal dispatcher for global variable changes
    */
    void selectMesh(QListWidgetItem* item, QListWidgetItem* prev)
    {
        emit this->meshSelectionUpdated(this->mesh_reverse_lookup.at(item));
    }

    void selectMaterial(QListWidgetItem* item, QListWidgetItem* prev)
    {
        auto& material = this->material_reverse_lookup.at(item);
        this->selected_mesh->material = material;
        emit this->materialSelectionUpdated(material);
    }

};