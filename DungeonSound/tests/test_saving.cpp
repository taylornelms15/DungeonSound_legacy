#include <gtest/gtest.h>
#include <QBuffer>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "soundsample.h"
#include "playlist.h"
#include "showfile.h"

/* Helpers */

static void startOutputBufferFile(QXmlStreamWriter& writer)
{
    writer.setAutoFormatting(true);
    writer.setAutoFormattingIndent(2);
    writer.writeStartDocument();
}

static void endOutputBufferFile(QXmlStreamWriter& writer)
{
    writer.writeEndDocument();
}

/* Tests */

TEST(TestSavingInit, BasicAssertions) {
    ShowFile *sf = new ShowFile();
    Playlist *pl = new Playlist();
    SoundSample *ss = new SoundSample();

    EXPECT_NE(sf, nullptr);
    EXPECT_NE(pl, nullptr);
    EXPECT_NE(ss, nullptr);

    delete sf;
    delete pl;
    delete ss;
}

TEST(TestSavingSoundSample, BasicAssertions) {
    SoundSample ss = SoundSample();

    QBuffer outbuf;
    outbuf.open(QBuffer::ReadWrite);
    QXmlStreamWriter writer(&outbuf);
    startOutputBufferFile(writer);



    endOutputBufferFile(writer);

}

TEST(TestSavingPlaylist, BasicAssertions) {
    Playlist pl = Playlist();

}
