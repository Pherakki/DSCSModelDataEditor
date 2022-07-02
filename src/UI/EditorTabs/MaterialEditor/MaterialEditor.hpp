#pragma once

#include <unordered_map>

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

#include "../../SelectedObjectReferences.hpp"
#include "../../Spoiler/Spoiler.hpp"
#include "../../../Renderer/src/DSCS/DataObjects/OpenGLDSCSMaterial.hpp"
#include "../../../Renderer/src/DSCS/RenderObjects/Types.hpp"
#include "ShaderEditors/ShaderEditorTabs.hpp"
#include "OpenGLSettingsEditor.hpp"

class MaterialEditorTab : public QWidget
{
    Q_OBJECT
private:
    typedef Rendering::DSCS::DataObjects::OpenGLDSCSModel Model;
    typedef std::shared_ptr<Model> ModelPtr;
    typedef Rendering::DSCS::DataObjects::OpenGLDSCSMesh Mesh;
    typedef std::shared_ptr<Mesh> MeshPtr;
    typedef Rendering::DSCS::DataObjects::OpenGLDSCSMaterial Material;
    typedef std::shared_ptr<Material> MaterialPtr;
    typedef void(QComboBox::* CIC_t)(int);
    typedef std::unique_ptr<Rendering::ShaderBackends::cgGLShaderBackend> ShaderBackend_t;
    typedef Rendering::DSCS::AnimationBuffer AnimBuf_t;


    SelectedObjectReferences& selected_objects;
    // Widgets
    QComboBox* material_dropdown;
    ShaderEditorTabs* shader_edit_modes;
    OpenGLSettingsWidget* opengl_settings;

    // Lookup dicts
    std::unordered_map<MaterialPtr, int> material_lookup;
    std::unordered_map<int, MaterialPtr> material_reverse_lookup;

public:
    MaterialEditorTab(SelectedObjectReferences& sor, ShaderBackend_t& backend, AnimBuf_t& animation_buffer, QWidget* parent = Q_NULLPTR) 
        : QWidget(parent)
        , selected_objects{sor}
    {
        auto layout = new QVBoxLayout(this);
        auto scrollarea = new QScrollArea(this);
        auto widget = new QWidget(this);
        auto w_layout = new QVBoxLayout;
        this->material_dropdown = new QComboBox(widget);
        this->shader_edit_modes = new ShaderEditorTabs(sor, backend, animation_buffer, widget);
        this->opengl_settings = new OpenGLSettingsWidget(sor, widget);

        w_layout->addWidget(this->shader_edit_modes); // use PutInSpoiler once the Spoiler class is fixed
        w_layout->addWidget(this->opengl_settings);   // use PutInSpoiler once the Spoiler class is fixed
        widget->setLayout(w_layout);
        scrollarea->setWidget(widget);

        layout->addWidget(this->material_dropdown);
        layout->addWidget(scrollarea);
        this->setLayout(layout);

        connect(this->material_dropdown, static_cast<CIC_t>(&QComboBox::currentIndexChanged), this, &MaterialEditorTab::selectMaterial);
        connect(this->shader_edit_modes, &ShaderEditorTabs::overwriteCurrentMaterial, this, &MaterialEditorTab::overwriteCurrentMaterial);

        connect(&this->selected_objects, &SelectedObjectReferences::selectedModelUpdated, this, &MaterialEditorTab::updateSelectedModel);
        connect(&this->selected_objects, &SelectedObjectReferences::selectedMaterialUpdated, this, &MaterialEditorTab::updateSelectedMaterial);
    };

    void updateDataList()
    {
        this->material_dropdown->clear();
        this->material_lookup.clear();
        this->material_reverse_lookup.clear();
        auto& selected_model = this->selected_objects.getSelectedModel();
        if (selected_model)
        {
            for (size_t i = 0; i < selected_model->materials.size(); ++i)
            {
                auto& material = selected_model->materials[i];
                this->material_dropdown->addItem(QString::fromStdString(material->name), QVariant::fromValue(i));
                this->material_lookup[material] = i;
                this->material_reverse_lookup[i] = material;
            }
        }
    }

    Spoiler* putInSpoiler(const QString&& name, QWidget* widget)
    {
        auto layout = new QVBoxLayout;
        layout->addWidget(widget);

        auto dropdown = new Spoiler(name, 300, this);
        dropdown->setContentLayout(*layout);

        return dropdown;
    }

signals:
    void overwriteCurrentMaterial(MaterialPtr material_ptr);
public slots:
    void updateSelectedModel()
    {
        this->updateDataList();
    }

    void updateSelectedMaterial()
    {
        try
        {
            auto& material_ptr = this->selected_objects.getSelectedMaterial();
            this->material_dropdown->setCurrentIndex(this->material_lookup.at(material_ptr));
        }
        catch (const std::exception& ex)
        {
            std::cout << "BAD LOOKUP IN SELECTED MATERIAL 2" << std::endl;
        }
    }

    void selectMaterial(int index)
    {
        if (!this->material_reverse_lookup.contains(index))
        {
            std::cout << "Did not contain index " << index << std::endl;
            return;
        }
        auto& material = this->material_reverse_lookup.at(index);
        this->selected_objects.setSelectedMaterial(material);
    }

};
