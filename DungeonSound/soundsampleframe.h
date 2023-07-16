#ifndef SOUNDSAMPLEFRAME_H
#define SOUNDSAMPLEFRAME_H

#include <QFrame>
#include <QPushButton>
#include <QLineEdit>
#include <QStyle>

#include "soundsample.h"

namespace Ui {
class SoundSampleFrame;
}

class SoundSampleFrame : public QFrame
{
    Q_OBJECT

public:
    explicit SoundSampleFrame(const SoundSample *ss, QWidget *parent = nullptr);
    ~SoundSampleFrame();

protected:
    QPushButton *openButton;
    QPushButton *playButton;
    QLineEdit *nameLineEdit;

    void updateWidgets();
    void setupUiElements();
    void setPlaybackButtonProperties(QPushButton *button, QStyle::StandardPixmap pm);

    const SoundSample *ss;

private:
    Ui::SoundSampleFrame *ui;
};

#endif // SOUNDSAMPLEFRAME_H
