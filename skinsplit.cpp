#include "skinsplit.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>

SkinSplit::SkinSplit()
{
}

void SkinSplit::skinRGB(cv::Mat &src, cv::Mat &dst)
{
    assert(src.channels() ==3);

    cv::Mat temp = cv::Mat(src.rows,src.cols,CV_8UC3);

//    std::cout<<"widht: "<<src.cols<<"height: "<<src.rows<<std::endl;

    for(int i=0;i<src.rows;i++)
    {
        for(int j=0;j<src.cols;j++)
        {
            int r = src.at<cv::Vec3b>(i,j)[0];
            int g = src.at<cv::Vec3b>(i,j)[1];
            int b = src.at<cv::Vec3b>(i,j)[2];

            if((r>95 && g>40 && b>20 &&
                    r-b>15 && r-g>15) ||
                    (r>200 && g>210 && b>170 &&
                    std::abs(r-b)<=15 && r>b && g>b)
                    )
            {
                temp.at<cv::Vec3b>(i,j)[0] = r;
                temp.at<cv::Vec3b>(i,j)[1] = g;
                temp.at<cv::Vec3b>(i,j)[2] = b;
            }
            else
            {
                temp.at<cv::Vec3b>(i,j)[0] = 0;
                temp.at<cv::Vec3b>(i,j)[1] = 0;
                temp.at<cv::Vec3b>(i,j)[2] = 0;
            }
        }
    }

    temp.copyTo(dst);
}

void SkinSplit::skinRG(cv::Mat &src, cv::Mat &dst)
{
    assert(src.channels() == 3);

    cv::Mat temp = cv::Mat(src.rows,src.cols,CV_8UC3);

    double Aup=-1.8423;
    double Bup=1.5294;
    double Cup=0.0422;
    double Adown=-0.7279;
    double Bdown=0.6066;
    double Cdown=0.1766;

    for(int i=0;i<src.rows;i++)
        for(int j=0;j<src.cols;j++)
        {
            double r = src.at<cv::Vec3b>(i,j)[0];
            double g = src.at<cv::Vec3b>(i,j)[1];
            double b = src.at<cv::Vec3b>(i,j)[2];

            double s = r+g+b;
            double pr = r/s;
            double pg = g/s;
            double Gup = Aup*pr*pr + Bup*pr +Cup;
            double Gdown = Adown*pr*pr + Bdown*pr + Cdown;
            double wr = (pr-0.33)*(pr-0.33) + (pg-0.33)*(pg-0.33);
            if(pg<Gup && pg>Gdown && wr>0.004)
            {
                temp.at<cv::Vec3b>(i,j)[0] = r;
                temp.at<cv::Vec3b>(i,j)[1] = g;
                temp.at<cv::Vec3b>(i,j)[2] = b;
            }
            else
            {
                temp.at<cv::Vec3b>(i,j)[0] = 0;
                temp.at<cv::Vec3b>(i,j)[1] = 0;
                temp.at<cv::Vec3b>(i,j)[2] = 0;
            }
        }

    temp.copyTo(dst);
}

void SkinSplit::cvThresholdOtsu(cv::Mat& src, cv::Mat& dst)
{
    assert(src.channels() == 1);
    int hist[256] = {0};
    double pro_hist[256] = {0.0};

    int height =  src.rows;
    int width = src.cols;
    //统计每个灰度的数量
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            int temp = src.at<uchar>(i,j);
            hist[temp]++;
        }
    }

    //计算每个灰度级占图像中的概率
    for(int i=0;i<256;i++)
    {
        pro_hist[i] = (double)hist[i]/(double)(width*height);
    }

    //计算平均灰度
    double avgPixel = 0.0;
    for(int i=0;i<256;i++)
    {
        avgPixel += i*pro_hist[i];
    }

    int threshold=0;
    double maxVariance = 0;
    double w =0,u=0;
    for(int i=0;i<256;i++)
    {
        w+=pro_hist[i];
        u+=i*pro_hist[i];

        double t = avgPixel*w -u;
        double variance = t*t/(w*(1-w));
        if(variance>maxVariance)
        {
            maxVariance = variance;
            threshold = i;
        }
    }

    cv::threshold(src,dst,threshold,255,CV_THRESH_BINARY);

}

void SkinSplit::skinOTSU(cv::Mat &src, cv::Mat &dst)
{
    assert(src.channels() ==3);

    cv::Mat ycrcb;
    cv::Mat cr;

    cv::cvtColor(src,ycrcb,CV_RGB2YCrCb);
    std::vector<cv::Mat> channels;
    cv::split(ycrcb,channels);
    cr=channels[1];

    cv::Mat temp = cv::Mat(src.rows,src.cols,CV_8UC3);

//    cv::namedWindow("Y");
//    cv::namedWindow("cr");
//    cv::namedWindow("cb");

//    cv::imshow("Y",channels[0]);
//    cv::imshow("cr",channels[1]);
//    cv::imshow("cb",channels[2]);
    cvThresholdOtsu(cr,cr);
//    cv::imshow("cr",cr);
    for(int i=0;i<src.rows;i++)
        for(int j=0;j<src.cols;j++)
        {
            if(cr.at<uchar>(i,j) >100)
            {
                temp.at<cv::Vec3b>(i,j)[0] = src.at<cv::Vec3b>(i,j)[0];
                temp.at<cv::Vec3b>(i,j)[1] = src.at<cv::Vec3b>(i,j)[1];
                temp.at<cv::Vec3b>(i,j)[2] = src.at<cv::Vec3b>(i,j)[2];
            }
            else
            {
                temp.at<cv::Vec3b>(i,j)[0] = 0;
                temp.at<cv::Vec3b>(i,j)[1] = 0;
                temp.at<cv::Vec3b>(i,j)[2] = 0;
            }
        }

    temp.copyTo(dst);
}

void SkinSplit::skinHSV(cv::Mat &src, cv::Mat &dst)
{
    cv::Mat temp = cv::Mat(src.rows,src.cols,CV_8UC3);
    cv::Mat hsv;
    cv::cvtColor(src,hsv,CV_RGB2HSV);
    for(int i=0;i<src.rows;i++)
    {
        for(int j=0;j<src.cols;j++)
        {
            double h = hsv.at<cv::Vec3b>(i,j)[0];
            double s = hsv.at<cv::Vec3b>(i,j)[1];
            double v = hsv.at<cv::Vec3b>(i,j)[2];

            if(h>=7 && h<=29)
            {
                temp.at<cv::Vec3b>(i,j)[0] = src.at<cv::Vec3b>(i,j)[0];
                temp.at<cv::Vec3b>(i,j)[1] = src.at<cv::Vec3b>(i,j)[1];
                temp.at<cv::Vec3b>(i,j)[2] = src.at<cv::Vec3b>(i,j)[2];
            }
            else
            {
                temp.at<cv::Vec3b>(i,j)[0] = 0;
                temp.at<cv::Vec3b>(i,j)[1] = 0;
                temp.at<cv::Vec3b>(i,j)[2] = 0;
            }
        }
    }

    temp.copyTo(dst);
}
