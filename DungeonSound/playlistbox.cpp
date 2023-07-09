#include "playlistbox.h"

#include <QGridLayout>
#include <QHBoxLayout>

static const QSize iconSize = QSize(36, 36);

void PlaylistBox::setPlaybackButtonProperties(QPushButton *button, QStyle::StandardPixmap pm)
{
    QSizePolicy sp;
    sp.setHorizontalPolicy(QSizePolicy::Fixed);
    sp.setVerticalPolicy(QSizePolicy::Fixed);
    sp.setHorizontalStretch(0);
    sp.setVerticalStretch(0);
    button->setSizePolicy(sp);
    button->setMinimumSize(iconSize);
    button->setMaximumSize(iconSize);
    button->setIcon(this->style()->standardIcon(pm));
}

PlaylistBox::PlaylistBox(QWidget *parent, const Playlist *pl)
    : QFrame(parent)
    , playlist(pl)
    , editButton(new QPushButton(this))
    , playButton(new QPushButton(this))
    , pauseButton(new QPushButton(this))
    , stopButton(new QPushButton(this))
    , plNameLabel(new QLabel(this))
{
    // Create the box
    QSizePolicy sp;
    sp.setHorizontalPolicy(QSizePolicy::Fixed);
    sp.setVerticalPolicy(QSizePolicy::Fixed);
    sp.setHorizontalStretch(0);
    sp.setVerticalStretch(0);
    setSizePolicy(sp);
    setMinimumSize(QSize(256, 128));
    setMaximumSize(QSize(256, 128));
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Raised);
    setLineWidth(1);
    QGridLayout *glayout = new QGridLayout(this);
    setLayout(glayout);

    // Put stuff in the box
    // Settings button
    editButton->setIcon(QIcon(":/DungeonSound/Icons/24x24 pencil.png"));
    editButton->setMinimumSize(iconSize);
    editButton->setMaximumSize(iconSize);
    glayout->addWidget(editButton, 0, 0, Qt::AlignRight);
    glayout->setRowStretch(0, 1);
    connect(editButton, &QAbstractButton::pressed, this, &PlaylistBox::editButtonPressed);
    // Name label
    plNameLabel->setText(pl->name);
    plNameLabel->setAlignment(Qt::AlignCenter);
    glayout->addWidget(plNameLabel, 1, 0, Qt::AlignCenter);
    glayout->setRowStretch(1, 1);
    // Playback controls
    QHBoxLayout *hlayout = new QHBoxLayout();
    glayout->addLayout(hlayout, 2, 0, Qt::AlignCenter);
    setPlaybackButtonProperties(playButton,  QStyle::SP_MediaPlay);
    setPlaybackButtonProperties(pauseButton,  QStyle::SP_MediaPause);
    setPlaybackButtonProperties(stopButton,  QStyle::SP_MediaStop);
    hlayout->addWidget(playButton, 1, Qt::AlignRight);
    hlayout->addWidget(pauseButton, 0, Qt::AlignCenter);
    hlayout->addWidget(stopButton, 1, Qt::AlignLeft);
    connect(playButton, &QAbstractButton::pressed, this, &PlaylistBox::playButtonPressed);
    connect(pauseButton, &QAbstractButton::pressed, this, &PlaylistBox::pauseButtonPressed);
    connect(stopButton, &QAbstractButton::pressed, this, &PlaylistBox::stopButtonPressed);
    glayout->setRowStretch(2, 1);


}

void PlaylistBox::editButtonPressed()
{
    qDebug() << "edit button pressed for playlist " << playlist->name;
}

void PlaylistBox::playButtonPressed()
{
    qDebug() << "play button pressed for playlist " << playlist->name;
}

void PlaylistBox::pauseButtonPressed()
{
    qDebug() << "pause button pressed for playlist " << playlist->name;
}

void PlaylistBox::stopButtonPressed()
{
    qDebug() << "stop button pressed for playlist " << playlist->name;
}
