#ifndef PLAYLISTEDITWINDOW_H
#define PLAYLISTEDITWINDOW_H

#include <QFrame>

#include "playlist.h"
#include "navigationstate.h"

namespace Ui {
class PlaylistEditWindow;
}

class PlaylistEditWindow : public QFrame
{
    Q_OBJECT

public:
    explicit PlaylistEditWindow(const Playlist *pl, QWidget *parent = nullptr);
    ~PlaylistEditWindow();

    void setPlaylist(const Playlist *pl) {
        this->pl = pl;
    }

public slots:
    void addSoundSampleButton();
    int updateWidgets();

signals:
    void needUpdateWidgets();

protected:
    // Widget manipulation
    int setupSoundSampleWidget();
    int updateSoundSampleWidget();
    void clearSoundSampleWidget();
    void updateNameLineEdit();

    int executeAddSoundSample();

    NavigationState& navstate;
    const Playlist* pl;

private slots:
    void playlistLineEditFinished();

private:
    Ui::PlaylistEditWindow *ui;
};

#endif // PLAYLISTEDITWINDOW_H
