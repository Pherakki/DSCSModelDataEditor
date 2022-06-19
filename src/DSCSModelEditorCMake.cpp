// DSCSModelEditorCMake.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <QtWidgets/QApplication>
#include "UI/MainWindow.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    QCoreApplication::addLibraryPath("./Qt5");
    QApplication a(argc, argv);
    DSCSModelDataEditorWindow w;
    //w.showMaximized();
    w.show();
    return a.exec();
}
