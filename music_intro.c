#include "music.h"
#include "util.h"

#define CHORUS_V1 0
#define VERSE_V1  1
#define DITTY_V1  2
#define DITTY2_V1 3
#define DITTY3_V1 4

#define ARPA_1 \
    NA + O3 + D1_32, \
    NE + O4 + D1_32, \
    NA + O3 + D1_32, \
    NE + O4 + D1_32, \
    NA + O3 + D1_32, \
    NE + O4 + D1_32, \
    NA + O3 + D1_32, \
    NE + O4 + D1_32, \
    NA + O3 + D1_32, \
    NE + O4 + D1_32, \
    NA + O3 + D1_32, \
    NE + O4 + D1_32, \
    NA + O3 + D1_32, \
    NE + O4 + D1_32, \
    NA + O3 + D1_32, \
    NE + O4 + D1_32, \
    NA + O3 + D1_32, \
    NE + O4 + D1_32, \
    NA + O3 + D1_32, \
    NE + O4 + D1_32, \
    NA + O3 + D1_32, \
    NE + O4 + D1_32, \
    NA + O3 + D1_32, \
    NE + O4 + D1_32, \
    NA + O3 + D1_32, \
    NE + O4 + D1_32, \
    NA + O3 + D1_32, \
    NE + O4 + D1_32, \
    NA + O3 + D1_32, \
    NE + O4 + D1_32, \
    NA + O1 + D1_16,

#define ARPAs_1_4 \
    NAs + O3 + D1_32, \
    NF + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NF + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NF + O4 + D1_32, \
    NAs + O1 + D1_16,

#define ARPAs_1_8 \
    NAs + O3 + D1_32, \
    NF + O4 + D1_32, \
    NAs + O1 + D1_16,

#define ARPC_1_8 \
    DO_CMD_AUTO_GATE_OFF(0), \
    NC + O4 + D1_32, \
    NG + O4 + D1_32, \
    DO_CMD_AUTO_GATE_OFF(1), \
    NC + O1 + D1_16, // using this low note to get around gating problems

#define ARPC_1_4 \
    DO_CMD_AUTO_GATE_OFF(0), \
    NC + O4 + D1_32, \
    NG + O4 + D1_32, \
    NC + O4 + D1_32, \
    NG + O4 + D1_32, \
    NC + O4 + D1_32, \
    NG + O4 + D1_32, \
    DO_CMD_AUTO_GATE_OFF(1), \
    NC + O1 + D1_16, // using this low note to get around gating problems

#define ARPD_1 \
    DO_CMD_AUTO_GATE_OFF(0), \
    ND + O4 + D1_32, \
    NA + O4 + D1_32, \
    ND + O4 + D1_32, \
    NA + O4 + D1_32, \
    ND + O4 + D1_32, \
    NA + O4 + D1_32, \
    ND + O4 + D1_32, \
    NA + O4 + D1_32, \
    ND + O4 + D1_32, \
    NA + O4 + D1_32, \
    ND + O4 + D1_32, \
    NA + O4 + D1_32, \
    ND + O4 + D1_32, \
    NA + O4 + D1_32, \
    ND + O4 + D1_32, \
    NA + O4 + D1_32, \
    ND + O4 + D1_32, \
    NA + O4 + D1_32, \
    ND + O4 + D1_32, \
    NA + O4 + D1_32, \
    ND + O4 + D1_32, \
    NA + O4 + D1_32, \
    ND + O4 + D1_32, \
    NA + O4 + D1_32, \
    ND + O4 + D1_32, \
    NA + O4 + D1_32, \
    ND + O4 + D1_32, \
    NA + O4 + D1_32, \
    ND + O4 + D1_32, \
    NA + O4 + D1_32, \
    DO_CMD_AUTO_GATE_OFF(1), \
    ND + O1 + D1_16, // using this low note to get around gating problems

#define ARPD_1_4 \
    DO_CMD_AUTO_GATE_OFF(0), \
    ND + O4 + D1_32, \
    NA + O4 + D1_32, \
    ND + O4 + D1_32, \
    NA + O4 + D1_32, \
    ND + O4 + D1_32, \
    NA + O4 + D1_32, \
    DO_CMD_AUTO_GATE_OFF(1), \
    ND + O1 + D1_16, // using this low note to get around gating problems

#define ARPD_1_8 \
    ND + O4 + D1_32, \
    NA + O4 + D1_32, \
    ND + O1 + D1_16, // bum note

// voice 1
static int v1[] =
{
  // SUBROUTINE CHUNKS
  // -----------------
  DO_CMD_DEFINE_SUB(DITTY_V1),
    -D1_4,

    DO_CMD_SET_ADSR(1, 9, 0, 0),

    ARPD_1_4

    ARPD_1_8

    ARPD_1_4

    -D1_8,

    ARPD_1_4
    ARPD_1_8
    ARPD_1_4

    ARPC_1_4

    ARPD_1_8
  DO_CMD_SUB_RETURN,


  DO_CMD_DEFINE_SUB(DITTY2_V1),
    -D1_4,
    ARPAs_1_4
    ARPC_1_4
    -D1_8,

    DO_CMD_SET_ADSR(1, 9, 7, 0),
    ARPA_1
    -D1_8,
  DO_CMD_SUB_RETURN,

  DO_CMD_DEFINE_SUB(DITTY3_V1),
    -D1_4,

    DO_CMD_SET_ADSR(1, 9, 0, 0),

    ARPD_1_4

    ARPD_1_8

    ARPD_1_4

    -D1_8,

    ARPD_1_4
    ARPD_1_8
    ARPD_1_4

    ARPC_1_4

    ARPAs_1_4

    -D1_8,

    ARPAs_1_4
    ARPAs_1_8
    ARPAs_1_4
    ARPAs_1_8

    ARPC_1_4
    -D1_8,
    ARPC_1_4
    ARPC_1_4
    ARPC_1_8
    DO_CMD_SET_ADSR(1, 9, 7, 0),
    ARPD_1

    ARPD_1_4
    ARPD_1_4
    ARPD_1_4
    ARPC_1_8


    // - - - - - - last rpt - - - -

    ARPAs_1_4

    -D1_8,

    ARPAs_1_4
    ARPAs_1_8
    ARPAs_1_4
    ARPAs_1_8

    ARPC_1_4
    -D1_8,
    ARPC_1_4
    ARPC_1_4
    ARPC_1_8
    DO_CMD_SET_ADSR(1, 9, 7, 0),
    ARPD_1

    ARPD_1_4
    ARPD_1_4
    ARPD_1_4
    ARPD_1_8
    ARPD_1_8

  DO_CMD_SUB_RETURN,

  DO_CMD_DEFINE_SUB(CHORUS_V1),
  DO_CMD_SUB_RETURN,


  // SONG BEGINS HERE
  // ----------------
  DO_CMD_CONTROL(C_SAWTOOTH + C_GATE),
  //DO_CMD_GOSUB(DITTY_V1),
  //DO_CMD_GOSUB(DITTY_V1),
  //DO_CMD_NEXT_SCREEN,
  //DO_CMD_GOSUB(DITTY_V1),
  //DO_CMD_GOSUB(DITTY2_V1),

  // chorus
  DO_CMD_GOSUB(DITTY_V1),
  DO_CMD_GOSUB(DITTY3_V1),

  REPEAT_TO_BEGINNING,

  0
};


#define CHORUS_V2 0
#define DITTY_V2  1

static int v2[] =
{
  -D1_4,
  NFs + O5 + D1_8,
  ND + O5 + D1_8,
  NFs + O5 + D1_8,
  NG + O5 + D1_4,
  NA + O5 + D1_2,
  NG + O5 + D1_16,
  NFs + O5 + D1_16,
  NE + O5 + D1_2,

  -D1_4,
  ND + O5 + D1_8,
  NFs + O5 + D1_8,
  NG + O5 + D1_8,
  NA + O5 + D1_8,
  ND + O6 + D1_8,
  NG + O6 + D1_2,
  NFs + O6 + D1_16,
  NE + O6 + D1_16,
  ND + O6 + D1_4,
  NE + O6 + D1_8,

  NF + O6 + D1_2,
  NE + O6 + D1_16,
  ND + O6 + D1_16,
  NAs + O5 + D1_4D,
  
  NG + O6 + D1_2,
  NF + O6 + D1_16,
  NE + O6 + D1_16,
  NC + O6 + D1_4,
  NE + O6 + D1_8,
  ND + O6 + D1,
  -D1_4,

  NG + O7 + D1_8,
  NFs + O7 + D1_8,
  ND + O7 + D1_8,
  NG + O7 + D1_8,
  NFs + O7 + D1_8,

  NE + O6 + D1_8,

  NF + O6 + D1_2,
  NE + O6 + D1_16,
  ND + O6 + D1_16,
  NAs + O5 + D1_4D,
  
  NG + O6 + D1_2,
  NF + O6 + D1_16,
  NE + O6 + D1_16,
  NC + O6 + D1_4,
  NE + O6 + D1_8,
  NFs + O6 + D1,
  -D1_4,

  NG + O7 + D1_8,
  NFs + O7 + D1_8,
  ND + O7 + D1_8,
  NG + O7 + D1_8,
  NFs + O7 + D1_8,
  ND + O7 + D1_8,
  ND + O7 + D1_8,
//
  REPEAT_TO_BEGINNING,
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

#define BASS_1_4 \
    NC + O2 + D1_32, \
    NB + O1 + D1_32, \
    NAs + O1 + D1_32, \
    NA + O1 + D1_32, \
    NGs + O1 + D1_32, \
    NG + O1 + D1_32, \
    NFs + O1 + D1_16,

#define BASS_1_8 \
    NC + O2 + D1_32, \
    NB + O1 + D1_32, \
    NAs + O1 + D1_16,

#define KICK_1_8 \
    NC + O4 + D1_32, \
    NB + O3 + D1_32, \
    NAs + O3 + D1_16,

// voice 3
static int v3[] =
{
  DO_CMD_DEFINE_SUB(DRUM_PAT1),
    -D1_4,
    NC + O2 + D1_4,
    -D1_4,
    NC + O2 + D1_4,
    -D1_4,
    NC + O2 + D1_4,
    -D1_4,
    NC + O4 + D1_8,
    NC + O2 + D1_8,
  DO_CMD_SUB_RETURN,


  DO_CMD_DEFINE_SUB(DRUM_PAT2),
    -D1_8,
    NC + O2 + D1_8,
    NC + O4 + D1_4,

    NC + O2 + D1_8,
    NC + O2 + D1_8,
    NC + O4 + D1_4,

    NC + O2 + D1_4,
    NC + O4 + D1_8,
    NC + O2 + D1_4,
    NC + O2 + D1_8,
    NC + O4 + D1_8,
    NC + O2 + D1_8,
  DO_CMD_SUB_RETURN,

  DO_CMD_DEFINE_SUB(DRUM_PAT3),
    -D1_8,
    NC + O2 + D1_8,
    NC + O4 + D1_8,
    NC + O2 + D1_8,
    NC + O4 + D1_8,
    NC + O2 + D1_8,
    NC + O2 + D1_8,
    NC + O4 + D1_4,

    NC + O4 + D1_8,
    NG + O3 + D1_8,
    NG + O3 + D1_8,
    NC + O3 + D1_8,
    NC + O3 + D1_8,
    NG + O2 + D1_8,
    NC + O2 + D1_8,
  DO_CMD_SUB_RETURN,

  DO_CMD_SET_ADSR(1, 9, 0, 0),
	//DO_CMD_GOSUB(DRUM_PAT1),

	DO_CMD_GOSUB(DRUM_PAT2),
	DO_CMD_GOSUB(DRUM_PAT2),
  //DO_CMD_GOSUB(DRUM_PAT3),
  REPEAT_TO_BEGINNING,
  0
};

void prepare_intro_song(void)
{
  prepare_song(v1, v2, v3, 32, 3, 0);
}
