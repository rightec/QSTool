#include "quantaloader.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QuantaLoader w;
    w.show();

    return a.exec();
}
