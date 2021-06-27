#pragma once

//#include <QtWidgets/QMainWindow>
#include <AbstractDSCSModelDataEditorWindow.hpp>
#include "ui_DSCSModelDataEditorWindow.h"

class DSCSModelDataEditorWindow : public AbstractDSCSModelDataEditorWindow
{
public:
    DSCSModelDataEditorWindow(QWidget* parent = Q_NULLPTR) : AbstractDSCSModelDataEditorWindow(parent) { ui.setupUi(this); };
    ~DSCSModelDataEditorWindow() {};

private:
    Ui::DSCSModelDataEditorClass ui;
};
