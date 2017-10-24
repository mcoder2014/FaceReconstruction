#include "FaceDetection.h"
#include "GLOBAL_VAR.h"
#include <QVector>
#include <QDebug>
#include <string>
#include <vector>
#include <QColor>

#define DLIB_PNG_SUPPORT
#define DLIB_JPEG_SUPPORT

using cv::Mat;
using cv::Vec2f;
using cv::Vec3f;
using cv::Vec4f;

FaceDetection *FaceDetection::m_instance = NULL;

FaceDetection::FaceDetection()
{

    // And we also need a shape_predictor.  This is the tool that will predict face
    // landmark positions given an image and face bounding box.  Here we are just
    // loading the model from the shape_predictor_68_face_landmarks.dat file you gave
    // as a command line argument.
    GLOBAL_VAR* global = GLOBAL_VAR::getInstance();

    deserialize(global->getLandmarkDat().toStdString()) >> sp;
}

///
/// \brief FaceDetection::landmark
/// \param filePath
/// \return
/// 不显示图像，直接计算出标记的区域
///
FaceDetection *FaceDetection::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new FaceDetection();
    }

    return m_instance;
}

LandmarkCollection<cv::Vec2f> *FaceDetection::landmark(QString filePath)
{
    // We need a face detector.  We will use this to get bounding boxes for
    // each face in an image.
    frontal_face_detector detector = get_frontal_face_detector();

    qDebug() << "shape_predictor init finished";
    qDebug() << "image path" << filePath;
//    QString changedPath = filePath.replace("/","\\");
//    qDebug() << "changed image path" << filePath;

    // 加载图片-dlib直接load image函数出错，自己写了个转换
    array2d<rgb_pixel> *img = NULL;     // 图片
    QImage image(filePath);
    img = this->qimageToArray2d(image);

    // 不需要放大图片
    std::vector<rectangle> dets = detector(*img);        // 检测人脸

    //  只检测处理第一张人脸
    full_object_detection shape = sp(*img, dets[0]);     // 检测

    return this->buildLandMarks(shape);

}

///
/// \brief FaceDetection::landmark
/// \param image
/// \return
///
LandmarkCollection<cv::Vec2f> *FaceDetection::landmark(QImage image)
{
    // We need a face detector.  We will use this to get bounding boxes for
    // each face in an image.
    frontal_face_detector detector = get_frontal_face_detector();
    // And we also need a shape_predictor.  This is the tool that will predict face
    // landmark positions given an image and face bounding box.  Here we are just
    // loading the model from the shape_predictor_68_face_landmarks.dat file you gave
    // as a command line argument.
//    shape_predictor sp;
//    GLOBAL_VAR* global = GLOBAL_VAR::getInstance();

//    deserialize(global->getLandmarkDat().toStdString()) >> sp;

    // 加载图片-dlib直接load image函数出错，自己写了个转换
    array2d<rgb_pixel> *img = NULL;     // 图片
    img = this->qimageToArray2d(image);
    // 不需要放大图片
    std::vector<rectangle> dets = detector(*img);        // 检测人脸
    //  只检测处理第一张人脸
    full_object_detection shape = sp(*img, dets[0]);     // 检测
    return this->buildLandMarks(shape);
}

LandmarkCollection<cv::Vec2f> *FaceDetection::buildLandMarks(full_object_detection shape)
{
    using cv::Vec2f;
    LandmarkCollection<Vec2f> *landmarks = new LandmarkCollection<Vec2f>();
    landmarks->reserve(68);     // 初始化68个特征点

    // 处理每一个点
    for(int i = 0; i < shape.num_parts(); i++)
    {
        Landmark<Vec2f> landmark;
        landmark.name = std::to_string(i+1);

        dlib::point pt_save = shape.part(i);
        landmark.coordinates[0] = pt_save.x();
        landmark.coordinates[1] = pt_save.y();

        landmark.coordinates[0] -= 1;
        landmark.coordinates[1] -= 1;

        qDebug() << "Point" << i+1
                 << " " << pt_save.x() << " " << pt_save.y();

        landmarks->emplace_back(landmark);
    }

    qDebug() << "landmark.size" << landmarks->size();

    return landmarks;

}

///
/// \brief FaceDetection::qimageToArray2d
/// \param image
/// \return
/// 将QImage 类型转换为 array2d
///
array2d<rgb_pixel> *FaceDetection::qimageToArray2d(QImage image)
{
    int width = image.width();
    int height = image.height();

    array2d<rgb_pixel> *a_image = new array2d<rgb_pixel>(height, width);      // 做转换
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            rgb_pixel pixel;
            QRgb color = image.pixel(x,y);
            pixel.red = qRed(color);
            pixel.blue = qBlue(color);
            pixel.green = qGreen(color);

            (*a_image)[y][x] = pixel;
        }
    }

    return a_image;

}

