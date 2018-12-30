#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDirIterator>
#include <QTextStream>
#include "facerec.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/opencv_modules.hpp"
#include "opencv2/face.hpp"
#include "opencv2/face/facerec.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/core/mat.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <QVector>
#include <qdebug.h>
#include <vector>

#define DEFAUL_SAVE_FILE "/faces"
Q_DECLARE_METATYPE(cv::Mat)

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_train_clicked();

    void on_recognize_clicked();

    void updateFrame(QImage Frame);

    void on_real_time_recognize_clicked();

private:
    Ui::MainWindow *ui;
    cv::Ptr<cv::face::FaceRecognizer> model;
    cv::Mat matrice;
    FaceRec *recognizer;
};

#endif // MAINWINDOW_H
