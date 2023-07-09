#include "playbackmainwindow.h"
#include "./ui_playbackmainwindow.h"

#include <QFile>

#include "flowlayout/flowlayout.h"
#include "navigationstate.h"

/* Playlist Button */

static QFrame *createPlaylistBox(const Playlist *pl, QWidget *parent)
{
    QFrame *rv = new QFrame(parent);

    // Create the box
    QSizePolicy sp;
    sp.setHorizontalPolicy(QSizePolicy::Fixed);
    sp.setVerticalPolicy(QSizePolicy::Fixed);
    sp.setHorizontalStretch(0);
    sp.setVerticalStretch(0);
    rv->setSizePolicy(sp);
    rv->setMinimumSize(QSize(256, 128));
    rv->setMaximumSize(QSize(256, 128));
    rv->setFrameShape(QFrame::Panel);
    rv->setFrameShadow(QFrame::Raised);
    rv->setLineWidth(1);
    QGridLayout *glayout = new QGridLayout(rv);
    rv->setLayout(glayout);

    // Put stuff in the box
    // Settings button
    QPushButton *setbut = new QPushButton(rv);
    setbut->setIcon(QIcon(":/DungeonSound/Icons/24x24 gear.png"));
    glayout->addWidget(setbut, 0, 0, Qt::AlignRight);
    glayout->setRowStretch(0, 1);
    QLabel *plNameLabel = new QLabel(rv);
    plNameLabel->setText(pl->name);
    plNameLabel->setAlignment(Qt::AlignCenter);
    glayout->addWidget(plNameLabel, 1, 0, Qt::AlignCenter);
    glayout->setRowStretch(1, 1);
    QSpacerItem *spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    glayout->addItem(spacer, 2, 0, Qt::AlignCenter);
    glayout->setRowStretch(2, 1);


    return rv;
}

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

int PlaybackMainWindow::updateBackgroundPlaylistsWidget()
{
    QWidget *container = ui->backgroundPlaylistsCollection;
    QFrame *addPlaylistFrame = ui->AddPlaylistFrame;
    // Remove the "add" frame, keep it around
    QLayoutItem *addLayoutItem = container->layout()->takeAt(container->layout()->indexOf(addPlaylistFrame));

    // Look at our show logic
    ShowFile *sf = navstate.showfile;
    // No ShowFile, so just re-add "add" button (?)
    if (!sf)
    {
        container->layout()->addItem(addLayoutItem);
        return 0;
    }
    for (int i = 0; i < sf->getNumBackgroundPlaylists(); ++i) {
        QFrame *pl_box = createPlaylistBox(sf->getBackgroundPlaylist(i), container);
        container->layout()->addWidget(pl_box);
    }
    container->layout()->addItem(addLayoutItem);
    return 0;
}

int PlaybackMainWindow::setupBackgroundPlaylistsWidget()
{
    // Grab the extant items
    QWidget *container = ui->backgroundPlaylistsCollection;
    QFrame *addPlaylistFrame = ui->AddPlaylistFrame;
    // Remove the "add" frame, keep it around
    QLayoutItem *addLayoutItem = container->layout()->takeAt(container->layout()->indexOf(addPlaylistFrame));

    // Change the layout to the flow layout
    FlowLayout *flayout = new FlowLayout(container);
    delete container->layout();
    container->setLayout(flayout);
    flayout->addItem(addLayoutItem);

    return updateBackgroundPlaylistsWidget();
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
#endif
    setupBackgroundPlaylistsWidget();

}

PlaybackMainWindow::~PlaybackMainWindow()
{
    delete ui;
}

/* Button Press Callbacks */

void PlaybackMainWindow::saveShowFileButton()
{
    qDebug("Button Press: Save Show File");
}

void PlaybackMainWindow::openShowFileButton()
{
    qDebug("Button Press: Open Show File");
}

void PlaybackMainWindow::newShowFileButton()
{
    qDebug("Button Press: New Show File");
}

void PlaybackMainWindow::settingsShowFileButton()
{
    qDebug("Button Press: Settings Show File");
}
