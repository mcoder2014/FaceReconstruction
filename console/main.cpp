#include <QCoreApplication>
#include <iostream>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QObject>
#include <QDebug>

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName(QObject::tr("Face-Reconstruction"));
    QCoreApplication::setApplicationVersion(QObject::tr("beta 1.0"));

    // 命令行工具
    QCommandLineParser parser;
    parser.setApplicationDescription( QObject::tr("Face Reconstruction"));
    parser.addHelpOption();             // help
    parser.addVersionOption();          // Version
    parser.addPositionalArgument(
                "source",
                QCoreApplication::translate("main", "Source file to copy."));
    parser.addPositionalArgument(
                "destination",
                QCoreApplication::translate("main", "Destination directory"));

    // A boolean option with a single name (-p)
    QCommandLineOption showProgressOption(
                "p",
                QCoreApplication::translate("main", "Show progress during copy"));
    parser.addOption(showProgressOption);

    // A boolean option with multiple names (-f, --force)
    QCommandLineOption forceOption(
                QStringList() << "f" << "force",
            QCoreApplication::translate("main", "Overwrite existing files."));
    parser.addOption(forceOption);

    // An option with a value
    QCommandLineOption targetDirectoryOption(
                QStringList() << "t" << "target-directory",
            QCoreApplication::translate("main", "Copy all source files into <directory>."),
            QCoreApplication::translate("main", "directory"));
    parser.addOption(targetDirectoryOption);

    // Process the actual command line arguments given by the user
    parser.process(a);

    const QStringList args = parser.positionalArguments();
    // source is args.at(0), destination is args.at(1)

    bool showProgress = parser.isSet(showProgressOption);
    bool force = parser.isSet(forceOption);
    QString targetDir = parser.value(targetDirectoryOption);

    cout << "param nums:" << argc << endl;
    for(int i = 0; i < argc; i++)
    {
        cout <<"i = " << i
            << "\t content: "<< argv[i] << endl;
    }

    qDebug() << "showProgress: "<< showProgress;
    qDebug() << "force: " << force;
    qDebug() << "targert dir: "<< targetDir;

    return a.exec();
}
