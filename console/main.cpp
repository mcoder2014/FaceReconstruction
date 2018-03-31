#include <QCoreApplication>
#include <iostream>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDir>

#include "face/facereconstructiontool.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    // 设置程序相关信息
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName(QObject::tr("Face-Reconstruction"));
    QCoreApplication::setApplicationVersion(QObject::tr("beta 1.0"));

    // 命令行工具
    QCommandLineParser parser;
    parser.setApplicationDescription( QObject::tr("Face Reconstruction"));
    parser.addHelpOption();             // --help
    parser.addVersionOption();          // --Version

    // image file
    QCommandLineOption imagePathOption(
                QStringList() << "i" << "image-path",
                QCoreApplication::translate("main",
                    "The image path to do face reconstruction! <path> e.g. E:/test.jpg" ),
                QCoreApplication::translate("main", "filepath"));
    parser.addOption(imagePathOption);  // 添加选项到程序

    // Save path
    QCommandLineOption savePath(
                QStringList() << "s" << "save-path",
                QCoreApplication::translate("main",
                    "The directory to save the obj model <driectory> e.g. E:/test/"),
                QCoreApplication::translate("main", "directory"));
    parser.addOption(savePath);


    // Process the actual command line arguments given by the user
    parser.process(a);

    QString path_image = parser.value(imagePathOption);
    QString path_save = parser.value(savePath);

    qDebug() << "image path: " << path_image;
    qDebug() << "save path: " << path_save;

    //return a.exec();
    return 0;
}
