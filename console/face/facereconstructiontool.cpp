#include "facereconstructiontool.h"
#include <QDir>
#include <QFile>
#include <QDebug>

#include <eos/core/LandmarkMapper.hpp>
#include <eos/fitting/nonlinear_camera_estimation.hpp>
#include <eos/fitting/linear_shape_fitting.hpp>
#include <eos/render/utils.hpp>
#include <eos/render/texture_extraction.hpp>

#include <GLOBAL_VAR.h>

using cv::Vec2f;
using cv::Vec3f;
using cv::Vec4f;
using std::cout;
using std::endl;
using std::vector;
using std::string;

FaceReconstructionTool::FaceReconstructionTool(QObject *parent) : QObject(parent)
{
    // 初始化 dlib 人脸识别相关
    GLOBAL_VAR *global = GLOBAL_VAR::getInstance();
    deserialize(global->getLandmarkDat().toStdString()) >> sp;

    // 初始化 FitModel
    QString mappingsfile = global->getMappingsFile();

    if(mappingsfile.size() == 0)
    {
        this->landmark_mapper = new LandmarkMapper();
    }
    else
    {
        this->landmark_mapper = new LandmarkMapper(mappingsfile.toStdString());
    }
    this->fileName = "model";
}

LandmarkCollection<cv::Vec2f> *FaceReconstructionTool::landmark(QString filePath)
{
    // We need a face detector.  We will use this to get bounding boxes for
    // each face in an image.
    frontal_face_detector detector = get_frontal_face_detector();

    qDebug() << "shape_predictor init finished";
    qDebug() << "image path" << filePath;

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
/// \brief FaceReconstructionTool::landmark
/// \param image
/// \return
///
LandmarkCollection<cv::Vec2f> *FaceReconstructionTool::landmark(QImage image)
{
    // We need a face detector.  We will use this to get bounding boxes for
    // each face in an image.
    frontal_face_detector detector = get_frontal_face_detector();
    // And we also need a shape_predictor.  This is the tool that will predict face
    // landmark positions given an image and face bounding box.  Here we are just
    // loading the model from the shape_predictor_68_face_landmarks.dat file you gave
    // as a command line argument.

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
/// \brief FaceReconstructionTool::cvMat2QImage
/// \param mat
/// \return
///
QImage FaceReconstructionTool::cvMat2QImage(const cv::Mat &mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

///
/// \brief FaceReconstructionTool::QImage2cvMat
/// \param image
/// \return
///
cv::Mat FaceReconstructionTool::QImage2cvMat(QImage image)
{
    cv::Mat mat;

    qDebug() << image.format();
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(
                    image.height(),
                    image.width(),
                    CV_8UC4,
                    (void*)image.constBits(),
                    image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(
                    image.height(),
                    image.width(),
                    CV_8UC3,
                    (void*)image.constBits(),
                    image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(
                    image.height(),
                    image.width(),
                    CV_8UC1,
                    (void*)image.constBits(),
                    image.bytesPerLine());
        break;
    }
    return mat;
}

///
/// \brief FaceReconstructionTool::Reconstruction
/// \param image_path   The full path of image e.g. E:/test.jpg
/// \param save_path    The directory of path to save the model, e.g. E:/test/
///     The model file: model.obj, model.iso.jpg, model.mtl
/// \return 1 for success , 0 for error, -1 for load MorphableModel failed.
///
int FaceReconstructionTool::Reconstruction(QString image_path, QString save_path)
{

    // 用QImage转换过来的Mat执行会出现错误……而且并不清楚是为啥
    Mat image = cv::imread(image_path.toStdString());

    QImage qimage = this->cvMat2QImage(image);      // 转换为 QImage;

    // 检测特征点
    LandmarkCollection<cv::Vec2f> *landmarks = this->landmark(qimage);     // 先检测特征点

    // 加载可形变模型
    GLOBAL_VAR *global = GLOBAL_VAR::getInstance();
    morphablemodel::MorphableModel morphable_model;
    try
    {
        morphable_model = morphablemodel::load_model(
                    global->getMorphableModel().toStdString());
    }
    catch (const std::runtime_error& e)
    {
        cout << "Error loading the Morphable Model: " << e.what() << endl;
        return -1;
    }

    // These will be the final 2D and 3D points used for the fitting:
    std::vector<cv::Vec4f> model_points; // the points in the 3D shape model
    std::vector<int> vertex_indices; // their vertex indices
    std::vector<cv::Vec2f> image_points; // the corresponding 2D landmark points
    // Sub-select all the landmarks which we have a mapping for (i.e. that are defined in the 3DMM):

    for (int i = 0; i < landmarks->size(); ++i)
    {
        auto converted_name = landmark_mapper->convert((*landmarks)[i].name);
        if (!converted_name)
        {
            // no mapping defined for the current landmark
            continue;
        }

        // 转换为int值
        int vertex_idx = std::stoi(converted_name.get());

        Vec4f vertex = morphable_model.get_shape_model().get_mean_at_point(vertex_idx);
        model_points.emplace_back(vertex);
        vertex_indices.emplace_back(vertex_idx);
        image_points.emplace_back((*landmarks)[i].coordinates);
    }

    /// Debug info
    std::cout << "model_point = " << endl;
    std::cout << "count model points:" << morphable_model.get_shape_model().get_mean().rows / 3.0 << endl;
    for (int i = 0; i < landmarks->size(); ++i) {
        //         3d points                                                    2d points
        std::cout << model_points[i] << "\t"
                  << vertex_indices[i] << "\t"
                  << image_points[i] << endl;
    }

    // Estimate the camera (pose) from the 2D - 3D point correspondences
    fitting::OrthographicRenderingParameters rendering_params
            = fitting::estimate_orthographic_camera(
                image_points,
                model_points,
                image.cols,
                image.rows);

    /// Debug info
    std::cout << "rendering_params = ";
    std::cout << rendering_params.r_x << " "
              << rendering_params.r_y << " "
              << rendering_params.r_z << " "
              << rendering_params.t_x << " "
              << rendering_params.t_y << endl;

    std::cout << "frustum : "
              << rendering_params.frustum.b << " "
              << rendering_params.frustum.l << " "
              << rendering_params.frustum.r << " "
              << rendering_params.frustum.t << endl;

    Mat affine_from_ortho = get_3x4_affine_camera_matrix(
                rendering_params,
                image.cols,
                image.rows);

    // Estimate the shape coefficients by fitting the shape to the landmarks:
    // bin模型           投影矩阵              图片二维点;

    std::vector<float> fitted_coeffs = fitting::fit_shape_to_landmarks_linear(
                morphable_model,
                affine_from_ortho,
                image_points,
                vertex_indices);

    // Debug Info
    cout << "affine_from_ortho = " << endl;
    cout << affine_from_ortho << endl;

    // The 3D head pose can be recovered as follows:
    float xaw_angle = glm::degrees(rendering_params.r_x);
    float yaw_angle = glm::degrees(rendering_params.r_y);
    float zaw_angle = glm::degrees(rendering_params.r_z);
    cout << "x_y_z_angle = ";
    cout << xaw_angle << "\t" << yaw_angle << "\t" << zaw_angle << endl;
    // and similarly for pitch (r_x) and roll (r_z).


    cout << "size = " << fitted_coeffs.size() << endl;
    for (int i = 0; i < fitted_coeffs.size(); ++i)
        cout << fitted_coeffs[i] << endl;

    // Obtain the full mesh with the estimated coefficients:
    render::Mesh mesh = morphable_model.draw_sample(
                fitted_coeffs,
                std::vector<float>());
    qDebug() << "draw sample success";

    // Extract the texture from the image using given mesh and camera parameters:
    Mat isomap = render::extract_texture(
                mesh,
                affine_from_ortho,
                image);
    qDebug() << "extract texture sucess!";

    this->isoMap = this->cvMat2QImage(isomap);

    qDebug() << "cvMat2QImage success";

    // Save the mesh as textured obj:
    this->outputPath = save_path;       // 设置保存路径
    QString objPath = this->outputPath + "/" + this->fileName + ".obj";
    QString isoMapPath = this->outputPath + "/" + this->fileName + ".isomap.png";

    qDebug() << "objPath: " << objPath
             << "isoMapPath: "<< isoMapPath;

    render::write_textured_obj(
                mesh,
                objPath.toStdString());         // 保存 obj 模型

    QImage qisomap = this->cvMat2QImage(isomap);
    qisomap.save(isoMapPath);                   //  保存 isomap

    return 1;
}

///
/// \brief FaceReconstructionTool::buildLandMarks
/// \param shape
/// \return
///
LandmarkCollection<cv::Vec2f> *FaceReconstructionTool::buildLandMarks(full_object_detection shape)
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
/// \brief FaceReconstructionTool::qimageToArray2d
/// \param image
/// \return
/// 将 QImage 类型转换为 array2d 类型
array2d<rgb_pixel> *FaceReconstructionTool::qimageToArray2d(QImage image)
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
