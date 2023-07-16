#ifndef PLAYLISTBOX_H
#define PLAYLISTBOX_H

#include <QFrame>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QStyle>

#include "playlist.h"

/**
 * @brief Visual component for representing a playlist on the playback main window
 */
class PlaylistBox : public QFrame
{
    Q_OBJECT

public:
    PlaylistBox(const Playlist *pl, QWidget *parent);

    const Playlist *playlist;
    QPushButton *editButton;
    QPushButton *playButton;
    QPushButton *pauseButton;
    QPushButton *stopButton;
    QLabel *plNameLabel;

protected:
    void setPlaybackButtonProperties(QPushButton *button, QStyle::StandardPixmap pm);

public slots:
    void editButtonPressed();
    void playButtonPressed();
    void pauseButtonPressed();
    void stopButtonPressed();

signals:
    void signalEditPlaylistPressed(const Playlist *pl);
};


#endif // PLAYLISTBOX_H
