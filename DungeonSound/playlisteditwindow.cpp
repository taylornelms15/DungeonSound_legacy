#include "playlisteditwindow.h"
#include "ui_playlisteditwindow.h"

PlaylistEditWindow::PlaylistEditWindow(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PlaylistEditWindow)
{
    ui->setupUi(this);
}

PlaylistEditWindow::~PlaylistEditWindow()
{
    delete ui;
}
