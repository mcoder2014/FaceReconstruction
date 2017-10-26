#include "PicWidget.h"
#include "ui_PicWidget.h"
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QPainter>
#include <QPen>
#include "Face/FaceDetection.h"
#include "Face/FitModel.h"

PicWidget::PicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PicWidget)
{
    ui->setupUi(this);
    this->scaleFactor = 1.0;

    this->init();               // 初始化
    this->initConnection();     // 初始化信号槽链接
    this->initThread();
    this->process = none;
    this->markedPoints = NULL;
}

PicWidget::~PicWidget()
{
    delete ui;
    this->thread_face.quit();
    this->thread_face.wait();
}

///
/// \brief PicWidget::setImagePath
/// \param path
///
void PicWidget::setImagePath(QString path)
{
    this->imagePath = path;

    // 更换图片
    QImage image(path);
    this->image = image;
    QPixmap pic = QPixmap::fromImage(image);

    qDebug() << "pic.size" <<pic.size();

    // 设置图片大小和窗口等大
    ui->picLabel->setPixmap(pic);
//    ui->picLabel->resize(pic.width(),pic.height());
    ui->imageArea->widget()->resize(pic.size());
    if(this->process != none)
        this->fitToWindow();        // 适应窗口大小

    this->process = loadedImage;    // 更改当前状态为加载图片
    this->setLisetSelection(0);

    qDebug() << "piclabel.size" << ui->picLabel->size()
             << "widget size: " << ui->imageArea->widget()->size();

}

///
/// \brief PicWidget::selectImage
/// 在picWidget中更换图片
///
void PicWidget::selectImage()
{

    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);     // 打开文件模式
    fileDialog->setFileMode(QFileDialog::ExistingFile);     // 显示存在的文件
    fileDialog->setViewMode(QFileDialog::Detail);           // 显示详细模式
    fileDialog->setNameFilter(tr("Image Files(*.jpg *.png)"));  // 过滤图片
    fileDialog->setWindowTitle(tr("select other image to open"));    // 对话框标题

    if(fileDialog->exec() == QDialog::Accepted)
    {
        QString path = fileDialog->selectedFiles()[0];      // 用户选择的第一个文件

        qDebug() << "select image: " << path;

//        this->imagePath = path;         // 记录图片路径
        this->setImagePath(path);       // 打开图片
    }
}

///
/// \brief PicWidget::fitToWindow
/// 缩放大小到适应窗口大小
///
void PicWidget::fitToWindow()
{
    qDebug() << "imageArea.size()" << ui->imageArea->size();

    double imageWidth = ui->picLabel->pixmap()->size().width();
    double imageHeight = ui->picLabel->pixmap()->size().height();

    double imageAreaWidgetWidth = ui->imageArea->viewport()->width();
    double imageAreaWidgetHeight = ui->imageArea->viewport()->height();

    double ratioWidth = imageAreaWidgetWidth / imageWidth;
    double ratioHeight = imageAreaWidgetHeight / imageHeight;

    double ratio = ratioWidth < ratioHeight ? ratioWidth : ratioHeight;

    this->setScaleFactor(ratio);

}

///
/// \brief PicWidget::zoomIn
///
void PicWidget::zoomIn()
{
    this->scaleImage(1.25);
}

///
/// \brief PicWidget::zoomOut
///
void PicWidget::zoomOut()
{
    this->scaleImage(0.8);
}

///
/// \brief PicWidget::stateChanged
/// \param prcess
///
void PicWidget::stateChanged(PicWidget::PROCESS prcess)
{

    switch (process)
    {
    case none:
        break;
    case loadedImage:
        break;
    case detected:
        break;
    case reconstruction:
        break;
    default:
        break;
    }

    this->process = process;

}

///
/// \brief PicWidget::setLisetSelection
/// \param index
///
void PicWidget::setLisetSelection(int index)
{
    ui->ImageList->setCurrentRow(index);
}

///
/// \brief PicWidget::faceDetection
/// 人脸检测
///
void PicWidget::faceDetection()
{
//    Q_ASSERT(this->process != none);
    if(this->process == none)
    {
        QMessageBox::warning(
                    this,
                    tr("Warning"),
                    tr("Please load image first!"));
        return;
    }

    QProgressDialog *progress = new QProgressDialog(
                tr("Face Detection,Please press second list label until progress finished"),
                tr("Abort Detected"),
                0, 100,this);

    FaceDetection *detector = FaceDetection::getInstance();
    progress->setWindowModality(Qt::WindowModal);
    progress->setWindowTitle(tr("Message"));
    progress->setValue(0);
    progress->show();

    qDebug() << thread_face.isRunning()
             << thread_face.isFinished();

    connect(detector, SIGNAL(signals_progressValue(int)),
            progress, SLOT(setValue(int)));
    connect(detector, SIGNAL(signals_finished()),
            progress, SLOT(deleteLater()));

    emit this->signals_landmarkAllFace(this->image);
}

///
/// \brief PicWidget::faceReconstruction
///
void PicWidget::faceReconstruction()
{
    if(this->process == none)
    {
        QMessageBox::warning(
                    this,
                    tr("Warning"),
                    tr("Please load image first!"));
        return;
    }
    else if(this->process == loadedImage)
    {
        QMessageBox::warning(
                    this,
                    tr("Warning"),
                    tr("Please execute face detection first!"));
        return;
    }

    FitModel *fitmodel = FitModel::getInstance();           // 获得对象
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);     // 打开文件模式
    fileDialog->setFileMode(QFileDialog::AnyFile);        // 显示路径
    fileDialog->setViewMode(QFileDialog::Detail);           // 显示详细模式
//    fileDialog->setNameFilter(tr("Image Files(*.jpg *.png)"));  // 过滤图片
    fileDialog->setWindowTitle(tr("Choose a floder to save model"));    // 对话框标题
    fileDialog->setDirectory(fitmodel->getOutputPath());
    fileDialog->selectFile(fitmodel->getFileName());

    if(fileDialog->exec() == QDialog::Accepted)
    {
        QDir dir = fileDialog->directory();
        fitmodel->setOutputPath(dir.absolutePath());    // 设置保存路径
        qDebug() << "selected directory: "
                 << dir.absolutePath()
                 << "selected files:"
                 << fileDialog->selectedFiles()[0];
        qDebug() <<"fit model finished";
    }

    faceReconstruction(0);
}

///
/// \brief PicWidget::faceReconstruction
/// \param i
///
void PicWidget::faceReconstruction(int i)
{
    Q_ASSERT(this->process != none);
    Q_ASSERT(this->process != loadedImage);

    if(i >= this->markedPoints->size() || i < 0)
    {
        qDebug() << "the request face is out of range";
        return;
    }
    LandmarkCollection<cv::Vec2f>* landmarks = this->markedPoints->operator [](i);

    QProgressDialog *progress = new QProgressDialog(
                tr("Face Reconstruction,Please press third list label until progress finished"),
                tr("Abort Reconstruction"),
                0, 100,this);

    FitModel *fitmodel = FitModel::getInstance();
    progress->setWindowModality(Qt::WindowModal);
    progress->setWindowTitle(tr("Message"));
    progress->setValue(0);
    progress->show();

    qDebug() << thread_face.isRunning()
             << thread_face.isFinished();

    connect(fitmodel, SIGNAL(signals_progressValue(int)),
            progress, SLOT(setValue(int)));
    connect(fitmodel, SIGNAL(signals_finished()),
            progress, SLOT(deleteLater()));


    emit this->signals_fitModel(
                this->imagePath,
                landmarks);

}

void PicWidget::showMessage(QString title, QString msg)
{
    QMessageBox::information(
                this,
                title,
                msg);
}

///
/// \brief PicWidget::scaleImage
/// \param factor
/// 在之前缩放比例的基础上缩放图片
///
void PicWidget::scaleImage(double factor)
{
    if(!ui->picLabel->pixmap())
    {
        // 如果没有图片可以用来缩放
        return;
    }

    scaleFactor *= factor;  // 叠加缩放参数
    ui->imageArea->widget()->resize(
                scaleFactor * ui->picLabel->pixmap()->size());

    adjustScrollBar(ui->imageArea->horizontalScrollBar(), factor);
    adjustScrollBar(ui->imageArea->verticalScrollBar(), factor);
}

///
/// \brief PicWidget::setScaleFactor
/// \param scaleFactor
/// 直接给定缩放比例
///
void PicWidget::setScaleFactor(double scaleFactor)
{
//    qDebug() << "setScaleFactor: " <<scaleFactor;
    double factor = scaleFactor / this->scaleFactor;

    this->scaleFactor = scaleFactor;
    ui->imageArea->widget()->resize(
                this->scaleFactor * ui->picLabel->pixmap()->size());

    adjustScrollBar(ui->imageArea->horizontalScrollBar(), factor);
    adjustScrollBar(ui->imageArea->verticalScrollBar(), factor);

}

///
/// \brief PicWidget::adjustScrollBar
/// \param scrollBar
/// \param factor
///
void PicWidget::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            +(factor -1) * scrollBar->pageStep()/2));
}

///
/// \brief PicWidget::imageRowSelectedRow
/// \param index
///
void PicWidget::imageRowSelectedRow(int index)
{
    if(index == 0)
    {
        // 查看原图时
        if(process != none)
        {
            ui->picLabel->setPixmap(
                        QPixmap::fromImage(image));

        }
    }
    else if(index == 1)
    {
        // 查看检测的图片时
        if(process != none && process != loadedImage)
        {
            ui->picLabel->setPixmap(
                        QPixmap::fromImage(markedImage));
        }
    }
    else if(index == 2)
    {
        // 重建后
        if(process == reconstruction)
        {
            ui->picLabel->setPixmap(
                        QPixmap::fromImage(isoImage));
        }
    }

    this->setScaleFactor(scaleFactor);
    qDebug() << "clicked id: " << index;
}

QImage PicWidget::drawLandMark(
        QVector<LandmarkCollection<cv::Vec2f> *> *marks,
        QImage &image)
{
    QImage markedImage(image);          // 复制一份
    QPainter painter(&markedImage);    // 新建一个画笔

    QPen pen;
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::green);
    pen.setBrush(brush);
    pen.setWidth(5);    // 线条粗细
    painter.setPen(pen);

    for(int face = 0; face < marks->size(); face ++)
    {
        // 依次处理每张脸
        LandmarkCollection<cv::Vec2f> *landmarks = marks->operator [](face);

        // 轮廓
        QPainterPath border_path;
        border_path.moveTo(
                    (*landmarks)[0].coordinates[0],
                (*landmarks)[0].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[1].coordinates[0],
                (*landmarks)[1].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[2].coordinates[0],
                (*landmarks)[2].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[3].coordinates[0],
                (*landmarks)[3].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[4].coordinates[0],
                (*landmarks)[4].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[5].coordinates[0],
                (*landmarks)[5].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[6].coordinates[0],
                (*landmarks)[6].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[7].coordinates[0],
                (*landmarks)[7].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[8].coordinates[0],
                (*landmarks)[8].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[9].coordinates[0],
                (*landmarks)[9].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[10].coordinates[0],
                (*landmarks)[10].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[11].coordinates[0],
                (*landmarks)[11].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[12].coordinates[0],
                (*landmarks)[12].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[13].coordinates[0],
                (*landmarks)[13].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[14].coordinates[0],
                (*landmarks)[15].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[16].coordinates[0],
                (*landmarks)[16].coordinates[1]);

        painter.drawPath(border_path);

        // 右眉
        QPainterPath rightEyeBrow;
        rightEyeBrow.moveTo(
                    (*landmarks)[17].coordinates[0],
                (*landmarks)[17].coordinates[1]);
        rightEyeBrow.lineTo(
                    (*landmarks)[18].coordinates[0],
                (*landmarks)[18].coordinates[1]);
        rightEyeBrow.lineTo(
                    (*landmarks)[19].coordinates[0],
                (*landmarks)[19].coordinates[1]);
        rightEyeBrow.lineTo(
                    (*landmarks)[20].coordinates[0],
                (*landmarks)[20].coordinates[1]);
        rightEyeBrow.lineTo(
                    (*landmarks)[21].coordinates[0],
                (*landmarks)[21].coordinates[1]);

        painter.drawPath(rightEyeBrow);

        // 左眉
        QPainterPath leftEyeBrow;
        leftEyeBrow.moveTo(
                    (*landmarks)[22].coordinates[0],
                (*landmarks)[22].coordinates[1]);

        leftEyeBrow.lineTo(
                    (*landmarks)[23].coordinates[0],
                (*landmarks)[23].coordinates[1]);
        leftEyeBrow.lineTo(
                    (*landmarks)[24].coordinates[0],
                (*landmarks)[24].coordinates[1]);
        leftEyeBrow.lineTo(
                    (*landmarks)[25].coordinates[0],
                (*landmarks)[25].coordinates[1]);
        leftEyeBrow.lineTo(
                    (*landmarks)[26].coordinates[0],
                (*landmarks)[26].coordinates[1]);

        painter.drawPath(leftEyeBrow);

        // 鼻梁
        QPainterPath brigeOfNose;
        brigeOfNose.moveTo(
                    (*landmarks)[27].coordinates[0],
                (*landmarks)[27].coordinates[1]);
        brigeOfNose.lineTo(
                    (*landmarks)[28].coordinates[0],
                (*landmarks)[28].coordinates[1]);
        brigeOfNose.lineTo(
                    (*landmarks)[29].coordinates[0],
                (*landmarks)[29].coordinates[1]);
        brigeOfNose.lineTo(
                    (*landmarks)[30].coordinates[0],
                (*landmarks)[30].coordinates[1]);

        painter.drawPath(brigeOfNose);

        // 鼻子
        QPainterPath bottomOfNose;
        bottomOfNose.moveTo(
                    (*landmarks)[31].coordinates[0],
                (*landmarks)[31].coordinates[1]);
        bottomOfNose.lineTo(
                    (*landmarks)[32].coordinates[0],
                (*landmarks)[32].coordinates[1]);
        bottomOfNose.lineTo(
                    (*landmarks)[33].coordinates[0],
                (*landmarks)[33].coordinates[1]);
        bottomOfNose.lineTo(
                    (*landmarks)[34].coordinates[0],
                (*landmarks)[34].coordinates[1]);
        bottomOfNose.lineTo(
                    (*landmarks)[35].coordinates[0],
                (*landmarks)[35].coordinates[1]);

        //右眼
        QPainterPath rightEye;
        rightEye.moveTo(
                    (*landmarks)[36].coordinates[0],
                (*landmarks)[36].coordinates[1]);
        rightEye.lineTo(
                    (*landmarks)[37].coordinates[0],
                (*landmarks)[37].coordinates[1]);
        rightEye.lineTo(
                    (*landmarks)[38].coordinates[0],
                (*landmarks)[38].coordinates[1]);
        rightEye.lineTo(
                    (*landmarks)[39].coordinates[0],
                (*landmarks)[39].coordinates[1]);
        rightEye.lineTo(
                    (*landmarks)[40].coordinates[0],
                (*landmarks)[40].coordinates[1]);
        rightEye.lineTo(
                    (*landmarks)[41].coordinates[0],
                (*landmarks)[41].coordinates[1]);
        rightEye.lineTo(
                    (*landmarks)[36].coordinates[0],
                (*landmarks)[36].coordinates[1]);
        painter.drawPath(rightEye);

        // 左眼
        QPainterPath leftEye;
        leftEye.moveTo(
                    (*landmarks)[42].coordinates[0],
                (*landmarks)[42].coordinates[1]);
        leftEye.lineTo(
                    (*landmarks)[43].coordinates[0],
                (*landmarks)[43].coordinates[1]);
        leftEye.lineTo(
                    (*landmarks)[44].coordinates[0],
                (*landmarks)[44].coordinates[1]);
        leftEye.lineTo(
                    (*landmarks)[45].coordinates[0],
                (*landmarks)[45].coordinates[1]);
        leftEye.lineTo(
                    (*landmarks)[46].coordinates[0],
                (*landmarks)[46].coordinates[1]);
        leftEye.lineTo(
                    (*landmarks)[47].coordinates[0],
                (*landmarks)[47].coordinates[1]);
        leftEye.lineTo(
                    (*landmarks)[42].coordinates[0],
                (*landmarks)[42].coordinates[1]);
        painter.drawPath(leftEye);

        // 嘴巴
        QPainterPath mouth;
        mouth.moveTo(
                    (*landmarks)[48].coordinates[0],
                (*landmarks)[48].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[49].coordinates[0],
                (*landmarks)[49].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[50].coordinates[0],
                (*landmarks)[50].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[51].coordinates[0],
                (*landmarks)[51].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[52].coordinates[0],
                (*landmarks)[52].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[53].coordinates[0],
                (*landmarks)[53].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[54].coordinates[0],
                (*landmarks)[54].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[55].coordinates[0],
                (*landmarks)[55].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[56].coordinates[0],
                (*landmarks)[56].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[57].coordinates[0],
                (*landmarks)[57].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[58].coordinates[0],
                (*landmarks)[58].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[59].coordinates[0],
                (*landmarks)[59].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[48].coordinates[0],
                (*landmarks)[48].coordinates[1]);
        mouth.moveTo(
                    (*landmarks)[60].coordinates[0],
                (*landmarks)[60].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[61].coordinates[0],
                (*landmarks)[61].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[62].coordinates[0],
                (*landmarks)[62].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[63].coordinates[0],
                (*landmarks)[63].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[64].coordinates[0],
                (*landmarks)[64].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[65].coordinates[0],
                (*landmarks)[65].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[66].coordinates[0],
                (*landmarks)[66].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[67].coordinates[0],
                (*landmarks)[67].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[60].coordinates[0],
                (*landmarks)[60].coordinates[1]);
        painter.drawPath(mouth);

    }

    return markedImage;
}

void PicWidget::handle_landmarkAllFace(
        QVector<LandmarkCollection<cv::Vec2f> *> *vector)
{
    Q_ASSERT(this->process != none);
    this->markedPoints = vector;

    this->markedImage = this->drawLandMark(
                this->markedPoints,this->image);

    if(this->markedPoints->size() == 0)
    {
        QMessageBox::warning(
                    this,
                    tr("Warning!"),
                    tr("Program has not detected a face, please change a image"));

    }
    else
    {
        this->process = detected;       // 修改阶段为到达检测完人脸的状态
        this->imageRowSelectedRow(1);
    }

}

///
/// \brief PicWidget::handle_fitModelIsoMap
/// \param image
///
void PicWidget::handle_fitModelIsoMap(QImage image)
{
    this->isoImage = image;
    this->process = reconstruction;
    this->imageRowSelectedRow(2);
}

///
/// \brief PicWidget::init
///
void PicWidget::init()
{
    // QMenu
    this->imageAreaContextMenu = new QMenu(tr("ImageAreaMenu"));
    this->listWidgetContextMenu = new QMenu(tr("ListMenu"));

    this->action_fitWindow = new QAction(tr("Fit Window"));     // fitWindow
    this->action_zoomIn = new QAction(tr("Zoom In"));           // zoomIn
    this->action_zoomOut = new QAction(tr("Zoom Out"));         // zoomOut
    this->action_zoom25 = new QAction(tr("25%"));               // 缩放到 25%
    this->action_zoom50 = new QAction(tr("50%"));               // 缩放到 50%
    this->action_zoom100 = new QAction(tr("100%"));             // 缩放到 100%
    this->action_zoom200 = new QAction(tr("200%"));             // 缩放到 200%

    // imageArea
    this->imageAreaContextMenu->addAction(this->action_fitWindow);
    this->imageAreaContextMenu->addAction(this->action_zoomIn);
    this->imageAreaContextMenu->addAction(this->action_zoomOut);
    this->imageAreaContextMenu->addAction(this->action_zoom25);
    this->imageAreaContextMenu->addAction(this->action_zoom50);
    this->imageAreaContextMenu->addAction(this->action_zoom100);
    this->imageAreaContextMenu->addAction(this->action_zoom200);
}

///
/// \brief PicWidget::initConnection
///
void PicWidget::initConnection()
{

    // 选择图片
    connect(ui->btn_changImage, SIGNAL(pressed()),
            this, SLOT(selectImage()));

    // imageArea contextMenu
    ui->imageArea->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->imageArea, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenu(QPoint)));

    // fit Window
    connect(this->action_fitWindow, SIGNAL(triggered(bool)),
            this, SLOT(fitToWindow()));

    // zoom In
    connect(this->action_zoomIn, SIGNAL(triggered(bool)),
            this, SLOT(zoomIn()));

    connect(ui->btn_zoomIn, SIGNAL(pressed()),
            this, SLOT(zoomIn()));

    // zoom out
    connect(this->action_zoomOut, SIGNAL(triggered(bool)),
            this, SLOT(zoomOut()));

    connect(ui->btn_zoomOut, SIGNAL(pressed()),
            this, SLOT(zoomOut()));

    // 使用 C++11特性
    // zoom 25
    connect(this->action_zoom25, &QAction::triggered,
            [=](){
        this->setScaleFactor(0.25);
    });

    // zoom 50
    connect(this->action_zoom50, &QAction::triggered,
            [=](){
        this->setScaleFactor(0.50);
    });

    // zoom 100
    connect(this->action_zoom100, &QAction::triggered,
            [=](){
        this->setScaleFactor(1.0);
    });

    // zoom 200
    connect(this->action_zoom200, &QAction::triggered,
            [=](){
        this->setScaleFactor(2.0);
    });

    // 选择row
    connect(ui->ImageList, SIGNAL(currentRowChanged(int)),
            this, SLOT(imageRowSelectedRow(int)));

    // face detection
    connect(ui->btn_faceDetection, SIGNAL(pressed()),
            this, SLOT(faceDetection()));

    // faceReconstruction
    connect(ui->btn_faceReconstruction, SIGNAL(pressed()),
            this, SLOT(faceReconstruction()));

}

///
/// \brief PicWidget::initThread
///
void PicWidget::initThread()
{
    // 将人脸检测类塞入分线程
    FaceDetection *detector = FaceDetection::getInstance();
    detector->moveToThread(&thread_face);

    // 传入参数->人脸识别
    connect(this, SIGNAL(signals_landmarkAllFace(QImage)),
            detector, SLOT(thread_landmarkAllFace(QImage)));

    // 传出参数->人脸识别结束
    connect(detector,
            SIGNAL(signals_landmarkAll(QVector<LandmarkCollection<cv::Vec2f>*>*)),
            this,
            SLOT(handle_landmarkAllFace(QVector<LandmarkCollection<cv::Vec2f>*>*)));

    // 显示信息
    connect(detector,SIGNAL(signals_msg(QString, QString)),
            this, SLOT(showMessage(QString, QString)));

    // 利用多线程初始化
    connect(this, SIGNAL(signals_initFaceDetection()),
            detector, SLOT(thread_init()));
    emit this->signals_initFaceDetection();

    // 将fitmodel塞入分线程
    FitModel *fitModel = FitModel::getInstance();

    // 链接-执行操作
    connect(this,
            SIGNAL(signals_fitModel(QString,LandmarkCollection<cv::Vec2f>*)),
            fitModel,
            SLOT(thread_fitModel(QString,LandmarkCollection<cv::Vec2f>*)));

    // 返回参数-人脸贴图
    connect(fitModel, SIGNAL(signals_isoMap(QImage)),
            this, SLOT(handle_fitModelIsoMap(QImage)));

    // 显示消息
    connect(fitModel, SIGNAL(signals_msg(QString,QString)),
            this, SLOT(showMessage(QString,QString)));

    thread_face.start();
}

///
/// \brief PicWidget::showContextMenu
/// \param pos
///
void PicWidget::showContextMenu(const QPoint &pos)
{
//    qDebug() << "imageArea contextMenu";
    QPoint global = ui->imageArea->mapToGlobal(pos);
    this->imageAreaContextMenu->exec(global);
}
