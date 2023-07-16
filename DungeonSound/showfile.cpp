#include "showfile.h"

#include <errno.h>
#include <QFile>
#include <QSaveFile>

ShowFile::ShowFile() :
    name("New Showfile")
    ,filepath("")
{

}

void ShowFile::saveToFile(QXmlStreamWriter& writer)
{
    // write header
    writer.setAutoFormatting(true);
    writer.setAutoFormattingIndent(2);
    writer.writeStartDocument();

    // save data
    writer.writeStartElement(ShowFile::element_name);
    writer.writeAttribute("title", name);

    // Background Playlists
    writer.writeStartElement(ShowFile::bg_playlists_element_name);
    foreach (Playlist x, bg_playlists) {
        x.saveToFile(writer);
    }
    writer.writeEndElement();//bg_playlists

    // close out
    writer.writeEndElement();//showfile
    writer.writeEndDocument();
}

int ShowFile::saveShowFile()
{
    // open file
    QSaveFile save_file(filepath);
    if (!save_file.open(QIODeviceBase::WriteOnly)) {
        qCritical() << "Error opening save file <" << filepath << ">";
        return -EIO;
    }
    QXmlStreamWriter writer(&save_file);

    saveToFile(writer);

    if (save_file.commit())
        return 0;
    else
        return -EIO;
}

int ShowFile::saveShowFile(const QString path)
{
    filepath = path;
    return saveShowFile();
}

int ShowFile::loadBackgroundPlaylists(QXmlStreamReader& reader)
{
    bg_playlists = Playlist_vec_t();

    while (reader.readNext() && !reader.isEndElement()) {
        if (reader.name() == Playlist::element_name){
            bg_playlists.push_back(Playlist(reader));
        }
    }

    return 0;
}

static void readNextSkipWhitespace(QXmlStreamReader& reader)
{
    while (reader.readNext() && reader.isWhitespace()){};
}

int ShowFile::loadFromFile(QXmlStreamReader& reader)
{
    reader.readNext();// start of document
    if (!reader.isStartDocument()) {
        qWarning() << "Could not read document start at beginning of save file";
        return -EINVAL;
    }
    reader.readNext();// ShowFile
    if (reader.name() != elementName()) {
        qWarning() << "Expected elementName " << elementName() << ", instead read " << reader.name();
        return -EINVAL;
    }

    QXmlStreamAttributes attrs = reader.attributes();

    if (attrs.hasAttribute("title")) {
        name = attrs.value("title").toString();
    }

    readNextSkipWhitespace(reader); // Background Playlists
    if (reader.name() == ShowFile::bg_playlists_element_name) {
        int rv = loadBackgroundPlaylists(reader);
        if (rv)
            return rv;
    }

    readNextSkipWhitespace(reader);//end Showfile
    if (!reader.isEndElement()){
        qWarning() << "Error reading xml element for ShowFile: did not read EndElement\n";
        qDebug() << "End document? " << reader.isEndDocument();
        return -EINVAL;
    }
    reader.readNext();
    if (!reader.isEndDocument()) {
        qWarning() << "Error reading EndDocument for ShowFile";
        return -EINVAL;
    }

    return 0;
}

int ShowFile::loadFile(const QString loadPath)
{
    QFile loadFile(loadPath);
    if (!loadFile.exists() ||
            !loadFile.open(QIODeviceBase::ReadOnly)) {
        qCritical() << "Error opening file <" << loadPath << ">";
        return -EIO;
    }
    QXmlStreamReader reader(&loadFile);

    int rv = loadFromFile(reader);

    loadFile.close();
    filepath = loadPath;
    return rv;
}

int ShowFile::renameBackgroundPlaylist(const Playlist *pl, QString& newName)
{
    ssize_t pl_idx = indexOfBackgroundPlaylist(pl);
    if (pl_idx < 0){
        qWarning("Could not find background playlist at %p", pl);
        return -EINVAL;
    }

    bg_playlists[pl_idx].name = newName;

    return 0;
}
