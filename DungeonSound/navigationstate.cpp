#include "navigationstate.h"

NavigationState::NavigationState() :
    showfile(nullptr)
{}

void NavigationState::loadShowFile(const QString path)
{
    if (showfile) {
        delete showfile;
    }
    showfile = new ShowFile(path);
}

#ifdef USING_SAMPLE_SHOWFILE
void NavigationState::loadSampleShowFile()
{
    loadShowFile(QString(sample_saveloc));
}
#endif
