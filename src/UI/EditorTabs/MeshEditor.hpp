#pragma once

#include <unordered_map>

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>

#include "../SelectedObjectReferences.hpp"
#include "../../Renderer/src/DSCS/DataObjects/OpenGLDSCSMesh.hpp"
#include "../../Renderer/src/DSCS/RenderObjects/Types.hpp"

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

    SelectedObjectReferences& selected_objects;
    QListWidget* mesh_list;
    QListWidget* material_list;
    std::unordered_map<MeshPtr, QListWidgetItem*> mesh_lookup;
    std::unordered_map<QListWidgetItem*, MeshPtr> mesh_reverse_lookup;
    std::unordered_map<MaterialPtr, QListWidgetItem*> material_lookup;
    std::unordered_map<QListWidgetItem* , MaterialPtr> material_reverse_lookup;
    QMetaObject::Connection mesh_link;
    QMetaObject::Connection material_link;


public:
    MeshEditorTab(SelectedObjectReferences& sor, QWidget* parent = Q_NULLPTR) 
        : QWidget(parent)
        , selected_objects{ sor }
    {
        auto layout = new QHBoxLayout(this);
        this->mesh_list = new QListWidget(this);
        this->material_list = new QListWidget(this);
        layout->addWidget(this->mesh_list);
        layout->addWidget(this->material_list);
        this->setLayout(layout);

        this->reconnect_mesh();
        this->reconnect_material();

        connect(&this->selected_objects, &SelectedObjectReferences::selectedModelUpdated, this, &MeshEditorTab::updateDataList);
        connect(&this->selected_objects, &SelectedObjectReferences::selectedMeshUpdated, this, &MeshEditorTab::updateSelectedMesh);
    };

    void reconnect_mesh()
    {
        this->mesh_link = connect(this->mesh_list, &QListWidget::currentItemChanged, this, &MeshEditorTab::selectMesh);
    }
    
    void reconnect_material()
    {
        this->material_link = connect(this->material_list, &QListWidget::currentItemChanged, this, &MeshEditorTab::reassignMeshMaterial);
    }

    void updateDataList()
    {
        disconnect(this->mesh_link);
        disconnect(this->material_link);
        this->mesh_list->clear();
        this->material_list->clear();
        this->mesh_lookup.clear();
        this->material_lookup.clear();
        this->material_reverse_lookup.clear();
        this->reconnect_mesh();
        this->reconnect_material();

        auto& selected_model = this->selected_objects.getSelectedModel();
        if (selected_model)
        {
            std::vector<QListWidgetItem*> meshlist_items;
            size_t mesh_sz = selected_model->meshes.size();
            meshlist_items.reserve(mesh_sz);
            for (size_t i = 0; i < mesh_sz; ++i)
            {
                auto& mesh = selected_model->meshes[i];
                auto mesh_name = QString::fromStdString("Mesh " + std::to_string(i));
                auto list_item = new QListWidgetItem(mesh_name);
                
                this->mesh_lookup[mesh] = list_item;
                this->mesh_reverse_lookup[list_item] = mesh;
                meshlist_items.push_back(list_item);
            }
            for (auto& item : meshlist_items)
            {
                this->mesh_list->addItem(item);
            }

            std::vector<QListWidgetItem*> materiallist_items;
            size_t mat_sz = selected_model->materials.size();
            materiallist_items.reserve(mat_sz);
            for (size_t i = 0; i < mat_sz; ++i)
            {
                auto& material = selected_model->materials[i];
                auto list_item = new QListWidgetItem(QString::fromStdString(material->name));

                this->material_lookup[material] = list_item;
                this->material_reverse_lookup[list_item] = material;
                materiallist_items.push_back(list_item);
            }
            for (auto& item : materiallist_items)
            {
                this->material_list->addItem(item);
            }
        }

    }

signals:
    void meshSelectionUpdated(MeshPtr mesh_ptr);
signals:
    void materialSelectionUpdated(MaterialPtr material_ptr);
public slots:


    void updateHighlightedMaterial()
    {
        auto& material_ptr = this->selected_objects.getSelectedMesh()->material;
        try
        {
            this->material_list->setCurrentItem(this->material_lookup.at(material_ptr));
        }
        catch (const std::exception& ex)
        {
            std::cout << "UNABLE TO HIGHLIGHT: " << material_ptr << " " << material_ptr->name << ", DOES NOT EXIST!" << std::endl;
            for (const auto& [key, value] : this->material_lookup)
            {
                std::cout << key << " " << value << " " << key->name << std::endl;
            }
        }
    }

    void updateSelectedMesh()
    {
        this->mesh_list->setCurrentItem(this->mesh_lookup.at(this->selected_objects.getSelectedMesh()));
        this->updateHighlightedMaterial();
    }

    /*
    Relay methods up to the central signal dispatcher for global variable changes
    */
    void selectMesh(QListWidgetItem* item, QListWidgetItem* prev)
    {
        if (!this->mesh_reverse_lookup.contains(item))
        {
            std::cout << "MISSING MESH ITEM!" << std::endl;
            std::cout << item << std::endl;
            for (const auto& [key, val] : this->mesh_reverse_lookup)
            {
                std::cout << val->material->name << std::endl;
            }
        }
        else
        {
            auto& selected_mesh = this->mesh_reverse_lookup.at(item);
            this->selected_objects.setSelectedMesh(selected_mesh);
        }
    }

    void reassignMeshMaterial(QListWidgetItem* item, QListWidgetItem* prev)
    {
        try
        {
            auto& material = this->material_reverse_lookup.at(item);
            this->selected_objects.getSelectedMesh()->material = material;
            this->updateHighlightedMaterial();
        }
        catch (const std::exception& ex)
        {
            std::cout << "ATTEMPTED TO ASSIGN MATERIAL TO MESH, MATERIAL DOES NOT EXIST." << std::endl;
        }
    }

};