#pragma once

#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTabWidget>

#include <UI/EditorTabs/ShaderEditors/PrebuiltShaders.hpp>
#include <UI/EditorTabs/ShaderEditors/ShaderFactory.hpp>
#include <UI/EditorTabs/ShaderEditors/CodeEditor.hpp>

class ShaderEditorTabs : public QTabWidget
{
    Q_OBJECT
private:
    typedef Rendering::DSCS::DataObjects::OpenGLDSCSMaterial Material;
    typedef std::shared_ptr<Material> MaterialPtr;
    typedef std::unordered_map<std::string, std::shared_ptr<Rendering::DataObjects::OpenGLDSCSTexture>> TextureLibrary_t;

    PrebuiltTab* prebuilts_tab = new PrebuiltTab(this);
    //QScrollArea* shader_factory_scroll_area = new QScrollArea(this);
    ShaderFactory* factory_tab;
    CodeEditor* code_tab = new CodeEditor(this);
public:
    ShaderEditorTabs(TextureLibrary_t& texlib, QWidget* parent = Q_NULLPTR) : QTabWidget(parent), factory_tab{ new ShaderFactory(texlib, this) }
    {
        //this->shader_factory_scroll_area->setWidgetResizable(true);
        //this->shader_factory_scroll_area->setWidget(this->factory_tab);

        this->addTab(this->prebuilts_tab, "Pre-Built");
        this->addTab(this->factory_tab, "Shader Factory");
        this->addTab(this->code_tab, "Code Editor");
    }
public slots:
    void updateSelectedMaterial(MaterialPtr material_ptr)
    {
        this->prebuilts_tab->updateSelectedMaterial(material_ptr);
        this->code_tab->updateSelectedMaterial(material_ptr);
    }


};
