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
               E:/boost 1.50/boost_1_50_0/boost \
               $$PWD/../dlib \
               $$PWD/../face_reconstruction/cereal \
               $$PWD/../face_reconstruction/glm \
               $$PWD

LIBS += -L$$PWD/../libs -ldlib

SOURCES += \
        main.cpp \
        MainWindow.cpp \
    Face/FaccDetection.cpp

HEADERS += \
        MainWindow.h \
    Face/FaccDetection.h

FORMS += \
        MainWindow.ui
