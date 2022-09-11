#include <QtWidgets/qapplication.h>
#include "include/ACGDream.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    ACGDream acgDream;
    return a.exec();
}