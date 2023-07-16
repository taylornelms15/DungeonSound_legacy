#include "playlisteditwindow.h"
#include "ui_playlisteditwindow.h"

/* Widget Manipulation */

/* Constructor, Destructor */

PlaylistEditWindow::PlaylistEditWindow(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PlaylistEditWindow)
{
    ui->setupUi(this);

    setupSoundSampleWidget;
}

PlaylistEditWindow::~PlaylistEditWindow()
{
    delete ui;
}
