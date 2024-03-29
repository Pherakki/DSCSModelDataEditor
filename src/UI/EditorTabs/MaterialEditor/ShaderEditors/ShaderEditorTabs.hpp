#pragma once

#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTabWidget>

#include <UI/EditorTabs/MaterialEditor/ShaderEditors/PrebuiltShaders.hpp>
#include <UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderFactory.hpp>
#include <UI/EditorTabs/MaterialEditor/ShaderEditors/CodeEditor.hpp>

#include "UI/SelectedObjectReferences.hpp"
#include "TabMaterials.hpp"

class ShaderEditorTabs : public QTabWidget
{
    Q_OBJECT
private:
    typedef Rendering::DSCS::DataObjects::OpenGLDSCSMesh Mesh;
    typedef std::shared_ptr<Mesh> MeshPtr;
    typedef Rendering::DSCS::DataObjects::OpenGLDSCSMaterial Material;
    typedef std::shared_ptr<Material> MaterialPtr;
    typedef std::unique_ptr<Rendering::ShaderBackends::cgGLShaderBackend> ShaderBackend_t;
    typedef std::unordered_map<std::string, std::shared_ptr<Rendering::DataObjects::OpenGLDSCSTexture>> TextureLibrary_t;
    typedef Rendering::DSCS::AnimationBuffer AnimBuf_t;

    PrebuiltTab* prebuilts_tab;
    //QScrollArea* shader_factory_scroll_area = new QScrollArea(this);
    ShaderFactory* factory_tab;
    CodeEditor* code_tab;

    SelectedObjectReferences& selected_objects;
signals:
    void overwriteCurrentMaterial(MaterialPtr material_ptr);
public:
    ShaderEditorTabs(SelectedObjectReferences& sor, ShaderBackend_t& backend, AnimBuf_t& animation_buffer, QWidget* parent = Q_NULLPTR)
        : QTabWidget(parent)
        , selected_objects(sor)
        , prebuilts_tab{ new PrebuiltTab(sor, animation_buffer, this) }
        , factory_tab{ new ShaderFactory(sor, backend, animation_buffer, this) }
    {
        //this->shader_factory_scroll_area->setWidgetResizable(true);
        //this->shader_factory_scroll_area->setWidget(this->factory_tab);

        this->code_tab = new CodeEditor(this->selected_objects, this);

        this->addTab(this->prebuilts_tab, "Pre-Built");
        this->addTab(this->factory_tab, "Shader Factory");
        this->addTab(this->code_tab, "Code Editor");
    }
};
