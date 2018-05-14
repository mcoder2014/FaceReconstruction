QT += core gui

CONFIG += c++11

TARGET = cfaceRec
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
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
            -L/mnt/e/libs/dlibs-18.17-minGW-5.3/dlib-18.17/build -ldlib \            # dlib
            -L/usr/lib/x86_64-linux-gnu -lboost_system -lboost_filesystem \                         # boost
            -L/usr/local/lib -lopencv_calib3d -lopencv_contrib -lopencv_core \
                -lopencv_features2d -lopencv_flann -lopencv_gpu \
                -lopencv_highgui -lopencv_imgproc -lopencv_legacy \
                -lopencv_ml -lopencv_objdetect \
                -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres \
                -lopencv_video -lopencv_videostab \                                                 # opencv
            -L/usr/lib/x86_64-linux-gnu -lassimp                                                    # assimp
}

DESTDIR = ../bin       # 生成的文件所在的目录
MOC_DIR = ./moc        # Q_OBJECT转换后的类
RCC_DIR = ./rcc        # .qrc文件转换后的位置
OBJECTS_DIR = ./tmp    # 编译中间文件的位置
UI_DIR = ./ui          # ui_xxx 文件存放位置

SOURCES += main.cpp \
    face/facereconstructiontool.cpp \
    ../start/GLOBAL_VAR.cpp

HEADERS += \
    face/facereconstructiontool.h \
    ../start/GLOBAL_VAR.h \
    ../start/eos/core/Landmark.hpp \
    ../start/eos/core/LandmarkMapper.hpp \
    ../start/eos/fitting/affine_camera_estimation.hpp \
    ../start/eos/fitting/linear_shape_fitting.hpp \
    ../start/eos/fitting/nonlinear_camera_estimation.hpp \
    ../start/eos/fitting/detail/nonlinear_camera_estimation_detail.hpp \
    ../start/eos/morphablemodel/MorphableModel.hpp \
    ../start/eos/morphablemodel/PcaModel.hpp \
    ../start/eos/morphablemodel/io/cvssp.hpp \
    ../start/eos/morphablemodel/io/mat_cerealisation.hpp \
    ../start/eos/render/Mesh.hpp \
    ../start/eos/render/render.hpp \
    ../start/eos/render/render_affine.hpp \
    ../start/eos/render/texture_extraction.hpp \
    ../start/eos/render/utils.hpp \
    ../start/eos/render/detail/render_affine_detail.hpp \
    ../start/eos/render/detail/render_detail.hpp \
    ../start/eos/render/detail/texture_extraction_detail.hpp
