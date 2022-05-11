#pragma once

# include<memory>

#include <QApplication>
#include <QDesktopWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>


#include "RenderWidget.hpp"
#include "EditorTabs/AnimationEditor.hpp"
#include "EditorTabs/MeshEditor.hpp"
#include "../Commands/CommandManager.hpp"
#include "../Commands/ICommand.hpp"
#include "EditorTabs/ShaderEditors/ShaderFactory/ShaderFactory.hpp"
#include "EditorTabs/MaterialEditor.hpp"
#include "SelectedObjectReferences.hpp"


class DSCSModelDataEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    DSCSModelDataEditorWindow() : DSCSModelDataEditorWindow(Q_NULLPTR) {};
    DSCSModelDataEditorWindow(QWidget* parent);
    ~DSCSModelDataEditorWindow() { };
private:
    typedef std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMaterial> MaterialPtr;
    SelectedObjectReferences selected_objects;

    CustomWidgets::RenderWidget* render_widget;
    QPlainTextEdit* vertex_shader_textedit;
    QPlainTextEdit* fragment_shader_textedit;

    void initToolbar();
    void loadModel(const QString& fileName);
    void loadAnim(const QString& fileName);
signals:
    void selectedModelUpdated(std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSModel> model_ptr);
    void selectedMeshUpdated(std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMesh> mesh_ptr);
    void selectedMaterialUpdated(std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMaterial> material_ptr);
private slots:
    void testInit();
    void openLoadModelDialog();
    void setSelectedModel(std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSModel> model_ptr);
    void setSelectedModelOnly(std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSModel> model_ptr);
    void setSelectedMesh(std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMesh> mesh);
    void setSelectedMeshOnly(std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMesh> mesh);
    void setSelectedMaterial(MaterialPtr material);
    void overwriteMaterial(MaterialPtr material);
};


