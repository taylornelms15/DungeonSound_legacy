#ifndef PLAYBACKMAINWINDOW_H
#define PLAYBACKMAINWINDOW_H

#include <QFrame>
#include <QStyle>

#include "navigationstate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PlaybackMainWindow; }
QT_END_NAMESPACE

class PlaybackMainWindow : public QFrame
{
    Q_OBJECT

public:
    PlaybackMainWindow(QWidget *parent = nullptr);
    ~PlaybackMainWindow();

public slots:
    void addBackgroundPlaylistButton();
    /**
     * @brief updateWidgets - Updates visual elements to match NavState
     * @return 0 on success, negative value otherwise
     */
    int updateWidgets();

signals:
    void needUpdateWidgets();

protected:
    // Widget manipulation
    int setupBackgroundPlaylistsWidget();
    int updateBackgroundPlaylistsWidget();
    void clearBackgroundPlaylistsWidget();

    NavigationState& navstate;


private:
    Ui::PlaybackMainWindow *ui;

};
#endif // PLAYBACKMAINWINDOW_H
