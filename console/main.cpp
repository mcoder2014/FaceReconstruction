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

///
/// \brief The main_code enum
/// 简单定义了一个返回值的枚举类型
enum main_code
{
    success = 0,
    image_not_exist = 1,
    face_not_detected = 2
};

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

    // 定义下用来返回的错误代码 enum 之类的
    // 检查路径上的文件是否存在
    // 开始人脸识别 识别失败返回错误代码
    // 人脸重建 重建失败返回错误代码

    QFile imageFile(path_image);
    if(!imageFile.exists())
    {
        // 如果图片不存在
        qDebug() << "The image path is wrong, not found!";
        return main_code::image_not_exist;
    }

    // 初始化人脸重建工具
    FaceReconstructionTool faceReconstructionTool;
    faceReconstructionTool.Reconstruction(path_image, path_save);   // 重建并保存人脸图片

    return main_code::success;
}
