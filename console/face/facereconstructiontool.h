#ifndef FACERECONSTRUCTIONTOOL_H
#define FACERECONSTRUCTIONTOOL_H

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <iostream>
#include <eos/core/Landmark.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QObject>
#include <QString>
#include <QImage>
#include <QVector>

using namespace dlib;
using namespace std;
using namespace eos;

using eos::core::Landmark;
using eos::core::LandmarkCollection;

using std::vector;
using std::string;
using namespace eos;
using eos::core::Landmark;
using eos::core::LandmarkCollection;
using cv::Mat;

namespace eos
{
    namespace core
    {
        class LandmarkMapper;
    }
}
using eos::core::LandmarkMapper;

class FaceReconstructionTool : public QObject
{
    Q_OBJECT
public:
    explicit FaceReconstructionTool(QObject *parent = nullptr);

    LandmarkCollection<cv::Vec2f> *landmark(QString filePath);          // 标记landMark-第一张人脸
    LandmarkCollection<cv::Vec2f> *landmark(QImage image);              // 标记landMark-第一张人脸
    void fitmodel(QString image_path);          // 生成人脸模型

    QImage cvMat2QImage(const cv::Mat& mat);    // cvMat转换为QImage
    cv::Mat QImage2cvMat(QImage image);         // QImage 转化为cvMat

private:
    LandmarkCollection<cv::Vec2f> *buildLandMarks(full_object_detection shape);
    array2d<rgb_pixel> *qimageToArray2d(QImage image);

    // 暂时还不知道怎么解决此程序只执行一次的问题。
    shape_predictor sp;                 // 因为每次识别都要加载几十兆的素材
    LandmarkMapper *landmark_mapper;    //LandMark_mapper

    QString fileName;                   // 保存出来的文件名称
};

#endif // FACERECONSTRUCTIONTOOL_H
