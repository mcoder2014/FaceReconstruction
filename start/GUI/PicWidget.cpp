#include "PicWidget.h"
#include "ui_PicWidget.h"
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QMenu>

PicWidget::PicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PicWidget)
{
    ui->setupUi(this);
    this->scaleFactor = 1.0;

    this->init();               // 初始化
    this->initConnection();     // 初始化信号槽链接
}

PicWidget::~PicWidget()
{
    delete ui;
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
//    this->setScaleFactor(this->scaleFactor);
    this->fitToWindow();        // 适应窗口大小

    qDebug() << "piclabel.size" << ui->picLabel->size();

}

void PicWidget::selectImage()
{

}

///
/// \brief PicWidget::fitToWindow
/// 缩放大小到适应窗口大小
///
void PicWidget::fitToWindow()
{

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
//    ui->imageArea->widget()->update();
//    ui->imageArea->update();
//    qDebug() << "picLabel size: " << ui->picLabel->size()
//             << " widget size: " << ui->imageArea->widget()->size()
//             << " image area size: "<< ui->imageArea->size();

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

    // zoom out
    connect(this->action_zoomOut, SIGNAL(triggered(bool)),
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
