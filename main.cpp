#include "mainwindow.h"
#include<stdlib.h>
#include <QApplication>
#include <QtPlugin>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
