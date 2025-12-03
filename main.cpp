#include "mainwindow.h"

#include <QApplication>

#include <QStyleFactory>
#include <QTranslator>

#include <QSettings>

#include "Metadata.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(QStyleFactory::keys().contains("Fusion"))
        a.setStyle(QStyleFactory::create("Fusion"));

    QSettings s(Metadata::authorName(), Metadata::applicationName());

    QTranslator t;
    if(t.load(s.value("language", 0).toInt() == 1 ? ":/de_DE.qm" : ":/en_US.qm"))
        a.installTranslator(&t);

    QTranslator tQt;
    if(t.load(s.value("language", 0).toInt() == 1 ? ":/qtbase_de.qm" : ":/qtbase_en.qm"))
        a.installTranslator(&tQt);

    MainWindow w;
    w.show();
    return a.exec();
}
