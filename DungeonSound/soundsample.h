#ifndef SOUNDSAMPLE_H
#define SOUNDSAMPLE_H

#include <QString>

/**
 * @brief Class for storing records of playable audio
 *
 * The SoundSample should record everything that can be set by a user
 * when designing a playlist, but is not meant to be reflective
 * of any current playback status on its own.
 */
class SoundSample
{
public:
    SoundSample();

    QString getResourceURL() const{
        return resource_url;
    };
    /**
     * @brief Sets the resource URL for the SoundSample
     *
     * This function sets the location of the media associated with the SoundSample.
     * It not only sets the relevant string, but also does a number of checks and
     * mutations internally to handle any issues in the
     */
    void setResourceURL();

    /* Mutable Members */
    /// Visible name of media
    QString name;
    /// User-defined start time in media, in seconds
    float start_timestamp;
    /// User-defined end time in media, in seconds
    float end_timestamp;
    /// Factor to apply to relative volume for media
    float volume_factor;
protected:
    /// Path to playable media
    QString resource_url;
    /// Total runtime of loaded media, in seconds
    float total_time;
    /// Playable runtime of loaded media, in seconds
    float total_playable_time;
};

#endif // SOUNDSAMPLE_H
