#include <QCoreApplication>
#include <QProcess>
#include <QString>
#include <QTextCodec>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //!!! Qt5
    QString program = "./win32-app.exe";
    QStringList arguments;
    //arguments << "/c" << "dir" << "C:\\";
    QProcess *cmdProcess = new QProcess;
    QObject::connect(cmdProcess, &QProcess::readyRead,
        [=] () {
            QTextCodec *codec = QTextCodec::codecForName("utf-8");
            QString dir = codec->toUnicode(cmdProcess->readAll());
            qDebug() << dir;});

    //cmdProcess->start(program, arguments);
    cmdProcess->start(program);
    return a.exec();
}
