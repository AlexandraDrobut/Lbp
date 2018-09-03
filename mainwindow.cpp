#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    QString image = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                 "/home",
                                                 tr("Images (*.png *.xpm *.jpg)"));
    ui->label->setPixmap(QPixmap::fromImage(QImage(image)));
}


static void read_csv(QFile& file, QVector<cv::Mat>& images, QVector<int>& labels, char separator = ';') {
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

void MainWindow::on_pushButton_clicked()
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

    QVector<cv::Mat> images;
    QVector<int> labels;

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
    images.removeLast();
    labels.removeLast();
    
    //Create LBH model and train it with the images given in the csv file
  //  cv::Ptr<cv::FaceRecognizer> model = cv::cre

}

