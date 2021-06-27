#include "DSCSModelDataEditorWindow.hpp"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DSCSModelDataEditorWindow w;
    w.show();
    return a.exec();
}
