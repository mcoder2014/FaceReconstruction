#include "ModelViewer.h"
#include "ui_ModelViewer.h"

ModelViewer::ModelViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModelViewer)
{
    ui->setupUi(this);
}

ModelViewer::~ModelViewer()
{
    delete ui;
}
