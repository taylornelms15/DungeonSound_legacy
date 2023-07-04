#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <errno.h>
#include <QString>
#include <QList>

#include "savable.h"
#include "soundsample.h"

/**
 * @brief A convenience typdef to simplify templated syntax
 */
typedef QList<SoundSample> SoundSample_vec_t;

/**
 * @brief Class representing a playable collection of sound samples
 *
 * This class represents everything that can be set by a user for
 * designing a collection of sounds to later play back.
 * It is not meant to be reflective of any current playback status.
 */
class Playlist: public Savable
{
public:
    /// Empty constructor
    Playlist();
    /// Constructor for when reading from file
    Playlist(QXmlStreamReader& reader);

    /* Accessors */
    inline int getNumSoundSamples(){
        return sampleList.length();
    }

    /* Mutable Members */
    /// Visible name of playlist
    QString name;
    /// Factor to apply to relative volume for media
    float volume_factor;

    /* Saving */
    int saveToFile(QXmlStreamWriter& writer) const override;

    int loadFromFile(QXmlStreamReader& reader) override;

    QString elementName() const override{
        return QString("Playlist");
    }

protected:
    /**
     * @brief The core data store; a reference to all child SoundSamples.
     *
     * The order of the list is reflective of where it is located in the edit window.
     * Notably, the order should not change unless by the user's choice.
     */
    SoundSample_vec_t sampleList;

    /* TODO: icon */
};

#endif // PLAYLIST_H