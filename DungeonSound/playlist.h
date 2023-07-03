#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QString>
#include <QList>
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
class Playlist
{
public:
    Playlist();

    /* Accessors */
    inline int getNumSoundSamples(){
        return sampleList.length();
    }

    /* Mutable Members */
    /// Visible name of playlist
    QString name;

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
