#-------------------------------------------------
#
# Project created by QtCreator 2017-10-19T09:49:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = start
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = ../bin        # 生成的文件所在的目录
MOC_DIR = ../moc        # Q_OBJECT转换后的类
RCC_DIR = ../rcc        # .qrc文件转换后的位置
OBJECTS_DIR = ../tmp    # 编译中间文件的位置

INCLUDEPATH += E:/openCV-2.4.11/build/install/include \
               E:/boost1.50/boost_1_50_0 \
               $$PWD/../libs \
               $$PWD \
               $$PWD/../libs/eigen-eigen-b9cd8366d4e8

LIBS += -L$$PWD/../libs -ldlib \
E:\openCV-2.4.11\build\lib\Release\opencv_videostab2411.lib \
E:\openCV-2.4.11\build\lib\Release\opencv_ts2411.lib   \
E:\openCV-2.4.11\build\lib\Release\opencv_superres2411.lib  \
E:\openCV-2.4.11\build\lib\Release\opencv_stitching2411.lib \
E:\openCV-2.4.11\build\lib\Release\opencv_contrib2411.lib   \
E:\boost1.50\build\lib\boost_system-vc140-mt-1_50.lib   \
E:\boost1.50\build\lib\boost_filesystem-vc140-mt-1_50.lib   \
E:\boost1.50\build\lib\boost_program_options-vc140-mt-1_50.lib  \
E:\openCV-2.4.11\build\lib\Release\opencv_nonfree2411.lib   \
E:\openCV-2.4.11\build\lib\Release\opencv_ocl2411.lib   \
E:\openCV-2.4.11\build\lib\Release\opencv_gpu2411.lib   \
E:\openCV-2.4.11\build\lib\Release\opencv_photo2411.lib \
E:\openCV-2.4.11\build\lib\Release\opencv_objdetect2411.lib \
E:\openCV-2.4.11\build\lib\Release\opencv_legacy2411.lib    \
E:\openCV-2.4.11\build\lib\Release\opencv_video2411.lib \
E:\openCV-2.4.11\build\lib\Release\opencv_ml2411.lib    \
E:\openCV-2.4.11\build\lib\Release\opencv_calib3d2411.lib \
E:\openCV-2.4.11\build\lib\Release\opencv_features2d2411.lib    \
E:\openCV-2.4.11\build\lib\Release\opencv_highgui2411.lib   \
E:\openCV-2.4.11\build\lib\Release\opencv_imgproc2411.lib \
E:\openCV-2.4.11\build\lib\Release\opencv_flann2411.lib \
E:\openCV-2.4.11\build\lib\Release\opencv_core2411.lib  \


SOURCES += \
    main.cpp \
    GUI/MainWindow.cpp \
    Face/FitModel.cpp \
    GUI/PicWidget.cpp \
    GUI/ModelViewer.cpp \
    GLOBAL_VAR.cpp \
    Face/FaceDetection.cpp

HEADERS += \
    GUI/MainWindow.h \
    Face/FitModel.h \
    GUI/PicWidget.h \
    GUI/ModelViewer.h \
    GLOBAL_VAR.h \
    Face/FaceDetection.h \

FORMS += \
    GUI/MainWindow.ui \
    GUI/PicWidget.ui \
    GUI/ModelViewer.ui
