#ifndef PLAYLISTEDITWINDOW_H
#define PLAYLISTEDITWINDOW_H

#include <QFrame>

namespace Ui {
class PlaylistEditWindow;
}

class PlaylistEditWindow : public QFrame
{
    Q_OBJECT

public:
    explicit PlaylistEditWindow(QWidget *parent = nullptr);
    ~PlaylistEditWindow();

protected:
    // Widget manipulation
    int setupSoundSampleWidget;
    int updateSoundSampleWidget;
    void clearSoundSampleWidget();

private:
    Ui::PlaylistEditWindow *ui;
};

#endif // PLAYLISTEDITWINDOW_H
