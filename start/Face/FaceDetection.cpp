#include "FaceDetection.h"
#include "GLOBAL_VAR.h"
#include <QVector>
#include <QDebug>
#include <string>
#include <vector>
#include <QColor>
#include <QPainter>
#include <QPainterPath>

#define DLIB_PNG_SUPPORT
#define DLIB_JPEG_SUPPORT

using cv::Mat;
using cv::Vec2f;
using cv::Vec3f;
using cv::Vec4f;

FaceDetection *FaceDetection::m_instance = NULL;

FaceDetection::FaceDetection()
    :QObject()
{

    // And we also need a shape_predictor.  This is the tool that will predict face
    // landmark positions given an image and face bounding box.  Here we are just
    // loading the model from the shape_predictor_68_face_landmarks.dat file you gave
    // as a command line argument.
//    GLOBAL_VAR* global = GLOBAL_VAR::getInstance();
//    deserialize(global->getLandmarkDat().toStdString()) >> sp;
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

///
/// \brief FaceDetection::landmarkAllFace
/// \param image
/// \return
///
QVector<LandmarkCollection<cv::Vec2f> *> *FaceDetection::landmarkAllFace(
        QImage image)
{
    // 建立容器
    QVector<LandmarkCollection<cv::Vec2f> *> *vector =
            new QVector<LandmarkCollection<cv::Vec2f> *>();

    // 人脸检测器
    frontal_face_detector detector = get_frontal_face_detector();

    array2d<rgb_pixel> *img = NULL;     // 加载图片
    img = this->qimageToArray2d(image);

    std::vector<rectangle> dets = detector(*img);        // 检测人脸

    // 对每个检测到的人脸都做处理
    for(int i = 0; i < dets.size(); i++)
    {
        full_object_detection shape = sp(*img, dets[i]);     // 检测
        vector->push_back(
                    this->buildLandMarks(shape));
    }

    return vector;
}

///
/// \brief FaceDetection::drawLandMark
/// \param marks
/// \param image
/// \return
///
QImage FaceDetection::drawLandMark(
        QVector<LandmarkCollection<cv::Vec2f> *> *marks,
        QImage &image)
{
    QImage markedImage(image);          // 复制一份
    QPainter painter(&markedImage);    // 新建一个画笔

    QPen pen;
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::green);
    pen.setBrush(brush);
    pen.setWidth(5);    // 线条粗细
    painter.setPen(pen);

    for(int face = 0; face < marks->size(); face ++)
    {
        // 依次处理每张脸
        LandmarkCollection<cv::Vec2f> *landmarks = marks->operator [](face);

        // 轮廓
        QPainterPath border_path;
        border_path.moveTo(
                    (*landmarks)[0].coordinates[0],
                (*landmarks)[0].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[1].coordinates[0],
                (*landmarks)[1].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[2].coordinates[0],
                (*landmarks)[2].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[3].coordinates[0],
                (*landmarks)[3].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[4].coordinates[0],
                (*landmarks)[4].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[5].coordinates[0],
                (*landmarks)[5].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[6].coordinates[0],
                (*landmarks)[6].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[7].coordinates[0],
                (*landmarks)[7].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[8].coordinates[0],
                (*landmarks)[8].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[9].coordinates[0],
                (*landmarks)[9].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[10].coordinates[0],
                (*landmarks)[10].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[11].coordinates[0],
                (*landmarks)[11].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[12].coordinates[0],
                (*landmarks)[12].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[13].coordinates[0],
                (*landmarks)[13].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[14].coordinates[0],
                (*landmarks)[15].coordinates[1]);
        border_path.lineTo(
                    (*landmarks)[16].coordinates[0],
                (*landmarks)[16].coordinates[1]);

        painter.drawPath(border_path);

        // 右眉
        QPainterPath rightEyeBrow;
        rightEyeBrow.moveTo(
                    (*landmarks)[17].coordinates[0],
                (*landmarks)[17].coordinates[1]);
        rightEyeBrow.lineTo(
                    (*landmarks)[18].coordinates[0],
                (*landmarks)[18].coordinates[1]);
        rightEyeBrow.lineTo(
                    (*landmarks)[19].coordinates[0],
                (*landmarks)[19].coordinates[1]);
        rightEyeBrow.lineTo(
                    (*landmarks)[20].coordinates[0],
                (*landmarks)[20].coordinates[1]);
        rightEyeBrow.lineTo(
                    (*landmarks)[21].coordinates[0],
                (*landmarks)[21].coordinates[1]);

        painter.drawPath(rightEyeBrow);

        // 左眉
        QPainterPath leftEyeBrow;
        leftEyeBrow.moveTo(
                    (*landmarks)[22].coordinates[0],
                (*landmarks)[22].coordinates[1]);

        leftEyeBrow.lineTo(
                    (*landmarks)[23].coordinates[0],
                (*landmarks)[23].coordinates[1]);
        leftEyeBrow.lineTo(
                    (*landmarks)[24].coordinates[0],
                (*landmarks)[24].coordinates[1]);
        leftEyeBrow.lineTo(
                    (*landmarks)[25].coordinates[0],
                (*landmarks)[25].coordinates[1]);
        leftEyeBrow.lineTo(
                    (*landmarks)[26].coordinates[0],
                (*landmarks)[26].coordinates[1]);

        painter.drawPath(leftEyeBrow);

        // 鼻梁
        QPainterPath brigeOfNose;
        brigeOfNose.moveTo(
                    (*landmarks)[27].coordinates[0],
                (*landmarks)[27].coordinates[1]);
        brigeOfNose.lineTo(
                    (*landmarks)[28].coordinates[0],
                (*landmarks)[28].coordinates[1]);
        brigeOfNose.lineTo(
                    (*landmarks)[29].coordinates[0],
                (*landmarks)[29].coordinates[1]);
        brigeOfNose.lineTo(
                    (*landmarks)[30].coordinates[0],
                (*landmarks)[30].coordinates[1]);

        painter.drawPath(brigeOfNose);

        // 鼻子
        QPainterPath bottomOfNose;
        bottomOfNose.moveTo(
                    (*landmarks)[31].coordinates[0],
                (*landmarks)[31].coordinates[1]);
        bottomOfNose.lineTo(
                    (*landmarks)[32].coordinates[0],
                (*landmarks)[32].coordinates[1]);
        bottomOfNose.lineTo(
                    (*landmarks)[33].coordinates[0],
                (*landmarks)[33].coordinates[1]);
        bottomOfNose.lineTo(
                    (*landmarks)[34].coordinates[0],
                (*landmarks)[34].coordinates[1]);
        bottomOfNose.lineTo(
                    (*landmarks)[35].coordinates[0],
                (*landmarks)[35].coordinates[1]);

        //右眼
        QPainterPath rightEye;
        rightEye.moveTo(
                    (*landmarks)[36].coordinates[0],
                (*landmarks)[36].coordinates[1]);
        rightEye.lineTo(
                    (*landmarks)[37].coordinates[0],
                (*landmarks)[37].coordinates[1]);
        rightEye.lineTo(
                    (*landmarks)[38].coordinates[0],
                (*landmarks)[38].coordinates[1]);
        rightEye.lineTo(
                    (*landmarks)[39].coordinates[0],
                (*landmarks)[39].coordinates[1]);
        rightEye.lineTo(
                    (*landmarks)[40].coordinates[0],
                (*landmarks)[40].coordinates[1]);
        rightEye.lineTo(
                    (*landmarks)[41].coordinates[0],
                (*landmarks)[41].coordinates[1]);
        rightEye.lineTo(
                    (*landmarks)[36].coordinates[0],
                (*landmarks)[36].coordinates[1]);
        painter.drawPath(rightEye);

        // 左眼
        QPainterPath leftEye;
        leftEye.moveTo(
                    (*landmarks)[42].coordinates[0],
                (*landmarks)[42].coordinates[1]);
        leftEye.lineTo(
                    (*landmarks)[43].coordinates[0],
                (*landmarks)[43].coordinates[1]);
        leftEye.lineTo(
                    (*landmarks)[44].coordinates[0],
                (*landmarks)[44].coordinates[1]);
        leftEye.lineTo(
                    (*landmarks)[45].coordinates[0],
                (*landmarks)[45].coordinates[1]);
        leftEye.lineTo(
                    (*landmarks)[46].coordinates[0],
                (*landmarks)[46].coordinates[1]);
        leftEye.lineTo(
                    (*landmarks)[47].coordinates[0],
                (*landmarks)[47].coordinates[1]);
        leftEye.lineTo(
                    (*landmarks)[42].coordinates[0],
                (*landmarks)[42].coordinates[1]);
        painter.drawPath(leftEye);

        // 嘴巴
        QPainterPath mouth;
        mouth.moveTo(
                    (*landmarks)[48].coordinates[0],
                (*landmarks)[48].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[49].coordinates[0],
                (*landmarks)[49].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[50].coordinates[0],
                (*landmarks)[50].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[51].coordinates[0],
                (*landmarks)[51].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[52].coordinates[0],
                (*landmarks)[52].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[53].coordinates[0],
                (*landmarks)[53].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[54].coordinates[0],
                (*landmarks)[54].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[55].coordinates[0],
                (*landmarks)[55].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[56].coordinates[0],
                (*landmarks)[56].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[57].coordinates[0],
                (*landmarks)[57].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[58].coordinates[0],
                (*landmarks)[58].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[59].coordinates[0],
                (*landmarks)[59].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[48].coordinates[0],
                (*landmarks)[48].coordinates[1]);

        mouth.moveTo(
                    (*landmarks)[60].coordinates[0],
                (*landmarks)[60].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[61].coordinates[0],
                (*landmarks)[61].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[62].coordinates[0],
                (*landmarks)[62].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[63].coordinates[0],
                (*landmarks)[63].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[64].coordinates[0],
                (*landmarks)[64].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[65].coordinates[0],
                (*landmarks)[65].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[66].coordinates[0],
                (*landmarks)[66].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[67].coordinates[0],
                (*landmarks)[67].coordinates[1]);
        mouth.lineTo(
                    (*landmarks)[60].coordinates[0],
                (*landmarks)[60].coordinates[1]);
        painter.drawPath(mouth);

    }


    return markedImage;
}

///
/// \brief FaceDetection::thread_landmarkAllFace
/// \param image
///
void FaceDetection::thread_landmarkAllFace(QImage image)
{
    // 建立容器
    QVector<LandmarkCollection<cv::Vec2f> *> *vector =
            new QVector<LandmarkCollection<cv::Vec2f> *>();

    // 人脸检测器
    frontal_face_detector detector = get_frontal_face_detector();

    emit this->signals_progressValue(30);

    array2d<rgb_pixel> *img = NULL;     // 加载图片
    img = this->qimageToArray2d(image);

    emit this->signals_progressValue(50);

    std::vector<rectangle> dets = detector(*img);        // 检测人脸

    emit this->signals_progressValue(70);

    // 对每个检测到的人脸都做处理
    for(int i = 0; i < dets.size(); i++)
    {
        full_object_detection shape = sp(*img, dets[i]);     // 检测
        vector->push_back(
                    this->buildLandMarks(shape));
        emit this->signals_progressValue(70 + i * 30.0 / dets.size());
    }

    emit this->signals_progressValue(100);
    emit this->signals_landmarkAll(vector);
    emit this->signals_finished();
}

void FaceDetection::thread_init()
{
    GLOBAL_VAR* global = GLOBAL_VAR::getInstance();
    deserialize(global->getLandmarkDat().toStdString()) >> sp;

//    emit this->signals_msg(
//                tr("FaceDetection Finished"),
//                tr("FaceDetection finished"));
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

