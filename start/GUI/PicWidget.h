#ifndef PICWIDGET_H
#define PICWIDGET_H

#include <QWidget>

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

public slots:
    void setImagePath(QString path);

private:
    Ui::PicWidget *ui;
    QString imagePath;

};

#endif // PICWIDGET_H
