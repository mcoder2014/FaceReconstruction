#-------------------------------------------------
#
# Project created by QtCreator 2017-10-19T09:49:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = faceRec
TEMPLATE = app
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
# DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = ../bin       # 生成的文件所在的目录
MOC_DIR = ./moc        # Q_OBJECT转换后的类
RCC_DIR = ./rcc        # .qrc文件转换后的位置
OBJECTS_DIR = ./tmp    # 编译中间文件的位置
UI_DIR = ./ui          # ui_xxx 文件存放位置

win32 {
    INCLUDEPATH += $$PWD \  # 自身
                   $$PWD/../start \ # start 项目 本来没打算做这个导致很多代码都放在了那里
                   $$PWD/../libs \
                   $$PWD/../libs/dlib-18.17 \                           # dlib
                   $$PWD/../libs/eigen-eigen-b9cd8366d4e8 \             # eigen
                   E:/openCV-2.4.11/build/install/include \             # opencv
                   E:/libs/boost-1.50-minGW-5.3/build-mingw-static/include/boost-1_50 \ # boost
                   E:/libs/assimp-3.3-minGW-5.3/assimp-3.3/include      # assimp

    LIBS += -L$$PWD/../libs -ldlib \                                # dlib 静态文件
        # boost
        ../../../libs/boost-1.50-minGW-5.3/build-mingw-static/lib/libboost_system-mgw53-1_50.dll.a \
        ../../../libs/boost-1.50-minGW-5.3/build-mingw-static/lib/libboost_filesystem-mgw53-1_50.dll.a \
        ../../../libs/boost-1.50-minGW-5.3/build-mingw-static/lib/libboost_filesystem-mgw53-1_50.dll.a \
        # opencv
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_calib3d2411.dll.a \
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_contrib2411.dll.a \
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_core2411.dll.a \
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_features2d2411.dll.a \
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_flann2411.dll.a \
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_gpu2411.dll.a \
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_haartraining_engine.a \
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_highgui2411.dll.a \
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_imgproc2411.dll.a \
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_legacy2411.dll.a \
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_ml2411.dll.a \
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_nonfree2411.dll.a \
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_objdetect2411.dll.a \
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_ocl2411.dll.a \
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_photo2411.dll.a \
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_stitching2411.dll.a \
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_superres2411.dll.a \
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_test_flann_pch_dephelp.a \
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_ts2411.a \
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_video2411.dll.a \
        ../../../libs/opencv2.4.11-minGW5.3/build-mingw-release/lib/libopencv_videostab2411.dll.a \
        ../../../libs/assimp-3.3-minGW-5.3/build/code/libassimp.dll.a             # assimp 静态文件
}

unix {

    INCLUDEPATH += $$PWD \  # 自身
                   $$PWD/../start \ # start 项目 本来没打算做这个导致很多代码都放在了那里
                   $$PWD/../libs \
                   $$PWD/../libs/dlib-18.17 \                          # dlib
                   $$PWD/../libs/eigen-eigen-b9cd8366d4e8 \            # eigen
                   /usr/include \                                      # boost assimp
                   /usr/local/include                                  # opencv
                   #/usr/local/include                                  # dlib

    LIBS += \ #-L/usr/local/lib -ldlib \
            \ #-L/mnt/e/libs/dlibs-18.17-minGW-5.3/dlib-18.17/build -ldlib \            # dlib
            -L /home/chaoqun/dlib-18.17/build -ldlib \
            -L/usr/lib/x86_64-linux-gnu -lboost_system -lboost_filesystem \                         # boost
            -L/usr/local/lib -lopencv_calib3d -lopencv_contrib -lopencv_core \
                -lopencv_features2d -lopencv_flann -lopencv_gpu \
                -lopencv_highgui -lopencv_imgproc -lopencv_legacy \
                -lopencv_ml -lopencv_objdetect \
                -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres \
                -lopencv_video -lopencv_videostab \                                                 # opencv
            -L/usr/lib/x86_64-linux-gnu -lassimp                                                    # assimp
}

SOURCES += \
    main.cpp \
    GUI/MainWindow.cpp \
    Face/FitModel.cpp \
    GUI/PicWidget.cpp \
    GUI/ModelViewer.cpp \
    GLOBAL_VAR.cpp \
    Face/FaceDetection.cpp \
    objViewer/Camera3D.cpp \
    objViewer/CustomMesh.cpp \
    objViewer/CustomTexture.cpp \
    objViewer/Input.cpp \
    #objViewer/main.cpp \
    objViewer/objLoader.cpp \
    objViewer/OpenGLWidget.cpp \
    objViewer/Transform3D.cpp \
    objViewer/Vertex.cpp

HEADERS += \
    GUI/MainWindow.h \
    Face/FitModel.h \
    GUI/PicWidget.h \
    GUI/ModelViewer.h \
    GLOBAL_VAR.h \
    Face/FaceDetection.h \
    eos/core/Landmark.hpp \
    eos/core/LandmarkMapper.hpp \
    eos/fitting/affine_camera_estimation.hpp \
    eos/fitting/linear_shape_fitting.hpp \
    eos/fitting/nonlinear_camera_estimation.hpp \
    eos/fitting/detail/nonlinear_camera_estimation_detail.hpp \
    eos/morphablemodel/MorphableModel.hpp \
    eos/morphablemodel/PcaModel.hpp \
    eos/morphablemodel/io/cvssp.hpp \
    eos/morphablemodel/io/mat_cerealisation.hpp \
    eos/render/Mesh.hpp \
    eos/render/render.hpp \
    eos/render/render_affine.hpp \
    eos/render/texture_extraction.hpp \
    eos/render/utils.hpp \
    eos/render/detail/render_affine_detail.hpp \
    eos/render/detail/render_detail.hpp \
    eos/render/detail/texture_extraction_detail.hpp \
    objViewer/Camera3D.h \
    objViewer/CustomMesh.h \
    objViewer/CustomTexture.h \
    objViewer/Input.h \
    objViewer/objLoader.h \
    objViewer/OpenGLWidget.h \
    objViewer/Transform3D.h \
    objViewer/Vertex.h

FORMS += \
    GUI/MainWindow.ui \
    GUI/PicWidget.ui \
    GUI/ModelViewer.ui

RESOURCES += \
    resource.qrc \
    objViewer/objviewer.qrc

TRANSLATIONS = cn_start.ts


