#include "soundsample.h"

#include <QDebug>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaMetaData>

SoundSample::SoundSample()
{

}

SoundSample::SoundSample(QXmlStreamReader& reader)
{
    loadFromFile(reader);
}

void SoundSample::setResourceURL(QString url)
{
    resource_url = url;
    rurl = QUrl::fromLocalFile(resource_url);

    QMediaPlayer mreader = QMediaPlayer();
    mreader.setSource(rurl);
    // TODO: figure out how to load this data synchronously
    //int duration_ms = mreader.duration();
    //total_time = duration_ms / 1000.0;
    total_time = 0.0;
}

bool SoundSample::operator==(const SoundSample& rhs) const
{
    if (getResourceURL() != rhs.getResourceURL())
        return false;
    if (name != rhs.name)
        return false;
    if (start_timestamp != rhs.start_timestamp)
        return false;
    if (end_timestamp != rhs.end_timestamp)
        return false;
    if (volume_factor != rhs.volume_factor)
        return false;

    return true;
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

    reader.readNext();
    if (!reader.isEndElement()) {
        qCritical() << "Error reading xml element for SoundSample: did not read EndElement\n";
    }
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
    writer.writeEndElement();

    return 0;
}
