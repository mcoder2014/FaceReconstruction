#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "GUI/PicWidget.h"
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include "Face/FaceDetection.h"
#include "Face/FitModel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->init();

}

MainWindow::~MainWindow()
{
    delete ui;
}

///
/// \brief MainWindow::init
///
void MainWindow::init()
{
    this->resize(800, 600);

    this->picWidget = new PicWidget(this);
    this->picWidget->setVisible(false);

    // 初始化QAction

    // 打开图片
    this->action_addimage = new QAction(tr("open Image Files"));
    this->action_addimage->setStatusTip(tr("Select a image and open it"));
    this->action_addimage->setIcon(QIcon(":/img/sources/iconImg.png"));

    // 打开模型-等实现了三维场景后再做s
    this->action_addObjModel = new QAction(tr("open obj Files"));
    this->action_addObjModel->setStatusTip(tr("Select an obj file and open it"));

    // landmark 作人脸标记s
    this->action_landmark = new QAction(tr("phase landmark"));
    this->action_landmark->setStatusTip(tr("landmark"));

    // fit model
    this->action_fitmodel = new QAction(tr("phase fit model"));
    this->action_fitmodel->setStatusTip(tr("fit model"));

    // face Reconstruction
    this->action_face_reconstruction = new QAction(tr("face reconstruction"));
    this->action_face_reconstruction->setStatusTip(tr("face reconstruction"));
    this->action_face_reconstruction->setIcon(
                QIcon(":/img/sources/reconstruction.png"));

    // about qt
    this->action_aboutQt = new QAction(tr("Qt"));
    this->action_aboutQt->setStatusTip(tr("Tell you about qt"));

    // about app
    this->action_aboutApp = new QAction(tr("App"));
    this->action_aboutApp->setStatusTip(tr("Tell you about this app"));

    // menu
    this->menu_file = new QMenu(tr("Files"));
    this->menu_reconstruction = new QMenu(tr("Reconstruction"));
    this->menu_about = new QMenu(tr("About"));

    // file
    this->menu_file->addAction(this->action_addimage);
    this->menu_file->addAction(this->action_addObjModel);

    // reconstruction
    this->menu_reconstruction->addAction(this->action_face_reconstruction);
    this->menu_reconstruction->addAction(this->action_landmark);
    this->menu_reconstruction->addAction(this->action_fitmodel);

    // about
    this->menu_about->addAction(this->action_aboutQt);
    this->menu_about->addAction(this->action_aboutApp);

    ui->menuBar->addMenu(this->menu_file);
    ui->menuBar->addMenu(this->menu_reconstruction);
    ui->menuBar->addMenu(this->menu_about);

    ui->mainToolBar->addAction(this->action_addimage);
    ui->mainToolBar->addAction(this->action_face_reconstruction);

    initConnection();
}

void MainWindow::initConnection()
{
    connect(this->action_addimage, SIGNAL(triggered(bool)),
            this, SLOT(openImage()));                       // 打开图片

    connect(ui->selectImageButton, SIGNAL(pressed()),
            this, SLOT(openImage()));                       // 打开图片按钮

    connect(this->action_landmark, SIGNAL(triggered(bool)),
            this, SLOT(faceDetection()));                   // 人脸检测

    connect(this->action_face_reconstruction, SIGNAL(triggered(bool)),
            this, SLOT(faceReconsturction()));

    connect(this->action_fitmodel, SIGNAL(triggered(bool)),
            this, SLOT(testFitModel()));

    // 关于Qt
    connect(this->action_aboutQt, SIGNAL(triggered(bool)),
            this, SLOT(aboutQtBox()));

    // 关于App
    connect(this->action_aboutApp, SIGNAL(triggered(bool)),
            this, SLOT(aboutAppBox()));


}

///
/// \brief MainWindow::openImage
///
void MainWindow::openImage()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);     // 打开文件模式
    fileDialog->setFileMode(QFileDialog::ExistingFile);     // 显示存在的文件
    fileDialog->setViewMode(QFileDialog::Detail);           // 显示详细模式
    fileDialog->setNameFilter(tr("Image Files(*.jpg *.png)"));  // 过滤图片
    fileDialog->setWindowTitle(tr("Choose one image file and open it"));    // 对话框标题

    if(fileDialog->exec() == QDialog::Accepted)
    {
        QString path = fileDialog->selectedFiles()[0];      // 用户选择的文件名

        qDebug() << "selected image file:"
                 << path;
        this->imagePath = path;         // 记录图片地址
        this->setImage(path);
    }
}

///
/// \brief MainWindow::setImage
/// \param filePath
///
void MainWindow::setImage(QString filePath)
{
    if(this->picWidget == NULL)
    {
        this->picWidget = new PicWidget();
    }

    this->setCentralWidget(this->picWidget);
    this->picWidget->setVisible(true);
    this->picWidget->setImagePath(filePath);

}

///
/// \brief MainWindow::faceDetection
///
void MainWindow::faceDetection()
{
    if(this->picWidget == NULL)
        return;
    QString filePath = this->picWidget->getImagePath();
    if(filePath.size() == 0)
         return;

    FaceDetection *detector = FaceDetection::getInstance();
    detector->landmark(filePath);
}

///
/// \brief MainWindow::faceReconsturction
///
void MainWindow::faceReconsturction()
{

    FitModel fitmodel;                                      // 新建对象

    if(this->picWidget == NULL)
        return;
    QString filePath = this->picWidget->getImagePath();
    if(filePath.size() == 0)
         return;

    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);     // 打开文件模式
    fileDialog->setFileMode(QFileDialog::Directory);     // 显示存在的文件
    fileDialog->setViewMode(QFileDialog::Detail);           // 显示详细模式
//    fileDialog->setNameFilter(tr("Image Files(*.jpg *.png)"));  // 过滤图片
    fileDialog->setWindowTitle(tr("Choose a floder to save model"));    // 对话框标题
    fileDialog->setDirectory(fitmodel.getOutputPath());

    if(fileDialog->exec() == QDialog::Accepted)
    {
        QDir dir = fileDialog->directory();
        fitmodel.setOutputPath(dir.absolutePath());
        qDebug() << "selected directory: "
                 << dir.absolutePath();

        fitmodel.fitmodel(this->imagePath);     // 执行生成模型
        qDebug() <<"fit model finished";
    }
}

void MainWindow::testFitModel()
{
    FitModel fitmodel;                                      // 新建对象

    if(this->picWidget == NULL)
        return;
    QString filePath = this->picWidget->getImagePath();
    if(filePath.size() == 0)
         return;


    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);     // 打开文件模式
    fileDialog->setFileMode(QFileDialog::ExistingFile);     // 显示存在的文件
    fileDialog->setViewMode(QFileDialog::Detail);           // 显示详细模式
    fileDialog->setNameFilter(tr("Image Files(*.jpg *.png)"));  // 过滤图片
    fileDialog->setWindowTitle(tr("Choose one image file and open it"));    // 对话框标题

    if(fileDialog->exec() == QDialog::Accepted)
    {
        QString path = fileDialog->selectedFiles()[0];      // 用户选择的文件名

        qDebug() << "selected image file:"
                 << path;
        this->setImage(path);

        fileDialog->setAcceptMode(QFileDialog::AcceptOpen);     // 打开文件模式
        fileDialog->setFileMode(QFileDialog::Directory);     // 显示存在的文件
        fileDialog->setViewMode(QFileDialog::Detail);           // 显示详细模式
    //    fileDialog->setNameFilter(tr("Image Files(*.jpg *.png)"));  // 过滤图片
        fileDialog->setWindowTitle(tr("Choose a floder to save model"));    // 对话框标题
        fileDialog->setDirectory(fitmodel.getOutputPath());

        if(fileDialog->exec() == QDialog::Accepted)
        {
            QDir dir = fileDialog->directory();
            fitmodel.setOutputPath(dir.absolutePath());
            qDebug() << "selected directory: "
                     << dir.absolutePath();

            fitmodel.fitmodel(path);
            qDebug() <<"fit model finished";
        }
    }
}

void MainWindow::aboutQtBox()
{
    QMessageBox::aboutQt(
                this,
                tr("About Qt"));
}

void MainWindow::aboutAppBox()
{
    QMessageBox::about(
                this,
                tr("About Face Restruction"),
                tr("There will put some words to Introduce this application."));
}
