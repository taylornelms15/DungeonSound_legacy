#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QStyle>

#include "playbackmainwindow.h"
#include "playlisteditwindow.h"
#include "navigationstate.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void saveShowFileButton();
    void openShowFileButton();
    void newShowFileButton();
    void settingsShowFileButton();
    /**
     * @brief updateWidgets - Updates visual elements to match NavState
     * @return 0 on success, negative value otherwise
     */
    int updateWidgets();

protected:
    // Widget manipulation
    void setupWindowSwitcher();
    void loadPlaybackMainWindow();
    void loadPlaylistEditWindow();
    int setupIconBar();

    // File operations
    int executeSaveShowFile();
    int executeLoadShowFile();
    int executeNewShowFile();

    NavigationState& navstate;

    QStackedWidget *switcher;
    PlaybackMainWindow *pbmw;
    PlaylistEditWindow *plew;


private:
    Ui::MainWindow *ui;

    static inline const QString pbmw_path =
        QString(":/DungeonSound/UiForms/playbackmainwindow.ui");
    static inline const QString plew_path =
        QString(":/DungeonSound/UiForms/playlisteditwindow.ui");

    void setActionIconStyle(QAction *action, QStyle::StandardPixmap stylepixmap);
    void setActionIconStyle(QAction *action, QString iconstring);
};

#endif // MAINWINDOW_H
