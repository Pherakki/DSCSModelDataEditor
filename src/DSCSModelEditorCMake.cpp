// DSCSModelEditorCMake.cpp : Defines the entry point for the application.
//
#include <iostream>

#include <QtWidgets/QApplication>
#include "serialisation/ReadWriter.hpp"
#include "UI/MainWindow.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    DSCSModelDataEditorWindow w;
    //w.showMaximized();
    w.show();
    return a.exec();
}
