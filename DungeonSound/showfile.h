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
    int loadFile(const QString loadPath);

    void appendBackgroundPlaylist(Playlist& pl){
        bg_playlists.push_back(pl);
    }

    int renameBackgroundPlaylist(const Playlist *pl, QString& newName);

    /* Accessors */
    inline int getNumBackgroundPlaylists() const{
        return bg_playlists.length();
    }

    const Playlist* getBackgroundPlaylist(int index) const {
        if (index < 0 || index > bg_playlists.length())
            return nullptr;
        return &bg_playlists[index];
    }

    /**
     * @brief Find the index of the given playlist, in the worst possible way (pointer math)
     * @param pl address of playlist to find
     * @return index of playlist
     */
    ssize_t indexOfBackgroundPlaylist(const Playlist *pl) const {
        const Playlist *pl_0 = bg_playlists.constData();
        long diff = pl - pl_0;

        if (diff < 0)
            return -1;
        return diff;
    }

    inline QString getFilePath() const {
        return filepath;
    }

    /* Saving/Loading */
    int saveShowFile();
    int saveShowFile(const QString path);

    static inline const QString element_name = QString("ShowFile");
    inline QString elementName(){
        return element_name;
    }


protected:
    /// "Title" of the Show File
    QString name;
    /// Filename that this file was last saved to or loaded from
    QString filepath;

    static inline const QString bg_playlists_element_name = QString("BackgroundPlaylists");

    int loadFromFile(QXmlStreamReader& reader);
    int loadBackgroundPlaylists(QXmlStreamReader& reader);
    void saveToFile(QXmlStreamWriter& writer);

    /**
     * @brief List of all playlists meant to be played as background music
     *
     * This may end up being the only data store, but its purpose is to specifically
     * have playlists that cannot be played over one another
     */
    Playlist_vec_t bg_playlists;

};

#endif // SHOWFILE_H
