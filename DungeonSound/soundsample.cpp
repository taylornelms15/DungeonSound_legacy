#include "soundsample.h"

#include <QDebug>

SoundSample::SoundSample()
{

}

SoundSample::SoundSample(QXmlStreamReader& reader)
{
    loadFromFile(reader);
}

/* Saving/Loading */

int SoundSample::loadFromFile(QXmlStreamReader& reader)
{
    if (!reader.isStartElement() ||
        (reader.name() != elementName()))
    {
        qCritical() << "Error reading xml element for SoundSample; isStartElement " << (reader.isStartElement() ? "true" : "false");
        qCritical() << "reader.name(): " << reader.name();
        return -EIO;
    }

    QXmlStreamAttributes attrs = reader.attributes();

    if (attrs.hasAttribute("title")) {
        name = attrs.value("title").toString();
    }

    LOAD_ATTRIBUTE(reader, start_timestamp, Float)
    LOAD_ATTRIBUTE(reader, end_timestamp, Float)
    LOAD_ATTRIBUTE(reader, volume_factor, Float)
    LOAD_ATTRIBUTE(reader, resource_url, String)
    LOAD_ATTRIBUTE(reader, total_time, Float)
    LOAD_ATTRIBUTE(reader, total_playable_time, Float)

    return 0;
}

int SoundSample::saveToFile(QXmlStreamWriter& writer) const
{
    writer.writeStartElement(elementName());
    writer.writeAttribute("title", name);
    SAVE_ATTRIBUTE_NUMERIC(writer, start_timestamp)
    SAVE_ATTRIBUTE_NUMERIC(writer, end_timestamp)
    SAVE_ATTRIBUTE_NUMERIC(writer, volume_factor)
    SAVE_ATTRIBUTE(writer, resource_url)
    SAVE_ATTRIBUTE_NUMERIC(writer, total_time)
    SAVE_ATTRIBUTE_NUMERIC(writer, total_playable_time)
    writer.writeEndElement();

    return 0;
}
