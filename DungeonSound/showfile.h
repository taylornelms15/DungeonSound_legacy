#ifndef SHOWFILE_H
#define SHOWFILE_H

#include <QString>
#include <QList>
#include <QDebug>
#include "playlist.h"

typedef QList<Playlist> Playlist_vec_t;

/**
 * @brief Class representing a collection of playlists that a GM may want to play
 *
 * This is primarily a collection of Playlist objects, and as such is designed to
 * retain knowledge of user-configurable attributes, but not any information
 * about current playback status.
 *
 * This is also the gateway to saving/loading files, and as such is best thought of
 * as capturing everything that you would expect to be maintained between executions
 * of the overall program.
 */
class ShowFile
{
public:
    /// Empty constructor (New File)
    ShowFile();
    /// Constructor with explicit filepath to load from
    ShowFile(const QString filepath)
        : filepath(filepath)
    {
        int retval;

        retval = loadFile(filepath);
        if (retval) {
            qDebug("Error loading file %s while constructing ShowFile (%d)",
                   qUtf8Printable(filepath), retval);
        }
    }

    int saveShowFile();
    int saveShowFile(const QString path);

protected:
    /// "Title" of the Show File
    QString name;
    /// Filename that this file was last saved to or loaded from
    QString filepath;

    int loadFile(const QString loadPath);

    /**
     * @brief List of all playlists meant to be played as background music
     *
     * This may end up being the only data store, but its purpose is to specifically
     * have playlists that cannot be played over one another
     */
    Playlist_vec_t bg_playlists;

};

#endif // SHOWFILE_H
