#include "playbackmainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlaybackMainWindow w;
    w.show();
    return a.exec();
}
