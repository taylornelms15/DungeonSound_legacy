#include "playbackmainwindow.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //PlaybackMainWindow w;
    MainWindow w;
    w.show();
    return a.exec();
}
