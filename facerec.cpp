#include "facerec.h"

FaceRec::~FaceRec()
{

}


bool FaceRec::is_running()
{
    return this->is_runnable;
}

void FaceRec::setFrame(cv::Mat new_frame)
{
    this->frame = new_frame;
}

void FaceRec::openCameraVideo()
{
        this->is_runnable = true;
        start(LowPriority);
}

void FaceRec::run()
{
    int pred = 0;

    while(this->is_runnable) {
    if(frame.channels() == 3)
        cv::cvtColor(frame,frame,CV_BGR2RGB);

    pred = model->predict(frame);
    qDebug() << pred;
    if(pred)
            cv::putText(frame,
                QString::number(pred).toStdString(),
                 cv::Point(40,40), // Coordinates
                 cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
                 1.0, // Scale. 2.0 = 2x bigger
                 cv::Scalar(150,0,30), // BGR Color
                 5); // Anti-alias (Optional)

    image = QImage((const unsigned char*)(frame.data),
                   frame.cols,frame.rows,QImage::Format_Indexed8);
     emit image_processed(image);
     this->msleep(60);
    }
}

void FaceRec::set_model(cv::Ptr<cv::face::FaceRecognizer> model)
{
    this->model = model;
}

void FaceRec::stop()
{
    this->is_runnable = false;
}
