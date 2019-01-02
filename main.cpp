#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.centralWidget()->setStyleSheet("background-color: gray;");
    w.show();
    w.show();

    return a.exec();
}
