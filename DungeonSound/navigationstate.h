#ifndef NAVIGATIONSTATE_H
#define NAVIGATIONSTATE_H

#include "development_headers.h"
#include "showfile.h"

/**
 * @brief Keeps track of how a loaded ShowFile is active without UI dependencies
 *
 * This is a non-savable record of things like "which showfile is loaded, what's being played back" etc.
 * Basically, it holds the active logic of the state the program is in,
 * but without being dependent on the UI components.
 *
 * Notably, is set up as a singleton class.
 */
class NavigationState
{
public:
    NavigationState();

    static NavigationState& getInstance() {
        static NavigationState instance;
        return instance;
    }

    /**
     * @brief loadShowFile Loads the ShowFile at the given path
     * @param path Filepath to load
     * @return 0 on success, negative value on error
     */
    int loadShowFile(const QString path);
#ifdef USING_SAMPLE_SHOWFILE
    /**
     * @brief loadSampleShowFile Loads the sample showfile
     * @return 0 on success, negative value on error
     */
    int loadSampleShowFile();
#endif
    /**
     * @brief loadNewShowFile Loads a "new" showfile
     *
     * Mostly, this clears the internal ShowFile and
     * instantiates a new one.
     */
    void loadNewShowFile();

    ShowFile *showfile;

// singleton magic
    NavigationState(NavigationState const&) = delete;
    void operator=(NavigationState const&) = delete;
};

#endif // NAVIGATIONSTATE_H
