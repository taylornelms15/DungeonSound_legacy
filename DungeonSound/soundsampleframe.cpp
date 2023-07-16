#include "soundsampleframe.h"
#include "ui_soundsampleframe.h"

static const QSize iconSize = QSize(36, 36);

void SoundSampleFrame::setPlaybackButtonProperties(QPushButton *button, QStyle::StandardPixmap pm)
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

void SoundSampleFrame::setupUiElements()
{
    setPlaybackButtonProperties(playButton,  QStyle::SP_MediaPlay);
    setPlaybackButtonProperties(openButton, QStyle::SP_DialogOpenButton);

    updateWidgets();
}

void SoundSampleFrame::updateWidgets()
{
    qDebug() << "<UI> " << __PRETTY_FUNCTION__;
    if (ss)
        nameLineEdit->setText(ss->name);
}

SoundSampleFrame::SoundSampleFrame(const SoundSample *ss, QWidget *parent) :
    QFrame(parent)
    , ss(ss)
    , ui(new Ui::SoundSampleFrame)
{
    ui->setupUi(this);

    openButton = ui->openButton;
    playButton = ui->playButton;
    nameLineEdit = ui->nameLineEdit;

    setupUiElements();
}

SoundSampleFrame::~SoundSampleFrame()
{
    delete ui;
}
