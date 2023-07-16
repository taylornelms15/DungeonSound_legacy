#include "soundsampleframe.h"
#include "ui_soundsampleframe.h"

SoundSampleFrame::SoundSampleFrame(QWidget *parent) :
    QFrame(parent)
    , ui(new Ui::SoundSampleFrame)
{
    ui->setupUi(this);

    openButton = ui->openButton;
    playButton = ui->playButton;
    nameLineEdit = ui->nameLineEdit;
}

SoundSampleFrame::~SoundSampleFrame()
{
    delete ui;
}
