#include "GUI/MainWindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName(QObject::tr("Face Reconstruction GUI"));
    QApplication::setApplicationVersion( QObject::tr("beta-1.0") );
    QApplication::setApplicationDisplayName( QObject::tr("Face Reconstruction") );

    // 加载中文语言包
    QTranslator translator;
    translator.load("cn_start.qm");
    a.installTranslator(&translator);

    QTranslator translatorQt;
    translatorQt.load("translations/qt_zh_CN.qm");
    a.installTranslator(&translatorQt);

    // 显示窗口主界面
    MainWindow w;
    w.show();

    return a.exec();
}
