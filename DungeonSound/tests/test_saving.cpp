#include <gtest/gtest.h>
#include <QBuffer>
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "soundsample.h"
#include "playlist.h"
#include "showfile.h"

#include "development_headers.h"

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

static SoundSample createTestSoundSample(QString resource_url, QString title) {

    SoundSample ss = SoundSample();

    ss.setResourceURL(resource_url);
    ss.start_timestamp = 0.0;
    ss.end_timestamp = 114.5;
    ss.name = title;
    ss.volume_factor = 0.8;

    return ss;
}

static Playlist createTestPlaylist(QString title, SoundSample* samples, int num_samples) {

    Playlist pl = Playlist();

    pl.name = title;
    for(int i = 0; i < num_samples; ++i){
        pl.appendSoundSample(samples[i]);
    }
    pl.volume_factor = 0.9;

    return pl;
}

TEST(TestSavingSoundSample, BasicAssertions) {
    // Create structures, prep output
    QBuffer outbuf;
    outbuf.open(QBuffer::ReadWrite);
    QXmlStreamWriter writer(&outbuf);
    startOutputBufferFile(writer);

    // Fill data
    QString resource_url("G:/My Drive/Dungeons and Dragons/Castlevania Campaign I Guess/CastlevaniaMusic/DungeonCrawl/1-01 Dracula's Theme.mp3");
    QString title("Dracula's Theme");
    SoundSample ss = createTestSoundSample(resource_url, title);

    // Save data
    ss.saveToFile(writer);
    endOutputBufferFile(writer);

    // Check XML when reading
    outbuf.seek(0);
    QXmlStreamReader reader(&outbuf);
    reader.readNext();
    ASSERT_TRUE(reader.isStartDocument());
    reader.readNext();
    SoundSample ss_check = SoundSample(reader);
    reader.readNext();
    ASSERT_TRUE(reader.isEndDocument());

    // Check data
    ASSERT_EQ(ss, ss_check);
}

void assertPlaylistsEqual(const Playlist *pl1, const Playlist *pl2)
{
    ASSERT_EQ(pl1->volume_factor, pl2->volume_factor);
    ASSERT_EQ(pl1->getNumSoundSamples(), pl2->getNumSoundSamples());
    ASSERT_QSTREQ(pl1->name, pl2->name);

    for(int i = 0; i < pl1->getNumSoundSamples(); ++i) {
        ASSERT_EQ(*pl1->getSoundSample(i), *pl2->getSoundSample(i));
    }
}

TEST(TestSavingPlaylist, BasicAssertions) {
    // Create structures, prep output
    QBuffer outbuf;
    outbuf.open(QBuffer::ReadWrite);
    QXmlStreamWriter writer(&outbuf);
    startOutputBufferFile(writer);

    // Fill data
    QString resource_url_1("G:/My Drive/Dungeons and Dragons/Castlevania Campaign I Guess/CastlevaniaMusic/DungeonCrawl/1-01 Dracula's Theme.mp3");
    QString resource_url_2("G:/My Drive/Dungeons and Dragons/Castlevania Campaign I Guess/CastlevaniaMusic/DungeonCrawl/2-12 Carmilla.mp3");
    QString title_1("Dracula's Theme");
    QString title_2("Carmilla");
    SoundSample ss[] = {
        createTestSoundSample(resource_url_1, title_1),
        createTestSoundSample(resource_url_2, title_2)
    };

    QString pl_title("Dungeon Crawl");
    Playlist pl = createTestPlaylist(pl_title, ss, 2);

    // Save data
    pl.saveToFile(writer);
    endOutputBufferFile(writer);

    // Check XML when reading
    outbuf.seek(0);
    QXmlStreamReader reader(&outbuf);
    reader.readNext();
    ASSERT_TRUE(reader.isStartDocument());
    reader.readNext();
    Playlist pl_check = Playlist(reader);
    reader.readNext();
    ASSERT_TRUE(reader.isEndDocument());

    // Check data
    SCOPED_TRACE("TestSavingPlaylist");
    assertPlaylistsEqual(&pl, &pl_check);
}

static void DebugContents(QString fname)
{
    // Debug contents
    QFile of(fname);
    of.open(QIODeviceBase::ReadOnly);
    QTextStream in(&of);
    while (!in.atEnd()) {
        qDebug() << in.readLine();
    }
    of.close();

}

void assertShowFilesEqual(const ShowFile *sf1, const ShowFile *sf2)
{
    ASSERT_EQ(sf1->getFilePath(), sf2->getFilePath());
    ASSERT_EQ(sf1->getNumBackgroundPlaylists(), sf2->getNumBackgroundPlaylists());
    for(int i = 0; i < sf1->getNumBackgroundPlaylists(); ++i) {
        SCOPED_TRACE(__func__ + std::to_string(i));
        assertPlaylistsEqual(sf1->getBackgroundPlaylist(i), sf2->getBackgroundPlaylist(i));
    }
}

TEST(TestSavingShowfile, BasicAssertions) {
    // Create structures
    QString tempdir = QString().fromStdString(testing::TempDir());
    QString tempfile(tempdir + "/temp.showfile");
    ShowFile sf = ShowFile();

    // Fill data
    QString resource_url_1("G:/My Drive/Dungeons and Dragons/Castlevania Campaign I Guess/CastlevaniaMusic/DungeonCrawl/1-01 Dracula's Theme.mp3");
    QString resource_url_2("G:/My Drive/Dungeons and Dragons/Castlevania Campaign I Guess/CastlevaniaMusic/DungeonCrawl/2-12 Carmilla.mp3");
    QString title_1("Dracula's Theme");
    QString title_2("Carmilla");

    SoundSample ss1[] = {
        createTestSoundSample(resource_url_1, title_1),
        createTestSoundSample(resource_url_2, title_2)
    };
    SoundSample ss2[] = {
        createTestSoundSample(resource_url_2, title_2),
    };

    QString pl_title1("Dungeon Crawl");
    Playlist pl1 = createTestPlaylist(pl_title1, ss1, 2);
    QString pl_title2("Dungeon Crawl, Encore");
    Playlist pl2 = createTestPlaylist(pl_title2, ss2, 1);

    sf.appendBackgroundPlaylist(pl1);
    sf.appendBackgroundPlaylist(pl2);

    // Save data
#ifdef USING_SAMPLE_SHOWFILE
    sf.saveShowFile(sample_saveloc);
#endif
    sf.saveShowFile(tempfile);

    // Load data, compare
    ShowFile sf2(tempfile);
    SCOPED_TRACE("TestSavingShowfile");
    assertShowFilesEqual(&sf, &sf2);

    // Delete temp file
    ASSERT_TRUE(QFile::remove(tempfile));

}
