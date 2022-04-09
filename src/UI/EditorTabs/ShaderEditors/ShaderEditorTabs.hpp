#pragma once

#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTabWidget>

#include <UI/EditorTabs/ShaderEditors/ShaderFactory.hpp>
#include <UI/EditorTabs/ShaderEditors/CodeEditor.hpp>

class ShaderEditorTabs : public QTabWidget
{
    Q_OBJECT
private:
    typedef Rendering::DSCS::DataObjects::OpenGLDSCSMaterial Material;
    typedef std::shared_ptr<Material> MaterialPtr;

    QWidget* prebuilts_tab = new QWidget(this);
    QScrollArea* shader_factory_scroll_area = new QScrollArea(this);
    ShaderFactory* factory_tab = new ShaderFactory(this);
    CodeEditor* code_tab = new CodeEditor(this);
public:
    ShaderEditorTabs(QWidget* parent = Q_NULLPTR) : QTabWidget(parent)
    {
        this->shader_factory_scroll_area->setWidgetResizable(true);
        this->shader_factory_scroll_area->setWidget(this->factory_tab);

        this->addTab(this->prebuilts_tab, "Pre-Built");
        this->addTab(this->shader_factory_scroll_area, "Shader Factory");
        this->addTab(this->code_tab, "Code Editor");

    }
public slots:
    void updateSelectedMaterial(MaterialPtr material_ptr)
    {
        this->code_tab->updateSelectedMaterial(material_ptr);
    }


};
