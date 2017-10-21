#ifndef FITMODEL_H
#define FITMODEL_H

#include <eos/core/Landmark.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#ifdef WIN32
#define BOOST_ALL_DYN_LINK	// Link against the dynamic boost lib. Seems to be necessary because we use /MD, i.e. link to the dynamic CRT.
#define BOOST_ALL_NO_LIB	// Don't use the automatic library linking by boost with VS2010 (#pragma ...). Instead, we specify everything in cmake.
#endif
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <vector>
#include <iostream>
#include <fstream>
#include <QString>
#include <QImage>

using namespace eos;
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
    void fitmodel(QImage image);

    QImage cvMat2QImage(const cv::Mat& mat);    // cvMat转换为QImage
    cv::Mat QImage2cvMat(QImage image);         // QImage 转化为cvMat

    QImage getOutImage(){return this->outImage;}
    QImage getIsoMap(){return this->isoMap;}

    QString getOutputPath(){return this->outputPath;}
    void setOutputPath(QString outPath){this->outputPath = outPath;}
    QString getFileName(){return this->fileName;}
    void setFileName(QString fileName){this->fileName = fileName;}

private:
    QImage outImage;
    QImage isoMap;

    QString outputPath;
    QString fileName;
};

#endif // FITMODEL_H
