#include "FitModel.h"
#include <QDebug>
#include "FaceDetection.h"
#include "GLOBAL_VAR.h"
#include <QDir>
#include <QFile>

#include <eos/core/LandmarkMapper.hpp>
#include <eos/fitting/nonlinear_camera_estimation.hpp>
#include <eos/fitting/linear_shape_fitting.hpp>
#include <eos/render/utils.hpp>
#include <eos/render/texture_extraction.hpp>

using cv::Vec2f;
using cv::Vec3f;
using cv::Vec4f;
using std::cout;
using std::endl;
using std::vector;
using std::string;

FitModel *FitModel::m_instance = NULL;

FitModel::FitModel()
    :QObject(0)
{
    this->outputPath = QDir::homePath();    // 默认文件路径
    this->fileName = QString("model");      // 默认文件名

    GLOBAL_VAR *global = GLOBAL_VAR::getInstance();
    QString mappingsfile = global->getMappingsFile();       // mapping file的路径

    if(mappingsfile.size() == 0)
    {
        this->landmark_mapper = new LandmarkMapper();
    }
    else
    {
        this->landmark_mapper = new LandmarkMapper(mappingsfile.toStdString());
    }

}

FitModel *FitModel::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new FitModel();
    }

    return m_instance;
}


///
/// \brief FitModel::fitmodel
/// \param image_path
///
void FitModel::fitmodel(QString image_path)
{

    // 用QImage转换过来的Mat执行会出现错误……而且并不清楚是为啥
    Mat image = cv::imread(image_path.toStdString());

    QImage qimage = this->cvMat2QImage(image);      // 转换为QImage；

    // 检测特征点
    FaceDetection *detector = FaceDetection::getInstance();
    LandmarkCollection<cv::Vec2f> *landmarks = detector->landmark(qimage);     // 先检测特征点

    GLOBAL_VAR *global = GLOBAL_VAR::getInstance();

    // 可形变的模型
    morphablemodel::MorphableModel morphable_model;
    try
    {
        morphable_model = morphablemodel::load_model(
                    global->getMorphableModel().toStdString());
    }
    catch (const std::runtime_error& e)
    {
        cout << "Error loading the Morphable Model: " << e.what() << endl;
        return;
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
        //int vertex_idx = QString::fromStdString(
        //            converted_name.get()).toInt();

        Vec4f vertex = morphable_model.get_shape_model().get_mean_at_point(vertex_idx);
        model_points.emplace_back(vertex);
        vertex_indices.emplace_back(vertex_idx);
        image_points.emplace_back((*landmarks)[i].coordinates);
    }

    /// Keegan.Ren
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

    /// Keegan.Ren
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
    //                                                                    bin模型           投影矩阵              图片二维点

    std::vector<float> fitted_coeffs = fitting::fit_shape_to_landmarks_linear(
                morphable_model,
                affine_from_ortho,
                image_points,
                vertex_indices);

    // Keegan
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

    // Extract the texture from the image using given mesh and camera parameters:
    Mat isomap = render::extract_texture(
                mesh,
                affine_from_ortho,
                image);

    //cv::imshow("isomap_png", isomap);
    //cv::waitKey(1);

    this->isoMap = this->cvMat2QImage(isomap);

    // Save the mesh as textured obj:
    QString objPath = this->outputPath + "/" + this->fileName + ".obj";
    QString isoMapPath = this->outputPath + "/" + this->fileName + ".isomap.png";

    qDebug() << "objPath: " << objPath
             << "isoMapPath: "<< isoMapPath;

    render::write_textured_obj(
                mesh,
                objPath.toStdString());

    QImage qisomap = this->cvMat2QImage(isomap);
    qisomap.save(isoMapPath);
}

///
/// \brief FitModel::cvMat2QImage
/// \param mat
/// \return
///
QImage FitModel::cvMat2QImage(const cv::Mat &mat)
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
/// \brief FitModel::QImage2cvMat
/// \param image
/// \return
///
cv::Mat FitModel::QImage2cvMat(QImage image)
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
/// \brief FitModel::thread_fitModel
/// \param image_path
/// \param marks
///
void FitModel::thread_fitModel(
        QString image_path,
        LandmarkCollection<cv::Vec2f> *landmarks)
{
    // 用QImage转换过来的Mat执行会出现错误……而且并不清楚是为啥
    Mat image = cv::imread(image_path.toStdString());

    emit this->signals_progressValue(15);               // 进度条信息

    GLOBAL_VAR *global = GLOBAL_VAR::getInstance();         // 全局变量

    // 可形变的模型
    morphablemodel::MorphableModel morphable_model;
    try
    {
        morphable_model = morphablemodel::load_model(
                    global->getMorphableModel().toStdString());
    }
    catch (const std::runtime_error& e)
    {
        cout << "Error loading the Morphable Model: " << e.what() << endl;
        return;
    }

    emit this->signals_progressValue(35);               // 进度条信息
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

    emit this->signals_progressValue(50);               // 进度条信息

    // Estimate the camera (pose) from the 2D - 3D point correspondences
    fitting::OrthographicRenderingParameters rendering_params
            = fitting::estimate_orthographic_camera(
                image_points,
                model_points,
                image.cols,
                image.rows);

    emit this->signals_progressValue(65);               // 进度条信息

    /// Keegan.Ren
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
    //                                                                    bin模型           投影矩阵              图片二维点

    std::vector<float> fitted_coeffs = fitting::fit_shape_to_landmarks_linear(
                morphable_model,
                affine_from_ortho,
                image_points,
                vertex_indices);

    emit this->signals_progressValue(75);               // 进度条信息

    // Keegan
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

    emit this->signals_progressValue(80);               // 进度条信息

    // Obtain the full mesh with the estimated coefficients:
    render::Mesh mesh = morphable_model.draw_sample(
                fitted_coeffs,
                std::vector<float>());

    emit this->signals_progressValue(85);               // 进度条信息

    // Extract the texture from the image using given mesh and camera parameters:
    Mat isomap = render::extract_texture(
                mesh,
                affine_from_ortho,
                image);

    emit this->signals_progressValue(95);               // 进度条信息

    this->isoMap = this->cvMat2QImage(isomap);

    // Save the mesh as textured obj:
    QString objPath = this->outputPath + "/" + this->fileName + ".obj";
    QString isoMapPath = this->outputPath + "/" + this->fileName + ".isomap.png";

    qDebug() << "objPath: " << objPath
             << "isoMapPath: "<< isoMapPath;

    render::write_textured_obj(
                mesh,
                objPath.toStdString());
    emit this->signals_progressValue(98);               // 进度条信息

    QImage qisomap = this->cvMat2QImage(isomap);
    emit this->signals_isoMap(qisomap);
    emit this->signals_progressValue(100);               // 进度条信息
    emit this->signals_finished();
    qisomap.save(isoMapPath);
}
