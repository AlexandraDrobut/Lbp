#include <QApplication>
#include <qdiriterator.h>
#include <qdebug.h>
#include <qfileinfo.h>
#include <QFileDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#define DEF_WIDTH 97
#define DEF_HEIGHT 112

using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cv::Mat source, desitnation;
    Size size(DEF_WIDTH, DEF_HEIGHT);
    QString directory = QFileDialog::getExistingDirectory(NULL, "Open Directory",
                                                          "/home",
                                                          QFileDialog::ShowDirsOnly
                                                          | QFileDialog::DontResolveSymlinks);


    QDirIterator iter(directory, QDirIterator::Subdirectories);
    QFileInfo file_dump;

    while(iter.hasNext()) {
        file_dump = iter.next();


    if (!file_dump.suffix().compare("pgm") || !file_dump.suffix().compare("jpg")) {
         source = imread(file_dump.absoluteFilePath().toStdString(), CV_LOAD_IMAGE_COLOR);
         resize(source, source, size);
    }
}

    return a.exec();
}
