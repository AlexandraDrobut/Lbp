#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv::face;
using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


static void read_csv(QFile& file, vector<cv::Mat>& images, vector<int>& labels, char separator = ';') {
    QString line;
    QStringList csv_elem;
    bool is_number = false;

    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while(!in.atEnd()) {
            line = in.readLine();
            csv_elem = line.split(';');

            foreach (const QString& elem , csv_elem) {

                if(elem.toInt(&is_number, 10)) {
                   labels.push_back(elem.toInt());
                }
                else images.push_back(cv::imread(elem.toStdString(),0));
            }
        }

        file.close();
    }

}


void MainWindow::on_train_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                          "/home",
                                                          QFileDialog::ShowDirsOnly
                                                          | QFileDialog::DontResolveSymlinks);

    QDirIterator iter(directory, QDirIterator::Subdirectories);
    QFileInfo file_dump;
    int subj_int = 0;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"));
    QFile file(fileName);

    if (file.open((QIODevice::ReadWrite))) {
        QTextStream stream(&file);

        while(iter.hasNext()) {
            file_dump = iter.next();

        if (file_dump.isDir() && file_dump.fileName().contains('s'))
             subj_int = file_dump.fileName().remove('s').toInt();

        if (!file_dump.suffix().compare("pgm")) {
             stream<<file_dump.absoluteFilePath();
            stream<<";"<<subj_int<<endl;
        }
    }

    file.close();
    }

    std::vector<cv::Mat> images;
    std::vector<int> labels;

    try {
        read_csv(file, images, labels);
    }
    catch(cv::Exception& e) {
        qDebug()<<"Error reading the files)";
        exit(1);
    }

    //Get the height from the first image
    int height = images.at(0).rows;
    cv::Mat test_sample = images[images.size() -1];
    int test_label = labels[labels.size() -1];
    images.pop_back();
    labels.pop_back();

    //Create LBH model and train it with the images given in the csv file
    cv::Ptr<cv::face::FaceRecognizer> model = LBPHFaceRecognizer::create();
    model->train((cv::InputArrayOfArrays)images, (cv::InputArray) labels);
    qDebug()<<"train";
    int predictedLabel = model->predict(test_sample);
    qDebug()<<"Predicted class  / Actual class ="<<predictedLabel << test_label <<endl;
      // First we'll use it to set the threshold of the LBPHFaceRecognizer
      // to 0.0 without retraining the model. This can be useful if
      // you are evaluating the model:
      //
      model->setThreshold(0.0);
      // Now the threshold of this model is set to 0.0. A prediction
      // now returns -1, as it's impossible to have a distance below
      // it
      predictedLabel = model->predict(test_sample);
      qDebug() << "Predicted class = " << predictedLabel << endl;
}

void MainWindow::on_recognize_clicked()
{
    QString image = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                 "/home",
                                                 tr("Images (*.png *.xpm *.jpg)"));
    ui->label->setPixmap(QPixmap::fromImage(QImage(image)));
}
