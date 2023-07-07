#include "soundsample.h"

#include <QDebug>
#include <QFileInfo>
#include <QElapsedTimer>
#include <QtMultimedia/QAudioDecoder>
#include <QtMultimedia/QMediaMetaData>
#include <id3v2lib.h>
#ifdef HAVE_FFMPEG
extern "C" {
#include <libavformat/avformat.h>
}
#endif

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

    QElapsedTimer timer;
    timer.start();
    ID3v2_Tag *tag = ID3v2_read_tag(url.toStdString().c_str());
    if (tag == NULL)
        qCritical() << "Error reading tag from string <" << url << ">";
    else
        qDebug() << "Read tag! Elapsed: " << timer.elapsed() << "ms";

#ifdef HAVE_FFMPEG
    AVFormatContext* pFormatCtx = avformat_alloc_context();
    avformat_open_input(&pFormatCtx, url.toStdString(), NULL, NULL);
    int64_t duration = pFormatCtx->duration;
    // etc
    avformat_close_input(&pFormatCtx);
    avformat_free_context(pFormatCtx);
#endif
/*
    QAudioDecoder decoder = QAudioDecoder();
    decoder.setSource(rurl);
    QFileInfo finfo(url);
    QAudioBuffer fbuf = decoder.read();
    qDebug() << QFileInfo(url);
    qDebug() << decoder.isSupported();
    qDebug() << decoder.audioFormat();
    qDebug() << decoder.isDecoding();
    qDebug() << decoder.source();
    qDebug() << decoder.read().format();
    */
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
