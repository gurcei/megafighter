#include "music.h"
#include "util.h"

#define CHORUS_V1 0
#define VERSE_V1  1
#define DITTY_V1  2
#define DITTY2_V1 3

// voice 1
static int v1[] =
{
  // SUBROUTINE CHUNKS
  // -----------------
  DO_CMD_DEFINE_SUB(DITTY_V1),
    -D1_4,

    DO_CMD_AUTO_GATE_OFF(0),
    ND + O5 + D1_32,
    NA + O5 + D1_32,
    ND + O5 + D1_32,
    NA + O5 + D1_32,
    ND + O5 + D1_32,
    NA + O5 + D1_32,
    ND + O5 + D1_32,
    NA + O5 + D1_32,
    DO_CMD_AUTO_GATE_OFF(1),

    ND + O5 + D1_8,
    ND + O5 + D1_4,
    -D1_8,
    ND + O5 + D1_4,
    ND + O5 + D1_8,
    ND + O5 + D1_4,

		// DO_CMD_NEXT_SCREEN,
    NC + O5 + D1_4,
    ND + O5 + D1_8,
  DO_CMD_SUB_RETURN,


  DO_CMD_DEFINE_SUB(CHORUS_V1),
  DO_CMD_SUB_RETURN,

  // SONG BEGINS HERE
  // ----------------
  DO_CMD_GOSUB(DITTY_V1),
  DO_CMD_GOSUB(CHORUS_V1),
  REPEAT_TO_BEGINNING,

  0
};


#define CHORUS_V2 0
#define DITTY_V2  1

static int v2[] =
{
//  DO_CMD_DEFINE_SUB(DITTY_V2),
//    NC + O2 + D1_8,
//    -D1_8,
//    NC + O2 + D1_8,
//    -D1_8,
//    NC + O2 + D1_8,
//    -D1_8,
//    NC + O2 + D1_8,
//    NC + O2 + D1_8,
//    -D1_8,
//
//    NC + O2 + D1_8,
//    -D1_8,
//    NC + O2 + D1_8,
//    -D1_8,
//    NC + O2 + D1_8,
//    -D1_8,
//    NC + O2 + D1_8,
//    NC + O2 + D1_8,
//    -D1_8,
//
//    NF + O2 + D1_8,
//    -D1_8,
//    NF + O2 + D1_8,
//    -D1_8,
//    NG + O2 + D1_8,
//    -D1_8,
//    NG + O2 + D1_8,
//    NG + O2 + D1_8,
//    -D1_8,
//
//    NC + O2 + D1_8,
//    -D1_8,
//    NC + O2 + D1_8,
//    -D1_8,
//    NC + O2 + D1_8,
//    -D1_8,
//    NC + O2 + D1_8,
//    NC + O2 + D1_8,
//    -D1_8,
//
//  DO_CMD_SUB_RETURN,
//
//  DO_CMD_DEFINE_SUB(CHORUS_V2),
//    NF + O2 + D1_8,
//    -D1_8,
//    NF + O2 + D1_8,
//    -D1_8,
//    NG + O2 + D1_8,
//    -D1_8,
//    NG + O2 + D1_8,
//    NG + O2 + D1_8,
//    -D1_8,
//
//    NC + O2 + D1_8,
//    -D1_8,
//    NB + O1 + D1_8,
//    -D1_8,
//    NA + O1 + D1_8,
//    -D1_8,
//    NA + O1 + D1_8,
//    NA + O1 + D1_8,
//    -D1_8,
//
//    NF + O2 + D1_8,
//    -D1_8,
//    NF + O2 + D1_8,
//    -D1_8,
//    NG + O2 + D1_8,
//    -D1_8,
//    NG + O2 + D1_8,
//    NG + O2 + D1_8,
//    -D1_8,
//
//    NC + O2 + D1_8,
//    -D1_8,
//    NC + O2 + D1_8,
//    -D1_8,
//    NC + O2 + D1_8,
//    -D1_8,
//    NC + O2 + D1_8,
//    NC + O2 + D1_8,
//    -D1_8,
//  DO_CMD_SUB_RETURN,
//
//  DO_CMD_GOSUB(DITTY_V2),
//  DO_CMD_GOSUB(DITTY_V2),
//  DO_CMD_GOSUB(CHORUS_V2),
//  DO_CMD_GOSUB(CHORUS_V2),
//
//  REPEAT_TO_BEGINNING,
  0
};

#define DRUM_PAT1 0
#define DRUM_PAT2 1
#define DRUM_PAT3 2
#define DITTY_V3  3

#define DRUM_PAT1x2 \
  DO_CMD_GOSUB(DRUM_PAT1), \
  DO_CMD_GOSUB(DRUM_PAT1),

#define DRUM_PAT1n2 \
  DO_CMD_GOSUB(DRUM_PAT1), \
  DO_CMD_GOSUB(DRUM_PAT2),

#define DRUM_PAT3n2 \
  DO_CMD_GOSUB(DRUM_PAT3), \
  DO_CMD_GOSUB(DRUM_PAT2),

#define DRUM_PAT1x4 \
  DRUM_PAT1x2 \
  DRUM_PAT1n2

// voice 3
static int v3[] =
{
  DO_CMD_DEFINE_SUB(DRUM_PAT1),
    NC + O2 + D1_4,
    NC + O4 + D1_4,

    NC + O2 + D1_8,
    NC + O2 + D1_8,
    NC + O4 + D1_4,

    NC + O2 + D1_4,
    NC + O4 + D1_8,
    NC + O2 + D1_4,
    NC + O2 + D1_8,
    NC + O4 + D1_4,
  DO_CMD_SUB_RETURN,

	DO_CMD_GOSUB(DRUM_PAT1),
  REPEAT_TO_BEGINNING,
  0
};

void prepare_intro_song(void)
{
  prepare_song(v1, v2, v3, 32, 3, 1);
}
