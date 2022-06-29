#pragma once

# include<memory>

#include <QApplication>
#include <QDesktopWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>


#include "RenderWidget.hpp"
#include "EditorTabs/AnimationEditor.hpp"
#include "EditorTabs/MeshEditor.hpp"
#include "../Commands/CommandManager.hpp"
#include "../Commands/ICommand.hpp"
#include "EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderFactory.hpp"
#include "EditorTabs/MaterialEditor/MaterialEditor.hpp"
#include "SelectedObjectReferences.hpp"


class DSCSModelDataEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    DSCSModelDataEditorWindow() : DSCSModelDataEditorWindow(Q_NULLPTR) {};
    DSCSModelDataEditorWindow(QWidget* parent);
    ~DSCSModelDataEditorWindow() { };
private:
    typedef std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSModel> ModelPtr;
    typedef std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMaterial> MaterialPtr;

    CustomWidgets::RenderWidget* render_widget;
    SelectedObjectReferences selected_objects; // Selected Objects depends on data from render widget, so initialise it after!!

    void initToolbar();
    DSCSModelDataEditorWindow::ModelPtr loadModel(const QString& fileName);
    void loadAnim(const ModelPtr& model, const QString& fileName);
    void loadShaderHashes(const QString& directory);
private slots:
    void testInit();
    void openLoadModelDialog();
};


