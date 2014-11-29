#include "facedetect.h"

FaceDetect::FaceDetect()
{
}

//检测人脸区域位置
cv::Rect FaceDetect::get_face_roi(cv::Mat &frame)
{
    cascade.load(this->face_cascade_name.toStdString());

    //灰度化
    cv::Mat gray;
    cv::cvtColor(frame,gray,CV_RGB2GRAY);

    //直方图均衡
    cv::equalizeHist(gray,gray);

    //检测人脸
    std::vector<cv::Rect> face_vector;
    //建立缩小的图片以加快检测速度
    int scale = 3;
    cv::Mat smallImage(cvRound(gray.rows/scale),cvRound(gray.cols/scale),CV_8UC1);
    //改变图像大小，使用双线性差值
    cv::resize(gray,smallImage,smallImage.size(),0,0,CV_INTER_LINEAR);
    //多尺度的人脸检测
    cascade.detectMultiScale(smallImage,face_vector,1.1,3,CV_HAAR_SCALE_IMAGE | CV_HAAR_FIND_BIGGEST_OBJECT,
                             cv::Size(50,50));//

    cv::Rect face_roi = cvRect(0,0,0,0);
    if(!face_vector.empty())
    {
        //这里只进行单张人脸的处理，因此只使用单个人脸
        //我们只用原来窗口大小的80%大小宽度，高度不变
        double temp = face_vector[0].width * scale * 0.1;
        double lx = cvRound(face_vector[0].x * scale + temp);
        double ly = cvRound(face_vector[0].y * scale);
//        leftUpCorner = cv::Point(lx,ly);    //左上角

        double rx = cvRound(face_vector[0].x * scale + face_vector[0].width * scale - temp);
        double ry = cvRound(face_vector[0].y * scale + face_vector[0].height * scale);
//        rightDownCorner = cv::Point(rx,ry);    //右下角

        face_roi = cvRect(lx,ly,rx-lx,ry-ly);
    }

    return face_roi;
}
