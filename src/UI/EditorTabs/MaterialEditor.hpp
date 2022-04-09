#pragma once

#include <unordered_map>

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QWidget>

#include "../Types.hpp"
#include "../../Rendering/DSCS/DataObjects/OpenGLDSCSMaterial.hpp"
#include "ShaderEditors/ShaderEditorTabs.hpp"


class MaterialEditorTab : public QWidget
{
    Q_OBJECT
private:
    typedef Rendering::DSCS::DataObjects::OpenGLDSCSModel Model;
    typedef std::shared_ptr<Model> ModelPtr;
    typedef Rendering::DSCS::DataObjects::OpenGLDSCSMaterial Material;
    typedef std::shared_ptr<Material> MaterialPtr;
    typedef void(QComboBox::* CIC_t)(int);

    ModelPtr selected_model = nullptr;
    MaterialPtr selected_material = nullptr;
    // Widgets
    QComboBox* material_dropdown;
    ShaderEditorTabs* shader_edit_modes;
    // Third widget needs to be the OpenGL options

    // Lookup dicts
    std::unordered_map<MaterialPtr, int> material_lookup;
    std::unordered_map<int, MaterialPtr> material_reverse_lookup;

public:
    MaterialEditorTab(QWidget* parent = Q_NULLPTR) : QWidget(parent)
    {
        auto layout = new QVBoxLayout(this);
        this->material_dropdown = new QComboBox(this);
        this->shader_edit_modes = new ShaderEditorTabs(this);

        layout->addWidget(this->material_dropdown);
        layout->addWidget(this->shader_edit_modes);
        this->setLayout(layout);

        connect(this->material_dropdown, static_cast<CIC_t>(&QComboBox::currentIndexChanged), this, &MaterialEditorTab::selectMaterial);
    };

    void updateDataList()
    {
        this->material_dropdown->clear();
        this->material_lookup.clear();
        this->material_reverse_lookup.clear();
        if (this->selected_model)
        {
            for (size_t i = 0; i < this->selected_model->materials.size(); ++i)
            {
                auto& material = this->selected_model->materials[i];
                this->material_dropdown->addItem(QString::fromStdString(material->name), i);
                this->material_lookup[material] = i;
                this->material_reverse_lookup[i] = material;
            }
        }

    }

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

    void updateSelectedMaterial(MaterialPtr material_ptr)
    {
        this->material_dropdown->setCurrentIndex(this->material_lookup.at(material_ptr));
        this->selected_material = selected_material;
    }

    /*
    Relay methods up to the central signal dispatcher for global variable changes
    */
    void selectMaterial(int index)
    {
        if (!this->material_reverse_lookup.contains(index))
            return;
        auto& material = this->material_reverse_lookup.at(index);
        emit this->materialSelectionUpdated(material);
    }

};