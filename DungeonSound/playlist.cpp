#include "playlist.h"

#include <errno.h>
#include <QDebug>

Playlist::Playlist()
{

}

Playlist::Playlist(QXmlStreamReader& reader)
{
    loadFromFile(reader);
}

/* Saving/Loading */

int Playlist::loadFromFile(QXmlStreamReader& reader)
{
    if (!reader.isStartElement() ||
        (reader.name() != elementName()))
    {
        qCritical() << "Error reading xml element for Playlist; isStartElement " << (reader.isStartElement() ? "true" : "false");
        qCritical() << "reader.name(): " << reader.name();
        return -EIO;
    }

    QXmlStreamAttributes attrs = reader.attributes();

    if (attrs.hasAttribute("title")) {
        name = attrs.value("title").toString();
    }

    sampleList = SoundSample_vec_t();
    while(reader.readNextStartElement()) {
        if (!reader.name().compare(QString("SoundSample"))) {
            sampleList.push_back(SoundSample(reader));
        }// if this element is a sound sample
    }
    LOAD_ATTRIBUTE(reader, volume_factor, Float)

    return 0;
}

int Playlist::saveToFile(QXmlStreamWriter& writer) const
{
    writer.writeStartElement(elementName());
    writer.writeAttribute("title", name);
    SAVE_ATTRIBUTE_NUMERIC(writer, volume_factor)

    foreach (SoundSample x, sampleList) {
        x.saveToFile(writer);
    }
    writer.writeEndElement();
    return 0;
}
