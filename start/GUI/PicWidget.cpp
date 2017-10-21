#include "PicWidget.h"
#include "ui_PicWidget.h"
#include <QImage>
#include <QPixmap>

PicWidget::PicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PicWidget)
{
    ui->setupUi(this);
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


    // 设置图片大小和窗口等大
    ui->picLabel->setPixmap(
                pic.scaled(
                    this->size(),
                    Qt::KeepAspectRatio));

}
