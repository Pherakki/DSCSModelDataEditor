#pragma once
#include <QtWidgets/QMainWindow>

class AbstractDSCSModelDataEditorWindow : public QMainWindow
{
	Q_OBJECT
public:
	AbstractDSCSModelDataEditorWindow(QWidget* parent = Q_NULLPTR) : QMainWindow(parent) {};
	~AbstractDSCSModelDataEditorWindow() {};
};