#ifndef FACEREC_H
#define FACEREC_H
#include <QThread>
#include <QMutex>
#include <QImage>
#include <QObject>
#include <QWaitCondition>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/face.hpp>
#include "opencv2/face/facerec.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>
#include <qdebug.h>

class FaceRec : public QThread
{
    Q_OBJECT//thread aware holds an instance to the thread it was creatged into
public:
    FaceRec(QObject *parent = 0) : QThread(parent), is_runnable(true) {}
    void openCameraVideo();
    void set_model(cv::Ptr<cv::face::FaceRecognizer>);
    void setFrame(cv::Mat);
    virtual ~FaceRec();
    void run() override;
    void stop();
    void restart();
    bool is_running();
signals:
    void image_processed(QImage);
public slots:
private:
    bool is_runnable;
    cv::Ptr<cv::face::FaceRecognizer> model;
    cv::Mat frame;
    cv::Mat g_frame;
    QImage image;
    cv::VideoCapture videosource;
};

#endif // FACEREC_H
