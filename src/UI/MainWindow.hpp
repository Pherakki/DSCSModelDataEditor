#pragma once

# include<memory>

#include <QtWidgets/QMainWindow>

#include "../Commands/CommandManager.hpp"
#include "../Commands/ICommand.hpp"
#include "RenderWidget.hpp"
#include "SelectedObjectReferences.hpp"
#include "EditorTabs/AnimationEditor.hpp"
#include "EditorTabs/MeshEditor.hpp"
#include "EditorTabs/MaterialEditor/MaterialEditor.hpp"
#include "EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderFactory.hpp"


class DSCSModelDataEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    DSCSModelDataEditorWindow(QWidget* parent=Q_NULLPTR);
    ~DSCSModelDataEditorWindow() { };
private:
    typedef std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSModel> ModelPtr;
    typedef std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMaterial> MaterialPtr;

    CustomWidgets::RenderWidget* render_widget;
    SelectedObjectReferences selected_objects; // Selected Objects depends on data from render widget, so initialise it after!!

    void initToolbar();
    ModelID_t loadModel(const QString& fileName);
    void loadAnim(ModelID_t model_id, const QString& fileName);
    void loadShaderHashes(const QString& directory);
private slots:
    void testInit();
    void openLoadModelDialog();
};


