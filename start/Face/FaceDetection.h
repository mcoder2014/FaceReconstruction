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
#include <QString>
#include <QImage>

#define DLIB_PNG_SUPPORT
#define DLIB_JPEG_SUPPORT

using namespace dlib;
using namespace std;
using namespace eos;

using eos::core::Landmark;
using eos::core::LandmarkCollection;

using cv::Mat;
using cv::Vec2f;
using cv::Vec3f;
using cv::Vec4f;
using std::vector;
using std::string;

class FaceDetection
{
public:
    FaceDetection();
    LandmarkCollection<cv::Vec2f> *landmark(QString filePath);
    LandmarkCollection<cv::Vec2f> *landmark(QImage image);

private:
    LandmarkCollection<cv::Vec2f> *buildLandMarks(full_object_detection shape);
    array2d<rgb_pixel> *qimageToArray2d(QImage image);
};

#endif // FaceDetection_H
