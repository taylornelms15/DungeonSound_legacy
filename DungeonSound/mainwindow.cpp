#include "mainwindow.h"
#include "playbackmainwindow.h"
#include "ui_mainwindow.h"

#include <QUiLoader>
#include <QFile>
#include <QFileDialog>

/* UI Set-Up */

void MainWindow::setActionIconStyle(QAction *action, QString iconstring)
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

void MainWindow::setActionIconStyle(QAction *action, QStyle::StandardPixmap stylepixmap)
{
    action->setIcon(this->style()->standardIcon(stylepixmap));
}

int MainWindow::setupIconBar()
{
    // Set images
    setActionIconStyle(ui->actionBackShowFile, QStyle::SP_FileDialogBack);
    setActionIconStyle(ui->actionNewShowFile, QStyle::SP_FileIcon);
    setActionIconStyle(ui->actionOpenShowFile, QStyle::SP_DialogOpenButton);
    setActionIconStyle(ui->actionSaveShowFile, QStyle::SP_DialogSaveButton);
    setActionIconStyle(ui->actionSettingsShowFile, "gear");

    // Connect button callbacks
    connect(ui->actionNewShowFile, &QAction::triggered, this, &MainWindow::newShowFileButton);
    connect(ui->actionOpenShowFile, &QAction::triggered, this, &MainWindow::openShowFileButton);
    connect(ui->actionSaveShowFile, &QAction::triggered, this, &MainWindow::saveShowFileButton);
    connect(ui->actionSettingsShowFile, &QAction::triggered, this, &MainWindow::settingsShowFileButton);
    connect(ui->actionBackShowFile, &QAction::triggered, this, &MainWindow::backButton);

    return 0;
}

void MainWindow::loadPlaybackMainWindow()
{
    QUiLoader loader;
    QFile f(MainWindow::pbmw_path);
    if (!f.open(QIODevice::ReadOnly)){
        qCritical() << "Could not find file <" << MainWindow::pbmw_path << ">: " << f.errorString();
        exit(1);
    }
    pbmw = new PlaybackMainWindow(loader.load(&f));
    f.close();

    QObject::connect(pbmw, &PlaybackMainWindow::needUpdateWidgets,
                     this, &MainWindow::updateWidgets);

    switcher->insertWidget(WindowId::PLAYBACK, pbmw);
}

void MainWindow::loadPlaylistEditWindow()
{
    QUiLoader loader;
    QFile f(MainWindow::plew_path);
    if (!f.open(QIODevice::ReadOnly)){
        qCritical() << "Could not find file <" << MainWindow::plew_path << ">: " << f.errorString();
        exit(1);
    }
    plew = new PlaylistEditWindow(loader.load(&f));
    f.close();
    switcher->insertWidget(WindowId::PLAYLIST, plew);
}

void MainWindow::setupWindowSwitcher()
{
    switcher = ui->MainStackWidget;
    /* clear any vestigial widgets before adding */
    while (switcher->count()) {
        QWidget *current = switcher->currentWidget();
        switcher->removeWidget(current);
        delete current;
    }
}

int MainWindow::updateWidgets()
{
    qDebug() << "<UI> " << __PRETTY_FUNCTION__;
    int rv = 0;

    /* Update toolbar */
    switch(navstate.currentWindowId){
    case WindowId::PLAYBACK:
        if (ui->actionBackShowFile->isEnabled()) {
            ui->actionBackShowFile->setEnabled(false);
        }
        break;
    case WindowId::PLAYLIST:
        if (!ui->actionBackShowFile->isEnabled()) {
            ui->actionBackShowFile->setEnabled(true);
        }
        break;
    default:
        qWarning() << "Unknown window id " << navstate.currentWindowId;
        return -EINVAL;
    }

    if (navstate.currentWindowId == WindowId::PLAYBACK) {

    }

    /* Update switcher focus */
    if (switcher->currentIndex() != navstate.currentWindowId) {
        switcher->setCurrentIndex(navstate.currentWindowId);
    }

    /* Update child windows */
    if (pbmw) {
        rv = pbmw->updateWidgets();
        if (rv)
            return rv;
    }
    if (plew) {
        //TODO: plew updateWidgets()
    }

    return rv;
}

/* Constructor, Destructor */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , navstate(NavigationState::getInstance())
{
    QObject::connect(this, &MainWindow::needUpdateWidgets,
                     this, &MainWindow::updateWidgets);

    ui->setupUi(this);

    /* Set up icons */
    setupIconBar();

    /* Set up window switcher */
    setupWindowSwitcher();
    loadPlaybackMainWindow();
    loadPlaylistEditWindow();
    switcher->setCurrentWidget(pbmw);

    /* Load Showfile */
#ifdef USING_SAMPLE_SHOWFILE
    navstate.loadSampleShowFile();
#else
    navstate.loadNewShowFile();
#endif

}

MainWindow::~MainWindow()
{
    delete ui;
}

/* File Operations */

int MainWindow::executeSaveShowFile()
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

int MainWindow::executeNewShowFile()
{
    // TODO: "Would you like to save?"
    qDebug() << "<Loading> Loading new show file";
    navstate.loadNewShowFile();
    emit needUpdateWidgets();
    return 0;
}

int MainWindow::executeLoadShowFile()
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
    emit needUpdateWidgets();
    return 0;
}

/* Navigation */
int MainWindow::executeBackButton()
{
    int rv = navstate.executeBack();
    if (rv)
        return rv;
    emit needUpdateWidgets();
    return rv;
}

/* Button Press Callbacks */

void MainWindow::saveShowFileButton()
{
    qDebug("<Button Press> Save Show File");
    int rv = executeSaveShowFile();
    if (rv)
        qWarning() << "<Saving> Error on save: " << rv;
}

void MainWindow::openShowFileButton()
{
    qDebug("<Button Press> Open Show File");
    int rv = executeLoadShowFile();
    if (rv)
        qWarning() << "<Loading> Error on load: " << rv;
}

void MainWindow::newShowFileButton()
{
    qDebug("<Button Press> New Show File");
    int rv = executeNewShowFile();
    if (rv)
        qWarning() << "<Loading> Error on new: " << rv;
}

void MainWindow::settingsShowFileButton()
{
    qDebug("<Button Press> Settings Show File");
}

void MainWindow::backButton()
{
    qDebug("<Button Press> Back");
    int rv = executeBackButton();
    if (rv)
        qWarning() << "<Navigation> Error on back: " << rv;
}
