#include "FaccDetection.h"
#include "GLOBAL_VAR.h"

FaccDetection::FaccDetection()
{

}

///
/// \brief FaccDetection::landmark
/// \param filePath
/// \return
/// 不显示图像，直接计算出标记的区域
///
Landmark<cv::Vec2f> FaccDetection::landmark(QString filePath)
{
    // We need a face detector.  We will use this to get bounding boxes for
    // each face in an image.
    frontal_face_detector detector = get_frontal_face_detector();
    // And we also need a shape_predictor.  This is the tool that will predict face
    // landmark positions given an image and face bounding box.  Here we are just
    // loading the model from the shape_predictor_68_face_landmarks.dat file you gave
    // as a command line argument.
    shape_predictor sp;
    GLOBAL_VAR* global = GLOBAL_VAR::getInstance();

    deserialize(global->getLandmarkDat()) >> sp;

    // 解读图片
    array2d<rgb_pixel> img;     // 图片
    load_image(img, filePath);  // 加载图片



}

