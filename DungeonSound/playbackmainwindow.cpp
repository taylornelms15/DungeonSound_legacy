#include "playbackmainwindow.h"
#include "./ui_playbackmainwindow.h"

#include <QFile>

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

/* Constructor/Destructor */

PlaybackMainWindow::PlaybackMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PlaybackMainWindow)
{
    ui->setupUi(this);
    setupIconBar();
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
