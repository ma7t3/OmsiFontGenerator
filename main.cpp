#include "mainwindow.h"

#include <QApplication>

#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(QStyleFactory::keys().contains("Fusion"))
        a.setStyle(QStyleFactory::create("Fusion"));

    MainWindow w;
    w.show();
    return a.exec();
}
