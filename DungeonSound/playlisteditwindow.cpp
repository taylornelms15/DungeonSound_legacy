#include "playlisteditwindow.h"
#include "ui_playlisteditwindow.h"

#include <QPushButton>

#include "soundsampleframe.h"

/* Widget Manipulation */

void PlaylistEditWindow::clearSoundSampleWidget()
{
    QWidget *container = ui->soundSampleCollection;

    container->setUpdatesEnabled(false);
    QLayout* layout = container->layout();
    for(int i = layout->count() - 1; i >= 0; --i) {
        QLayoutItem* item = layout->takeAt(i);
        delete item->widget();
        delete item;
    }
    //qDeleteAll(container->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));
    container->setUpdatesEnabled(true);
}

int PlaylistEditWindow::updateSoundSampleWidget()
{
    QWidget *container = ui->soundSampleCollection;
    QFrame *addSoundSampleFrame = ui->AddSoundFrame;
    // Grab the extant items
    QLayoutItem *addLayoutItem = container->layout()->takeAt(container->layout()->indexOf(addSoundSampleFrame));

    clearSoundSampleWidget();
    container->layout()->activate();
    qDebug() << "SoundSampleWidget count: " << container->layout()->count();

    // Look at our show logic
    ShowFile *sf = navstate.showfile;
    // No ShowFile, so just re-add "add" button (?)
    if (!sf)
    {
        container->layout()->addItem(addLayoutItem);
        return 0;
    }
    ssize_t current_playlist_id = navstate.currentBackgroundPlaylistId;
    if (current_playlist_id < 0 ||
        !navstate.showfile ||
        current_playlist_id >= navstate.showfile->getNumBackgroundPlaylists())
    {
        qWarning("Attempting to load illegal playlist id (%zd, of possible %d)",
                 current_playlist_id, navstate.showfile->getNumBackgroundPlaylists());
    }
    const Playlist *current = navstate.showfile->getBackgroundPlaylist(current_playlist_id);
    if (this->pl != current){
        setPlaylist(current);
        updateNameLineEdit();//TODO: rethink order
    }
    if (current) {
        qDebug() << "<UI> Setting up " << pl->getNumSoundSamples() << " child rows";
        for (int i = 0; i < pl->getNumSoundSamples(); ++i) {
            const SoundSample *ss = pl->getSoundSample(i);
            container->layout()->addWidget(new SoundSampleFrame(ss, container));
        }
    }
    container->layout()->addItem(addLayoutItem);
    return 0;
}

int PlaylistEditWindow::setupSoundSampleWidget()
{
    // Connect the add button to the callback for this window
    connect(ui->AddSoundButton, &QAbstractButton::pressed,
            this, &PlaylistEditWindow::addSoundSampleButton);

    // Grab the extant items
    QWidget *container = ui->soundSampleCollection;
    QFrame *addSoundFrame = ui->AddSoundFrame;
    // Remove the "add" frame, keep it around
    QLayoutItem *addLayoutItem = container->layout()->takeAt(container->layout()->indexOf(addSoundFrame));

    // Change the layout to a new layout(?)
    delete container->layout();
    QVBoxLayout *vboxlayout = new QVBoxLayout(container);
    vboxlayout->addItem(addLayoutItem);

    // Update
    updateSoundSampleWidget();

    return 0;
}

void PlaylistEditWindow::updateNameLineEdit()
{
    if (pl){
        ui->PlaylistNameLineEdit->setText(pl->name);
    }

}

int PlaylistEditWindow::updateWidgets()
{
    qDebug() << "<UI> " << __PRETTY_FUNCTION__;

    updateNameLineEdit();

    // Update SoundSample list
    int rv;
    rv = updateSoundSampleWidget();
    if (rv)
        return rv;

    return rv;
}

/* Constructor, Destructor */

PlaylistEditWindow::PlaylistEditWindow(const Playlist *pl, QWidget *parent) :
    QFrame(parent)
    , pl(pl)
    , ui(new Ui::PlaylistEditWindow)
    , navstate(NavigationState::getInstance())
{
    ui->setupUi(this);

    QObject::connect(ui->PlaylistNameLineEdit, &QLineEdit::editingFinished,
                     this, &PlaylistEditWindow::playlistLineEditFinished);

    setupSoundSampleWidget();
    updateNameLineEdit();
}

PlaylistEditWindow::~PlaylistEditWindow()
{
    delete ui;
}

/* Operations */

int PlaylistEditWindow::executeAddSoundSample()
{
    return 0;
}

/* Button Press Callbacks */

void PlaylistEditWindow::addSoundSampleButton()
{
    qDebug("<Button Press> Add SoundSample");
}

void PlaylistEditWindow::playlistLineEditFinished()
{
    QString newName = ui->PlaylistNameLineEdit->text();
    qDebug() << "<Text Input> Playlist name changed to <" << newName << ">";
    // Check for empty string, do nothing if empty
    if (!newName.length()) {
        updateNameLineEdit();
        return;
    }
    if (pl && pl->name == newName) return;

    int rv = navstate.showfile->renameBackgroundPlaylist(pl, newName);
    if (rv)
        return;
    emit needUpdateWidgets();


}

