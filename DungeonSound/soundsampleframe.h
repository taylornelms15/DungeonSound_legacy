#ifndef SOUNDSAMPLEFRAME_H
#define SOUNDSAMPLEFRAME_H

#include <QFrame>
#include <QPushButton>
#include <QLineEdit>

namespace Ui {
class SoundSampleFrame;
}

class SoundSampleFrame : public QFrame
{
    Q_OBJECT

public:
    explicit SoundSampleFrame(QWidget *parent = nullptr);
    ~SoundSampleFrame();

protected:
    QPushButton *openButton;
    QPushButton *playButton;
    QLineEdit *nameLineEdit;

private:
    Ui::SoundSampleFrame *ui;
};

#endif // SOUNDSAMPLEFRAME_H
