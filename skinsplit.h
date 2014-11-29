#ifndef SKINSPLIT_H
#define SKINSPLIT_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class SkinSplit
{
public:
    SkinSplit();

    // skin region location using rgb limitation
    void skinRGB(cv::Mat& src,cv::Mat& dst);

    // skin detection in rg space
    void skinRG(cv::Mat& src,cv::Mat& dst);

    //implementation of otsu algorithm
    void cvThresholdOtsu(cv::Mat &src, cv::Mat &dst);

    void skinOTSU(cv::Mat& src,cv::Mat& dst);

    void skinYUV(cv::Mat& src,cv::Mat& dst);

    void skinHSV(cv::Mat& src,cv::Mat& dst);
};

#endif // SKINSPLIT_H
