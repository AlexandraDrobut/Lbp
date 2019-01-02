#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    recognizer = new FaceRec();

    QObject::connect(recognizer, SIGNAL(image_processed(QImage)),
                                    this, SLOT(updateFrame(QImage)));
    ui->setupUi(this);
    this->setWindowTitle("Local Binary Patterns");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete recognizer;
}


void MainWindow::read_csv(QFile& file, vector<cv::Mat>& images, vector<int>& labels) {
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

void MainWindow::updateFrame(QImage Frame)
{
        ui->label->setAlignment(Qt::AlignCenter);
        ui->label->setPixmap(QPixmap::fromImage(Frame).scaled(ui->label->size()
                                          , Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::on_actionTrain_changed()
{

    QString directory = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                          "/home",
                                                          QFileDialog::ShowDirsOnly
                                                          | QFileDialog::DontResolveSymlinks);

    if (directory == NULL)
        qDebug() << "No directory specified, aborting!";

    QDirIterator iter(directory, QDirIterator::Subdirectories);
    QFileInfo file_dump;
    int subj_int = 0;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"));

    if (fileName == NULL)
        fileName = DEFAUL_SAVE_FILE;

    QFile file(fileName);

    if (file.open((QIODevice::ReadWrite))) {
        QTextStream stream(&file);

        while(iter.hasNext()) {
            file_dump = iter.next();

        if (file_dump.isDir() && file_dump.fileName().contains('s'))
             subj_int = file_dump.fileName().remove('s').toInt();

        if (!file_dump.suffix().compare("pgm") || !file_dump.suffix().compare("jpg")) {
             stream<<file_dump.absoluteFilePath();
            stream<<";"<<subj_int<<endl;
        }
    }

    file.close();
    }

    std::vector<cv::Mat> images;
    std::vector<int> labels;

    try {
        MainWindow::read_csv(file, images, labels);
    }
    catch(cv::Exception& e) {
        qDebug()<<"Error reading the files";
        exit(1);
    }
    //Create LBH model and train it with the images given in the csv file
    model = LBPHFaceRecognizer::create();
    model->train(images,labels);
}

void MainWindow::on_actionRecognize_changed()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                   tr("Open Image"),
                                                   tr("."),
                                                   tr("Image Files (*.jpg *.pgm)"));

    matrice = cv::imread(fileName.toStdString(), 0 );

    if(this->model == NULL) {
        ui->label_2->setText("Didn't train any model retry!");
        on_actionTrain_changed();
    }

    try  {
    recognizer->set_model(this->model);
    recognizer->setFrame(matrice);
    recognizer->openCameraVideo();
    } catch(cv::Exception &e) {
        exit(1);
    }

    recognizer->stop();
}
