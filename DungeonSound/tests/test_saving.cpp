#include <gtest/gtest.h>
#include <QBuffer>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "soundsample.h"
#include "playlist.h"
#include "showfile.h"

/* Helpers */

#define ASSERT_QSTREQ(EXPECT, ACTUAL) \
    ASSERT_STREQ(EXPECT.toStdString().c_str(), ACTUAL.toStdString().c_str())

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
    // Create structures, prep output
    SoundSample ss = SoundSample();
    QBuffer outbuf;
    outbuf.open(QBuffer::ReadWrite);
    QXmlStreamWriter writer(&outbuf);
    startOutputBufferFile(writer);

    // Fill data
    QString resource_url("G:/My Drive/Dungeons and Dragons/Castlevania Campaign I Guess/CastlevaniaMusic/DungeonCrawl/1-01 Dracula's Theme.mp3");
    QString title("Dracula's Theme");
    ss.setResourceURL(resource_url);
    ss.start_timestamp = 0.0;
    ss.end_timestamp = 114.5;
    ss.name = title;
    ss.volume_factor = 0.8;

    // Save data
    ss.saveToFile(writer);
    endOutputBufferFile(writer);

    // Check XML when reading
    outbuf.seek(0);
    QXmlStreamReader reader(&outbuf);
    reader.readNext();
    ASSERT_TRUE(reader.isStartDocument());
    SoundSample ss_check = SoundSample(reader);
    reader.readNext();
    ASSERT_TRUE(reader.isEndDocument());

    // Check data
    ASSERT_QSTREQ(title, ss_check.name);
    ASSERT_QSTREQ(resource_url, ss_check.getResourceURL());
    ASSERT_EQ(ss.start_timestamp, ss_check.start_timestamp);
    ASSERT_EQ(ss.end_timestamp, ss_check.end_timestamp);
    ASSERT_EQ(ss.volume_factor, ss_check.volume_factor);

}

TEST(TestSavingPlaylist, BasicAssertions) {
    Playlist pl = Playlist();

}
