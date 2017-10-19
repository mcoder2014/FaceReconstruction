#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include <QWidget>

namespace Ui {
class ModelViewer;
}

class ModelViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ModelViewer(QWidget *parent = 0);
    ~ModelViewer();

private:
    Ui::ModelViewer *ui;
};

#endif // MODELVIEWER_H
