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
#include "../Commands/CommandManager.hpp"
#include "../Commands/ICommand.hpp"
#include "EditorTabs/ShaderEditors/ShaderFactory.hpp"

class DSCSModelDataEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    DSCSModelDataEditorWindow() : DSCSModelDataEditorWindow(Q_NULLPTR) {};
    DSCSModelDataEditorWindow(QWidget* parent);
    ~DSCSModelDataEditorWindow() { };
private slots:
    void openLoadModelDialog();
    void setVertexShaderText(const QString& shader_text);
    void setFragmentShaderText(const QString& shader_text);

private:
    CustomWidgets::RenderWidget* render_widget;
    QPlainTextEdit* vertex_shader_textedit;
    QPlainTextEdit* fragment_shader_textedit;

    void initToolbar();
    void loadModel(const QString& fileName);
    void loadAnim(const QString& fileName);

};


