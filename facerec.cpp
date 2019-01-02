#include "facerec.h"

FaceRec::~FaceRec()
{
    is_runnable = false;
    frame.empty();
    model.release();
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
    Size size(DEF_WIDTH, DEF_HEIGHT);
    Rect rect;
    cv::Rect slide_window;
    cv::Mat window;

    int pred = 0;

    while(this->is_runnable) {
        if(frame.channels() == 3)
            cv::cvtColor(frame, frame, CV_BGR2RGB);

        for ( int i = 0 ; i < (frame.cols - W_SIZE) ; i += 40) {
            for ( int j = 0 ; j < frame.rows - W_SIZE; j+= 40) {
                slide_window = cv::Rect(i ,j , W_SIZE, W_SIZE);
                window = cv::Mat(frame, slide_window);
                resize(window, window, size);
                pred = model->predict(window);
                if(pred == 42) {
                        cv::putText(frame,
                             QString::number(pred).toStdString(),
                             cv::Point(40,40), // Coordinates
                             cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
                             1.0, // Scale. 2.0 = 2x bigger
                             cv::Scalar(150,0,30), // BGR Color
                             5); // Anti-alias (Optional)

                        rect = Rect(i, j, W_SIZE, W_SIZE);

                        rectangle(frame, rect, Scalar(255,0 , 0 ), 1, 8, 0 );
                  }

            }
        }

        image = QImage((const unsigned char*)(frame.data),
                       frame. cols,frame.rows, QImage::Format_Indexed8);
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
