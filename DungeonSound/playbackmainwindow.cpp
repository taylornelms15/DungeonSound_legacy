#include "playbackmainwindow.h"
#include "./ui_playbackmainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QtAlgorithms>

#include "flowlayout/flowlayout.h"
#include "playlistbox.h"

/* UI Set-Up */

void PlaybackMainWindow::clearBackgroundPlaylistsWidget()
{
    QWidget *container = ui->backgroundPlaylistsCollection;
    container->setUpdatesEnabled(false);
    QLayout* layout = container->layout();
    for(int i = layout->count() - 1; i >= 0; --i) {
        QLayoutItem* item = layout->takeAt(i);
        delete item->widget();
        delete item;
    }
    //qDeleteAll(container->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));
    container->setUpdatesEnabled(true);
}

int PlaybackMainWindow::updateBackgroundPlaylistsWidget()
{
    QWidget *container = ui->backgroundPlaylistsCollection;
    QFrame *addPlaylistFrame = ui->AddPlaylistFrame;
    // Remove the "add" frame, keep it around
    QLayoutItem *addLayoutItem = container->layout()->takeAt(container->layout()->indexOf(addPlaylistFrame));

    clearBackgroundPlaylistsWidget();

    // Look at our show logic
    ShowFile *sf = navstate.showfile;
    // No ShowFile, so just re-add "add" button (?)
    if (!sf)
    {
        container->layout()->addItem(addLayoutItem);
        return 0;
    }
    for (int i = 0; i < sf->getNumBackgroundPlaylists(); ++i) {
        const Playlist *pl = sf->getBackgroundPlaylist(i);
        if (!pl){
            qWarning("No valid playlist found for index %d", i);
            return -EINVAL;
        }
        PlaylistBox *pl_box = new PlaylistBox(pl, container);
        QObject::connect(pl_box, &PlaylistBox::signalEditPlaylistPressed,
                         this, &PlaybackMainWindow::catchEditPlaylistPressed);
        container->layout()->addWidget(pl_box);
    }
    container->layout()->addItem(addLayoutItem);
    return 0;
}

int PlaybackMainWindow::setupBackgroundPlaylistsWidget()
{
    // Connect the add button to the callback for this window
    connect(ui->addBackgroundMusicPlaylistButton, &QAbstractButton::pressed, this, &PlaybackMainWindow::addBackgroundPlaylistButton);

    // Grab the extant items
    QWidget *container = ui->backgroundPlaylistsCollection;
    QFrame *addPlaylistFrame = ui->AddPlaylistFrame;
    // Remove the "add" frame, keep it around
    QLayoutItem *addLayoutItem = container->layout()->takeAt(container->layout()->indexOf(addPlaylistFrame));

    // Change the layout to the flow layout
    delete container->layout();
    FlowLayout *flayout = new FlowLayout(container);
    flayout->addItem(addLayoutItem);

    return updateBackgroundPlaylistsWidget();
}

int PlaybackMainWindow::updateWidgets()
{
    qDebug() << "<UI> " << __PRETTY_FUNCTION__;
    int rv;
    rv = updateBackgroundPlaylistsWidget();
    if (rv)
       return rv;

    return rv;
}

/* Constructor/Destructor */

PlaybackMainWindow::PlaybackMainWindow(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::PlaybackMainWindow)
    , navstate(NavigationState::getInstance())
{
    ui->setupUi(this);

#ifdef USING_SAMPLE_SHOWFILE
    navstate.loadSampleShowFile();
#else
    navstate.loadNewShowFile();
#endif
    setupBackgroundPlaylistsWidget();

}

PlaybackMainWindow::~PlaybackMainWindow()
{
    delete ui;
}

/* Operations */

int PlaybackMainWindow::executeAddBackgroundPlaylist()
{
    navstate.currentWindowId = WindowId::PLAYLIST;
    navstate.executeEditBackgroundPlaylist(navstate.showfile->getNumBackgroundPlaylists());
    emit needUpdateWidgets();
    return 0;
}

/* Button Press Callbacks */

void PlaybackMainWindow::addBackgroundPlaylistButton()
{
    qDebug("<Button Press> Add Background Music Playlist");
    int rv = executeAddBackgroundPlaylist();
    if (rv)
       qWarning() << "Error on Add Background Playlist: " << rv;
}

void PlaybackMainWindow::catchEditPlaylistPressed(const Playlist *pl)
{
    int rv = navstate.executeEditBackgroundPlaylist(pl);
    if (rv) {
       qWarning() << "Error on Edit Background Playlist: " << rv;
       return;
    }
    emit needUpdateWidgets();
}
