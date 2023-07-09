#ifndef DEVELOPMENT_HEADERS_H
#define DEVELOPMENT_HEADERS_H

/**
 * DOC: Purpose of Development Headers
 *
 * These are meant to be one-off defines that affect behavior during development but not production
 * One example: loading a "sample" file on start to avoid the manual testing step of finding a filepath to load
 */

#define USING_SAMPLE_SHOWFILE

#ifdef USING_SAMPLE_SHOWFILE
static char resource_url_1[] = "G:/My Drive/Dungeons and Dragons/Castlevania Campaign I Guess/CastlevaniaMusic/DungeonCrawl/1-01 Dracula's Theme.mp3";
static char resource_url_2[] = "G:/My Drive/Dungeons and Dragons/Castlevania Campaign I Guess/CastlevaniaMusic/DungeonCrawl/2-12 Carmilla.mp3";
static char title_1[] = "Dracula's Theme";
static char title_2[] = "Carmilla";
static char sample_saveloc[] = "G:/My Drive/DungeonSound/sample.showfile";
#endif

#endif // DEVELOPMENT_HEADERS_H
