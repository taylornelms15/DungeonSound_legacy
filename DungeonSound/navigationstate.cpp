#include "navigationstate.h"

NavigationState::NavigationState() :
    showfile(nullptr)
    , currentWindowId(WindowId::PLAYBACK)
{}

void NavigationState::loadNewShowFile()
{
    showfile = new ShowFile();
    currentWindowId = WindowId::PLAYBACK;
}

int NavigationState::loadShowFile(const QString path)
{
    currentWindowId = WindowId::PLAYBACK;
    if (showfile) {
        delete showfile;
    }
    showfile = new ShowFile();
    return showfile->loadFile(path);
}

#ifdef USING_SAMPLE_SHOWFILE
int NavigationState::loadSampleShowFile()
{
    return loadShowFile(QString(sample_saveloc));
}
#endif

int NavigationState::executeBack()
{
    switch(currentWindowId) {
    case WindowId::PLAYBACK:
        qWarning() << "Invalid state: 'Back' operation on Playback window";
        return -EINVAL;
    case WindowId::PLAYLIST:
        qDebug() << "<Navigation> Registered 'Back' while on PLAYLIST window, returning to PLAYBACK";
        currentWindowId = WindowId::PLAYBACK;
        break;
    default:
        qWarning() << "Unknown window id " << currentWindowId;
        return -EINVAL;
    }

    return 0;
}
