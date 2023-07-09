#include "playbackmainwindow.h"
#include "./ui_playbackmainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QtAlgorithms>

#include "flowlayout/flowlayout.h"
#include "playlistbox.h"
#include "navigationstate.h"

/* UI Set-Up */

void PlaybackMainWindow::setActionIconStyle(QAction *action, QString iconstring)
{
    QIcon constructedIcon;
    QString filename_16 = QString(":/DungeonSound/Icons/16x16 " + iconstring + ".png");
    QString filename_24 = QString(":/DungeonSound/Icons/24x24 " + iconstring + ".png");
    if (QFile::exists(filename_16)) {
        constructedIcon.addFile(filename_16, QSize(16, 16));
    }
    else {
        qDebug("Icon path %s not found", qUtf8Printable(filename_16));
    }
    if (QFile::exists(filename_24)) {
        constructedIcon.addFile(filename_24, QSize(24, 24));
    }
    else {
        qDebug("Icon path %s not found", qUtf8Printable(filename_24));
    }
    action->setIcon(constructedIcon);
}

void PlaybackMainWindow::setActionIconStyle(QAction *action, QStyle::StandardPixmap stylepixmap)
{
    action->setIcon(this->style()->standardIcon(stylepixmap));
}

int PlaybackMainWindow::setupIconBar()
{
    // Set images
    setActionIconStyle(ui->actionBackShowFile, QStyle::SP_FileDialogBack);
    setActionIconStyle(ui->actionNewShowFile, QStyle::SP_FileIcon);
    setActionIconStyle(ui->actionOpenShowFile, QStyle::SP_DialogOpenButton);
    setActionIconStyle(ui->actionSaveShowFile, QStyle::SP_DialogSaveButton);
    setActionIconStyle(ui->actionSettingsShowFile, "gear");

    // Connect button callbacks
    connect(ui->actionNewShowFile, &QAction::triggered, this, &PlaybackMainWindow::newShowFileButton);
    connect(ui->actionOpenShowFile, &QAction::triggered, this, &PlaybackMainWindow::openShowFileButton);
    connect(ui->actionSaveShowFile, &QAction::triggered, this, &PlaybackMainWindow::saveShowFileButton);
    connect(ui->actionSettingsShowFile, &QAction::triggered, this, &PlaybackMainWindow::settingsShowFileButton);

    return 0;
}

void PlaybackMainWindow::clearBackgroundPlaylistsWidget()
{
    QWidget *container = ui->backgroundPlaylistsCollection;
    container->setUpdatesEnabled(false);
    qDeleteAll(container->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));
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
        PlaylistBox *pl_box = new PlaylistBox(container, sf->getBackgroundPlaylist(i));
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
   int rv;
   rv = updateBackgroundPlaylistsWidget();
   if (rv)
       return rv;

   return rv;
}

/* Constructor/Destructor */

PlaybackMainWindow::PlaybackMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PlaybackMainWindow)
    , navstate(NavigationState::getInstance())
{
    ui->setupUi(this);
    setupIconBar();

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

/* File Operations */

int PlaybackMainWindow::executeSaveShowFile()
{
    if (navstate.showfile) {
        if (!navstate.showfile->getFilePath().length()) {
            // if no save filepath, get one
            QString savepath = QFileDialog::getSaveFileName(
                        this,
                        "Save File",
                        QString(),
                        "ShowFiles (*.showfile)");
            if (savepath.length()){
                qDebug() << "<Saving> Save path chosen <" << savepath << ">, saving.";
                return navstate.showfile->saveShowFile(savepath);
            }
            else{
                qDebug() << "<Saving> Save path not chosen <" << savepath << ">, not saving.";
                return 0;
            }
        }
        else {
            qDebug() << "<Saving> Save path kept as <" << navstate.showfile->getFilePath() << ">, saving.";
            return navstate.showfile->saveShowFile();
        }
    }
    else {
        qWarning() << "No showfile under NavState object, not saving";
        return -EINVAL;
    }
}

int PlaybackMainWindow::executeNewShowFile()
{
    // TODO: "Would you like to save?"
    qDebug() << "<Loading> Loading new show file";
    navstate.loadNewShowFile();
    updateWidgets();
    return 0;
}

int PlaybackMainWindow::executeLoadShowFile()
{
    int rv = 0;
    QString default_dir("");
    if (navstate.showfile && navstate.showfile->getFilePath().length()) {
        QFileInfo finfo(navstate.showfile->getFilePath());
        if (finfo.exists()) {
            default_dir = finfo.dir().canonicalPath();
        }
    }
    // TODO: "Would you like to save?"
    QString loadpath = QFileDialog::getOpenFileName(
                this,
                "Open File",
                default_dir,
                "ShowFiles (*.showfile)");
    if (loadpath.length()) {
        qDebug() << "<Loading> Load path chosen <" << loadpath << ">, loading.";
        rv = navstate.loadShowFile(loadpath);
        if (rv)
            return rv;
    }
    else {
        qDebug() << "<Loading> Load path not chosen <" << loadpath << ">, not loading.";
        return 0;
    }
    updateWidgets();
    return 0;
}

/* Button Press Callbacks */

void PlaybackMainWindow::saveShowFileButton()
{
    qDebug("<Button Press> Save Show File");
    int rv = executeSaveShowFile();
    if (rv)
        qWarning() << "<Saving> Error on save: " << rv;
}

void PlaybackMainWindow::openShowFileButton()
{
    qDebug("<Button Press> Open Show File");
    int rv = executeLoadShowFile();
    if (rv)
        qWarning() << "<Loading> Error on load: " << rv;
}

void PlaybackMainWindow::newShowFileButton()
{
    qDebug("<Button Press> New Show File");
    int rv = executeNewShowFile();
    if (rv)
        qWarning() << "<Loading> Error on new: " << rv;
}

void PlaybackMainWindow::settingsShowFileButton()
{
    qDebug("<Button Press> Settings Show File");
}

void PlaybackMainWindow::addBackgroundPlaylistButton()
{
    qDebug("<Button Press> Add Background Music Playlist");
}
