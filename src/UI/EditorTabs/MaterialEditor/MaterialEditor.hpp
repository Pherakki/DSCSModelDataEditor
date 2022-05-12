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
#include "../../Types.hpp"
#include "../../Spoiler/Spoiler.hpp"
#include "../../../Rendering/DSCS/DataObjects/OpenGLDSCSMaterial.hpp"
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
    typedef std::unordered_map<std::string, std::shared_ptr<Rendering::DataObjects::OpenGLDSCSTexture>> TextureLibrary_t;
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
    MaterialEditorTab(SelectedObjectReferences& sor, TextureLibrary_t& texlib, ShaderBackend_t& backend, AnimBuf_t& animation_buffer, QWidget* parent = Q_NULLPTR) 
        : QWidget(parent)
        , selected_objects{sor}
    {
        auto layout = new QVBoxLayout(this);
        auto scrollarea = new QScrollArea(this);
        auto widget = new QWidget(this);
        auto w_layout = new QVBoxLayout;
        this->material_dropdown = new QComboBox(widget);
        this->shader_edit_modes = new ShaderEditorTabs(sor, texlib, backend, animation_buffer, widget);
        this->opengl_settings = new OpenGLSettingsWidget(widget);

        w_layout->addWidget(this->shader_edit_modes); // use PutInSpoiler once the Spoiler class is fixed
        w_layout->addWidget(this->opengl_settings);   // use PutInSpoiler once the Spoiler class is fixed
        widget->setLayout(w_layout);
        scrollarea->setWidget(widget);

        layout->addWidget(this->material_dropdown);
        layout->addWidget(scrollarea);
        this->setLayout(layout);

        connect(this->material_dropdown, static_cast<CIC_t>(&QComboBox::currentIndexChanged), this, &MaterialEditorTab::selectMaterial);
        connect(this->shader_edit_modes, &ShaderEditorTabs::overwriteCurrentMaterial, this, &MaterialEditorTab::overwriteCurrentMaterial);
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
                this->material_dropdown->addItem(QString::fromStdString(material->name), i);
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
    void materialSelectionUpdated(MaterialPtr material_ptr);
signals:
    void overwriteCurrentMaterial(MaterialPtr material_ptr);
public slots:
    /*
    Update methods for the data on this widget
    */
    void updateSelectedModel(ModelPtr model_ptr)
    {
        this->selected_objects.setSelectedModel(model_ptr);
        this->updateDataList();
    }

    void updateSelectedMesh(MeshPtr mesh_ptr)
    {
        this->selected_objects.setSelectedMesh(mesh_ptr);
        this->shader_edit_modes->updateSelectedMesh(mesh_ptr);
    }

    void updateSelectedMaterial(MaterialPtr material_ptr)
    {
        try
        {
            this->material_dropdown->setCurrentIndex(this->material_lookup.at(material_ptr));
            this->shader_edit_modes->updateSelectedMaterial(material_ptr);
            this->opengl_settings->updateSelectedMaterial(material_ptr);
        }
        catch (const std::exception& ex)
        {
            std::cout << "BAD LOOKUP IN SELECTED MATERIAL 2" << std::endl;
        }
    }

    /*
    Relay methods up to the central signal dispatcher for global variable changes
    */
    void selectMaterial(int index)
    {
        if (!this->material_reverse_lookup.contains(index))
        {
            std::cout << "Did not contain index " << index << std::endl;
            return;
        }
        auto& material = this->material_reverse_lookup.at(index);
        emit this->materialSelectionUpdated(material);
    }

};