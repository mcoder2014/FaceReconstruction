#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>

class PicWidget;        // 显示图片的widget

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void init();            // 初始化程序
    void initConnection();  // 链接QAction

    // QMenu
    QMenu *menu_file;                       // 文件菜单
    QMenu *menu_reconstruction;             // 主菜单

    // QAction
    // 文件
    QAction *action_addimage;               // 打开图片
    QAction *action_addObjModel;            // 打开模型

    QAction *action_landmark;               // 执行查找标记点
    QAction *action_fitmodel;               // 执行生成模型
    QAction *action_face_reconstruction;    // 直接执行完整程序-人脸重建

    // widget
    PicWidget *picWidget;

private slots:
    void openImage();                       // 打开图片
    void setImage(QString filePath);        // 设置图片
    void faceDetection();                   // 执行人脸检测
    void faceReconsturction();              // 执行人脸重建
};

#endif // MAINWINDOW_H
