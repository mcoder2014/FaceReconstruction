#ifndef PICWIDGET_H
#define PICWIDGET_H

#include <QWidget>
#include <QImage>

namespace Ui {
class PicWidget;
}

class PicWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PicWidget(QWidget *parent = 0);
    ~PicWidget();
    QString getImagePath(){return imagePath;}
    QImage getImage(){return this->image;}

public slots:
    void setImagePath(QString path);

private:
    Ui::PicWidget *ui;
    QString imagePath;
    QImage image;

};

#endif // PICWIDGET_H
