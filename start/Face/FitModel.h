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
namespace eos
{
namespace core
{
class LandmarkMapper;
}
}
using eos::core::LandmarkMapper;

class FitModel
{
public:
    FitModel();

    static FitModel *getInstance();
    void fitmodel(QString image_path);          // 生成人脸模型

    QImage cvMat2QImage(const cv::Mat& mat);    // cvMat转换为QImage
    cv::Mat QImage2cvMat(QImage image);         // QImage 转化为cvMat

    QString getOutputPath(){return this->outputPath;}
    void setOutputPath(QString outPath){this->outputPath = outPath;}
    QString getFileName(){return this->fileName;}
    void setFileName(QString fileName){this->fileName = fileName;}

    QImage getIsoMap(){return this->isoMap;}

private:
//    QImage outImage;                  // 标记上Mark点的图片
    QImage isoMap;                      // 保存出的贴图

    QString outputPath;                 // 输出文件夹路径
    QString fileName;                   // 默认的文件名

    LandmarkMapper *landmark_mapper;    //LandMark_mapper

    static FitModel * m_instance;       // 单例模式
};

#endif // FITMODEL_H
