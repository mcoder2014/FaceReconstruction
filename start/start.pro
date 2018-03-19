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
# DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = ../bin        # 生成的文件所在的目录
MOC_DIR = ../moc        # Q_OBJECT转换后的类
RCC_DIR = ../rcc        # .qrc文件转换后的位置
OBJECTS_DIR = ../tmp    # 编译中间文件的位置

INCLUDEPATH += $$PWD \  # 自身
               # E:/libs/opencv2.4.11-minGW5.3/opencv-2.4.11/include \ # opencv
               E:/openCV-2.4.11/build/install/include \
               E:/libs/boost-1.50-minGW-5.3/build-mingw-static/include/boost-1_50 \ # boost
               # E:/boost1.50/boost_1_50_0 \
               $$PWD/../libs \                                      # dlib cereal glm
               $$PWD/../libs/eigen-eigen-b9cd8366d4e8 \             # eigen
               E:/libs/assimp-3.3-minGW-5.3/assimp-3.3/include \
               # E:/libs/assimp-3.3/include \      # assimp的Include文件夹的路径


LIBS += -L$$PWD/../libs -ldlib \                                # dlib 静态文件
#E:\openCV-2.4.11\build\lib\Release\opencv_videostab2411.lib \
#E:\openCV-2.4.11\build\lib\Release\opencv_ts2411.lib   \
#E:\openCV-2.4.11\build\lib\Release\opencv_superres2411.lib  \
#E:\openCV-2.4.11\build\lib\Release\opencv_stitching2411.lib \
#E:\openCV-2.4.11\build\lib\Release\opencv_contrib2411.lib   \
#E:\boost1.50\build\lib\boost_system-vc140-mt-1_50.lib   \
#E:\boost1.50\build\lib\boost_filesystem-vc140-mt-1_50.lib   \
#E:\boost1.50\build\lib\boost_program_options-vc140-mt-1_50.lib  \
#E:\openCV-2.4.11\build\lib\Release\opencv_nonfree2411.lib   \
#E:\openCV-2.4.11\build\lib\Release\opencv_ocl2411.lib   \
#E:\openCV-2.4.11\build\lib\Release\opencv_gpu2411.lib   \
#E:\openCV-2.4.11\build\lib\Release\opencv_photo2411.lib \
#E:\openCV-2.4.11\build\lib\Release\opencv_objdetect2411.lib \
#E:\openCV-2.4.11\build\lib\Release\opencv_legacy2411.lib    \
#E:\openCV-2.4.11\build\lib\Release\opencv_video2411.lib \
#E:\openCV-2.4.11\build\lib\Release\opencv_ml2411.lib    \
#E:\openCV-2.4.11\build\lib\Release\opencv_calib3d2411.lib \
#E:\openCV-2.4.11\build\lib\Release\opencv_features2d2411.lib    \
#E:\openCV-2.4.11\build\lib\Release\opencv_highgui2411.lib   \
#E:\openCV-2.4.11\build\lib\Release\opencv_imgproc2411.lib \
#E:\openCV-2.4.11\build\lib\Release\opencv_flann2411.lib \
#E:\openCV-2.4.11\build\lib\Release\opencv_core2411.lib  \
#-LE:/libs/assimp-3.3/build/code/Release -lassimp-vc140-mt \  # assimp的lib文件
    # boost
    #../../../libs/boost-1.50-minGW-5.3/build/lib/libboost_filesystem-mgw53-mt-sd-1_50.a \
    #../../../libs/boost-1.50-minGW-5.3/build/lib/libboost_program_options-mgw53-mt-sd-1_50.a \
    #../../../libs/boost-1.50-minGW-5.3/build/lib/libboost_system-mgw53-mt-sd-1_50.a \
    #-L../../../libs/boost-1.50-minGW-5.3/build-mingw-dll/lib -lboost_system -lboost_filesystem -lboost_system \
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


