#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv/highgui.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    devices_num(1),
    process_flag(false)
{
    ui->setupUi(this);
    cascade_file_name = "/home/kevin/MasterProgram/SkinSplit/SkinSplit/haarcascades/haarcascade_frontalface_alt2.xml";
    face_detector = new FaceDetect;
    helper = new WindowHelper;
    skin = new SkinSplit;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    face_detector->set_cascade_file(cascade_file_name);
    process_flag = true;

    if(!cam.open(devices_num))
    {
        std::cout<<"Error:can't open webcam device!"<<std::endl;
        return;
    }

    while(process_flag)
    {

        cam.read(src_frame);
        helper->sleep(25);
        cv::cvtColor(src_frame,src_frame,CV_BGR2RGB);
        cv::flip(src_frame,src_frame,1);

        face_roi = face_detector->get_face_roi(src_frame);

        face = src_frame(face_roi);

        if(face_roi.height>0 &&face_roi.width>0)
        {
            skin->skinRGB(face,rgb_skin);
            skin->skinRG(face,rg_skin);
            skin->skinOTSU(face,otsu_skin);
            skin->skinHSV(face,hsv_skin);
        }
        cv::rectangle(src_frame,face_roi,cv::Scalar(255,0,0),1,8);

        displaySrc(src_frame);
        display_rgbSkin(rgb_skin);
        display_rgSkin(rg_skin);
        display_otsuSkin(otsu_skin);
        display_hsvSkin(hsv_skin);
    }

}

//帧显示
void MainWindow::displaySrc(cv::Mat &frame)
{
    // display image in the label
    QImage qsrc = QImage((const unsigned char*)(frame.data),
                         frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

    ui->labelframe->setPixmap(QPixmap::fromImage(qsrc));
    ui->labelframe->repaint();
}

void MainWindow::display_rgbSkin(cv::Mat &skin)
{
    // display image in the label
    cv::putText(skin,tr("rgb method").toStdString(),cv::Point(skin.cols/4,15),CV_FONT_NORMAL,0.7,cv::Scalar(255,0,0),1.3,8);
    QImage qsrc = QImage((const unsigned char*)(skin.data),
                         skin.cols, skin.rows, skin.step, QImage::Format_RGB888);

    ui->labelrgb->setPixmap(QPixmap::fromImage(qsrc));
    ui->labelrgb->repaint();
}

void MainWindow::display_rgSkin(cv::Mat &skin)
{
    cv::putText(skin,tr("rg method").toStdString(),cv::Point(skin.cols/4,15),CV_FONT_NORMAL,0.7,cv::Scalar(255,0,0),1.3,8);
    // display image in the label
    QImage qsrc = QImage((const unsigned char*)(skin.data),
                         skin.cols, skin.rows, skin.step, QImage::Format_RGB888);

    ui->labelrg->setPixmap(QPixmap::fromImage(qsrc));
    ui->labelrg->repaint();
}

void MainWindow::display_otsuSkin(cv::Mat &skin)
{
    cv::putText(skin,tr("otsu method").toStdString(),cv::Point(skin.cols/4,15),CV_FONT_NORMAL,0.7,cv::Scalar(255,0,0),1.3,8);
    // display image in the label
    QImage qsrc = QImage((const unsigned char*)(skin.data),
                         skin.cols, skin.rows, skin.step, QImage::Format_RGB888);

    ui->labelotsu->setPixmap(QPixmap::fromImage(qsrc));
    ui->labelotsu->repaint();
}

void MainWindow::display_hsvSkin(cv::Mat &skin)
{
    cv::putText(skin,tr("hsv method").toStdString(),cv::Point(skin.cols/4,15),CV_FONT_NORMAL,0.7,cv::Scalar(255,0,0),1.3,8);
    // display image in the label
    QImage qsrc = QImage((const unsigned char*)(skin.data),
                         skin.cols, skin.rows, skin.step, QImage::Format_RGB888);

    ui->labelhsv->setPixmap(QPixmap::fromImage(qsrc));
    ui->labelhsv->repaint();
}

void MainWindow::on_actionExit_triggered()
{
    process_flag = false;
    qApp->exit();
}
