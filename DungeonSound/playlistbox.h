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
public:
    PlaylistBox(QWidget *parent, const Playlist *pl);

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
};


#endif // PLAYLISTBOX_H
