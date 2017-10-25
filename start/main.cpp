#include "GUI/MainWindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load("cn_start.qm");
    a.installTranslator(&translator);

    QTranslator translatorQt;
    translatorQt.load("translations/qt_zh_CN.qm");
    a.installTranslator(&translatorQt);

    MainWindow w;
    w.show();



    return a.exec();
}
