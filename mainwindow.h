#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <iostream>
#include "facedetect.h"
#include "windowhelper.h"
#include "skinsplit.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
//    帧显示
    void displaySrc(cv::Mat &frame);
    void display_rgbSkin(cv::Mat &skin);
    void display_rgSkin(cv::Mat &skin);
    void display_otsuSkin(cv::Mat &skin);
    void display_hsvSkin(cv::Mat &skin);

private slots:
    void on_actionOpen_triggered();

    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
//    process标志
    bool process_flag;
//    设备号
    int devices_num;
//    人脸检测
    FaceDetect *face_detector;

    WindowHelper *helper;
    SkinSplit *skin;
//    使用的分类器文件
    QString cascade_file_name;
//    原始帧
    cv::Mat src_frame;
    cv::Mat rgb_skin;
    cv::Mat rg_skin;
    cv::Mat otsu_skin;
    cv::Mat hsv_skin;
//    videocapture
    cv::VideoCapture cam;
//    获取到的人脸区域
    cv::Rect face_roi;
//    人脸图像
    cv::Mat face;
};

#endif // MAINWINDOW_H
