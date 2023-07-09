#ifndef PLAYBACKMAINWINDOW_H
#define PLAYBACKMAINWINDOW_H

#include <QMainWindow>
#include <QStyle>

#include "navigationstate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PlaybackMainWindow; }
QT_END_NAMESPACE

class PlaybackMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    PlaybackMainWindow(QWidget *parent = nullptr);
    ~PlaybackMainWindow();

public slots:
    void saveShowFileButton();
    void openShowFileButton();
    void newShowFileButton();
    void settingsShowFileButton();
    void addBackgroundPlaylistButton();

protected:
    // Widget manipulation
    int setupIconBar();
    int setupBackgroundPlaylistsWidget();
    int updateBackgroundPlaylistsWidget();
    void clearBackgroundPlaylistsWidget();

    int updateWidgets();

    // File operations
    int executeSaveShowFile();
    int executeLoadShowFile();
    int executeNewShowFile();

    NavigationState& navstate;


private:
    Ui::PlaybackMainWindow *ui;

    void setActionIconStyle(QAction *action, QStyle::StandardPixmap stylepixmap);
    void setActionIconStyle(QAction *action, QString iconstring);
};
#endif // PLAYBACKMAINWINDOW_H
