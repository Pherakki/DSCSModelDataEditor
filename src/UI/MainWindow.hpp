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
#include "EditorTabs/ShaderEditors/ShaderFactory.hpp"
#include "EditorTabs/MaterialEditor.hpp"


class DSCSModelDataEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    DSCSModelDataEditorWindow() : DSCSModelDataEditorWindow(Q_NULLPTR) {};
    DSCSModelDataEditorWindow(QWidget* parent);
    ~DSCSModelDataEditorWindow() { };
private slots:
    void testInit();
    void openLoadModelDialog();
    void setSelectedModel(std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSModel> model_ptr);
    void setSelectedMesh(std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMesh> mesh);
    void setSelectedMaterial(std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMaterial> material);

private:
    std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSModel> selected_model = nullptr;
    std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMesh> selected_mesh = nullptr;
    std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMaterial> selected_material = nullptr;

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
};


