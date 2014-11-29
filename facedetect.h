#ifndef FACEDETECT_H
#define FACEDETECT_H

#include <QString>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/video/video.hpp>

class FaceDetect
{
public:
    FaceDetect();

    //设置使用的分类器
    void set_cascade_file(QString& cascade_file)
    {
        face_cascade_name = cascade_file;
    }

    //检测人脸区域
    cv::Rect get_face_roi(cv::Mat &frame);

private:

    cv::CascadeClassifier cascade; //分类器对象

    QString face_cascade_name;  //所使用的分类器名称

//    cv::Rect face_roi; //检测到的人脸区域

};

#endif // FACEDETECT_H
