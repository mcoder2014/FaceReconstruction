#ifndef PICWIDGET_H
#define PICWIDGET_H

#include <QWidget>
#include <QImage>
#include <QAction>
#include <QScrollBar>
#include <QVector>
#include <QListWidgetItem>
#include <QThread>

#include <eos/core/Landmark.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "objViewer/OpenGLWidget.h"

namespace Ui {
class PicWidget;
}

class QMenu;
using namespace eos;
using eos::core::Landmark;
using eos::core::LandmarkCollection;

class PicWidget
        : public QWidget
{
    Q_OBJECT

public:
    explicit PicWidget(QWidget *parent = 0);
    ~PicWidget();
    QString getImagePath(){return imagePath;}
    QImage getImage(){return this->image;}

    enum PROCESS{none,loadedImage,detected,reconstruction};

public slots:
    void setImagePath(QString path);    // 选择指定路径的图片加载
    void selectImage();     // 重新选择一个图片加载
    void fitToWindow();     // 图片适应窗口大小
    void zoomIn();          // 放大
    void zoomOut();         // 缩小
    void stateChanged(PROCESS prcess);
    void setLisetSelection(int index);
    void faceDetection();           // 执行人脸检测
    void faceReconstruction();      // 重建第一个用户
    void faceReconstruction(int i); // 重建第i个用户
    void showMessage(QString title, QString msg);  // 接收从多线程传来的消息，用QmessageBox显示

private:
    Ui::PicWidget *ui;
    OpenGLWidget *openGLWidget;     // 三维场景

    QString imagePath;      // 读取的图片的路径
    QString outObjPath;     // 输出的Obj图像的路径
    QString outIsoMapPath;  // 输出的ISO贴图的路径
    QImage image;           // 正在处理的图片的原图
    QImage markedImage;     // 被人脸标记的图片
    QImage isoImage;        // 变形后得到的模型的贴图
    double scaleFactor;     // 缩放比例

    QThread thread_face;       // 人脸检测相关的线程
//    QThread thread_faceReconstruction;  // 人脸重建相关的线程

    PROCESS process;        // 处理到的流程
    QVector<LandmarkCollection<cv::Vec2f> *> *markedPoints;  // 人脸检测后的点

    QMenu *imageAreaContextMenu;    // 图片的右键菜单
    QMenu *listWidgetContextMenu;   // listWidget的右键菜单

    // iamgeArea action
    QAction *action_fitWindow;      // 适合屏幕
    QAction *action_zoomIn;         // 放大
    QAction *action_zoomOut;        // 缩小
    QAction *action_zoom25;         // 缩放到25%
    QAction *action_zoom50;         // 缩放到50%
    QAction *action_zoom100;        // 缩放到100%
    QAction *action_zoom200;        // 缩放到200%
    QAction *action_imageAreaSaveAs;    // 将当前显示的图片导出

    void init();            // 初始化一些东西
    void initOpenGLWidget();    // 初始化OpenGL窗口
    void initConnection();  // 初始化链接
    void initThread();      // 结合多线程做一些初始化

private slots:
    void showContextMenu(const QPoint &pos);
    void scaleImage(double factor);             // 缩放图片
    void setScaleFactor(double scaleFactor);    // 直接设置图片显示为百分之多少
    void adjustScrollBar(QScrollBar *scrollBar, double factor);     // 调整进度条
    void imageRowSelectedRow(int index);        // 点击查看其它类别的图片
    QImage drawLandMark(
            QVector<LandmarkCollection<cv::Vec2f> *> * marks,
            QImage& image);     // 结合计算出的结果，在图片上标出人脸

    void handle_landmarkAllFace(QVector<LandmarkCollection<cv::Vec2f> *> * vector);
    void handle_fitModelIsoMap(QImage image);       // 接收人脸贴图
    void handle_fitModelObjPath(QString floderPath,QString objName, QString isoMapName);    // 接收生成的路径

signals:
    void signals_landmarkAllFace(QImage image);     // 标记人脸mark点
    void signals_initFaceDetection();               // 因为FaceDetection初始化耗时严重
    void signals_fitModel(QString image_path,
                          LandmarkCollection<cv::Vec2f>* marks);    // 构建模型


};

#endif // PICWIDGET_H
