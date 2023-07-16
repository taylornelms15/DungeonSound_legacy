#include "navigationstate.h"

NavigationState::NavigationState() :
    showfile(nullptr)
    , currentWindowId(WindowId::PLAYBACK)
    , currentBackgroundPlaylistId(-1)
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

int NavigationState::executeEditBackgroundPlaylist(size_t idx)
{
    currentWindowId = WindowId::PLAYLIST;
    if (idx == showfile->getNumBackgroundPlaylists()){
        // create a new playlist at the end
        Playlist pl = Playlist();
        showfile->appendBackgroundPlaylist(pl);
    }
    if (idx > showfile->getNumBackgroundPlaylists()){
        qWarning() << "Invalid playlist index provided: " << idx << ">" << showfile->getNumBackgroundPlaylists();
        return -EINVAL;
    }
    currentBackgroundPlaylistId = idx;
    return 0;
}

int NavigationState::executeBack()
{
    switch(currentWindowId) {
    case WindowId::PLAYBACK:
        qWarning() << "Invalid state: 'Back' operation on Playback window";
        return -EINVAL;
    case WindowId::PLAYLIST:
        qDebug() << "<Navigation> Registered 'Back' while on PLAYLIST window, returning to PLAYBACK";
        currentBackgroundPlaylistId = -1;
        currentWindowId = WindowId::PLAYBACK;
        break;
    default:
        qWarning() << "Unknown window id " << currentWindowId;
        return -EINVAL;
    }

    return 0;
}
