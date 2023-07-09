#include "navigationstate.h"

NavigationState::NavigationState() :
    showfile(nullptr)
{}

void NavigationState::loadNewShowFile()
{
    showfile = new ShowFile();
}

int NavigationState::loadShowFile(const QString path)
{
    if (showfile) {
        delete showfile;
    }
    showfile = new ShowFile();
    return showfile->loadFile(path);
}

#ifdef USING_SAMPLE_SHOWFILE
int NavigationState::loadSampleShowFile()
{
    return loadShowFile(QString(sample_saveloc));
}
#endif
