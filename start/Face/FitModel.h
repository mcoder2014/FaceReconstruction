#ifndef FITMODEL_H
#define FITMODEL_H

#include "eos/core/Landmark.hpp"
#include "eos/core/LandmarkMapper.hpp"
#include "eos/fitting/nonlinear_camera_estimation.hpp"
#include "eos/fitting/linear_shape_fitting.hpp"
#include "eos/render/utils.hpp"
#include "eos/render/texture_extraction.hpp"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#ifdef WIN32
#define BOOST_ALL_DYN_LINK	// Link against the dynamic boost lib. Seems to be necessary because we use /MD, i.e. link to the dynamic CRT.
#define BOOST_ALL_NO_LIB	// Don't use the automatic library linking by boost with VS2010 (#pragma ...). Instead, we specify everything in cmake.
#endif
#include "boost/program_options.hpp"
#include "boost/filesystem.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <QString>
#include <QImage>

using namespace eos;
namespace po = boost::program_options;
namespace fs = boost::filesystem;
using eos::core::Landmark;
using eos::core::LandmarkCollection;
using cv::Mat;
using cv::Vec2f;
using cv::Vec3f;
using cv::Vec4f;
using std::cout;
using std::endl;
using std::vector;
using std::string;

class FitModel
{
public:
    FitModel();
    void fitmodel(QImage image, QString isomapPath, QString outputfilePath);

    QImage cvMat2QImage(const cv::Mat& mat);    // cvMat转换为QImage
    cv::Mat QImage2cvMat(QImage image);         // QImage 转化为cvMat

};

#endif // FITMODEL_H
