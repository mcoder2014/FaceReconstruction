#include "PicWidget.h"
#include "ui_PicWidget.h"

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
}
