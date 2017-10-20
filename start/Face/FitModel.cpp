#include "FitModel.h"
#include <QDebug>
#include "FaceDetection.h"
#include "GLOBAL_VAR.h"

FitModel::FitModel()
{

}

///
/// \brief FitModel::fitmodel
/// \param image
/// \param isomapPath
/// \param outputfilePath
///
void FitModel::fitmodel(QImage qimage, QString isomapPath, QString outputfilePath)
{
    FaceDetection detector;
    LandmarkCollection<cv::Vec2f> *landmarks = detector.landmark(qimage);     // 先检测特征点
    Mat image = this->QImage2cvMat(qimage);     // 将图像转换为cvMat

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
        return;
    }
    QString mappingsfile = global->getMappingsFile();

    // landmarkMapper
    core::LandmarkMapper landmark_mapper =
            mappingsfile.size()==0 ?
                core::LandmarkMapper() : core::LandmarkMapper(mappingsfile.toStdString());

    // 处出的图片
    Mat outimg = image.clone();
    int i = 1;
    for (auto&& lm : landmarks)
    {
        cv::Point numPoint(lm.coordinates[0] - 2.0f, lm.coordinates[1] - 2.0f);
        cv::rectangle(
                    outimg,
                    cv::Point2f(lm.coordinates[0] - 2.0f, lm.coordinates[1] - 2.0f),
                    cv::Point2f(lm.coordinates[0] + 2.0f, lm.coordinates[1] + 2.0f),
                    { 255, 0, 0 });
        /// Keegan.Ren
        /// TODO: plot the face point and point number in the image
        char str_i[11];
        sprintf(str_i, "%d", i);
        cv::putText(outimg, str_i, numPoint, CV_FONT_HERSHEY_COMPLEX, 0.3, cv::Scalar(0, 0, 255));
        ++i;
    }
    qDebug() << "i= " << i;

    // These will be the final 2D and 3D points used for the fitting:
    vector<Vec4f> model_points; // the points in the 3D shape model
    vector<int> vertex_indices; // their vertex indices
    vector<Vec2f> image_points; // the corresponding 2D landmark points
    // Sub-select all the landmarks which we have a mapping for (i.e. that are defined in the 3DMM):
    for (int i = 0; i < landmarks.size(); ++i) {
        auto converted_name = landmark_mapper.convert(landmarks[i].name);
        if (!converted_name) { // no mapping defined for the current landmark
            continue;
        }
        int vertex_idx = std::stoi(converted_name.get());
        Vec4f vertex = morphable_model.get_shape_model().get_mean_at_point(vertex_idx);
        model_points.emplace_back(vertex);
        vertex_indices.emplace_back(vertex_idx);
        image_points.emplace_back(landmarks[i].coordinates);
    }

    /// Keegan.Ren
    qDebug() << "model_point = " ;
    for (int i = 0; i < landmarks.size(); ++i) {
        //         3d points                                                    2d points
        qDebug() << model_points[i] << "\t" << vertex_indices[i] << "\t" << image_points[i];
    }

    // Estimate the camera (pose) from the 2D - 3D point correspondences
        fitting::OrthographicRenderingParameters rendering_params = fitting::estimate_orthographic_camera(image_points, model_points, image.cols, image.rows);
        /// Keegan.Ren
        cout << "rendering_params = " << endl;
        std::cout << rendering_params.r_x << " " << rendering_params.r_y << " " << rendering_params.r_z << " " << rendering_params.t_x << " " << rendering_params.t_y << endl;
        std::cout << rendering_params.frustum.b << " " << rendering_params.frustum.l << " " << rendering_params.frustum.r << " " << rendering_params.frustum.t << endl;
        Mat affine_from_ortho = get_3x4_affine_camera_matrix(rendering_params, image.cols, image.rows);

        // Estimate the shape coefficients by fitting the shape to the landmarks:
        //                                                                    bin模型           投影矩阵              图片二维点

        vector<float> fitted_coeffs = fitting::fit_shape_to_landmarks_linear(morphable_model, affine_from_ortho, image_points, vertex_indices);

        // Keegan
        cout << "affine_from_ortho = " << endl;
        cout << affine_from_ortho << endl;
        // 	cv::imshow("affine_from_ortho", affine_from_ortho);
        // 	cv::waitKey();

            // The 3D head pose can be recovered as follows:
        float xaw_angle = glm::degrees(rendering_params.r_x);
        float yaw_angle = glm::degrees(rendering_params.r_y);
        float zaw_angle = glm::degrees(rendering_params.r_z);
        cout << "x_y_z_angle = " << endl;
        cout << xaw_angle << "\t" << yaw_angle << "\t" << zaw_angle << endl;
        // and similarly for pitch (r_x) and roll (r_z).


        cout << "size = " << fitted_coeffs.size() << endl;
        for (int i = 0; i < fitted_coeffs.size(); ++i)
            cout << fitted_coeffs[i] << endl;

        // Obtain the full mesh with the estimated coefficients:
        render::Mesh mesh = morphable_model.draw_sample(fitted_coeffs, vector<float>());

        // Extract the texture from the image using given mesh and camera parameters:
        Mat isomap = render::extract_texture(mesh, affine_from_ortho, image);

        // Save the mesh as textured obj:
        outputfile += fs::path(".obj");
        render::write_textured_obj(mesh, outputfile.string());

        // And save the isomap:
        outputfile.replace_extension(".isomap.png");
        cv::imwrite(outputfile.string(), isomap);

        cv::imshow(outputfile.string(), isomap);
        cv::waitKey();

        cout << "Finished fitting and wrote result mesh and isomap to files with basename " << outputfile.stem().stem() << "." << endl;


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
