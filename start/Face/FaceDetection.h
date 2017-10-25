#ifndef FaceDetection_H
#define FaceDetection_H

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

class FaceDetection:public QObject
{
    Q_OBJECT
public:
    static FaceDetection *getInstance();
    LandmarkCollection<cv::Vec2f> *landmark(QString filePath);          // 标记landMark-第一张人脸
    LandmarkCollection<cv::Vec2f> *landmark(QImage image);              // 标记landMark-第一张人脸
    QVector<LandmarkCollection<cv::Vec2f> *> *landmarkAllFace(QImage image);    // 标记landMark-所有人脸  
    QImage drawLandMark(
            QVector<LandmarkCollection<cv::Vec2f> *> * marks,
            QImage& image);     // 结合计算出的结果，在图片上标出人脸
public slots:
    void thread_landmarkAllFace(QImage image);

private:
    static FaceDetection* m_instance;
    FaceDetection();

    shape_predictor sp;        // 因为每次识别都要加载几十兆的素材

    LandmarkCollection<cv::Vec2f> *buildLandMarks(full_object_detection shape);
    array2d<rgb_pixel> *qimageToArray2d(QImage image);

signals:
    void signals_landmarkAll(QVector<LandmarkCollection<cv::Vec2f> *> *vector);
    void signals_finished();                // 计算完成
    void signals_progressValue(int value);  // 更新进度条
};

#endif // FaceDetection_H
