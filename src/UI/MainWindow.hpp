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
#include "../Commands/CommandManager.hpp"
#include "../Commands/ICommand.hpp"

class DSCSModelDataEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    DSCSModelDataEditorWindow() : DSCSModelDataEditorWindow(Q_NULLPTR) {};
    DSCSModelDataEditorWindow(QWidget* parent);
    ~DSCSModelDataEditorWindow() { };

private slots:
    void openLoadModelDialog();

private:
    CustomWidgets::RenderWidget* render_widget;
    void initToolbar();
    void loadModel(const QString& fileName);
    void loadAnim(const QString& fileName);
};


