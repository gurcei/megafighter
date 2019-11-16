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

#define ARPAs_3_2D \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O3 + D1_32, \
    NCs + O4 + D1_32, \
    NAs + O1 + D1_16,

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
    NE + O4 + D1_32, \
    DO_CMD_AUTO_GATE_OFF(1), \
    NC + O1 + D1_16, // using this low note to get around gating problems

#define ARPC_1_4 \
    NC + O4 + D1_32, \
    NE + O4 + D1_32, \
    NC + O4 + D1_32, \
    NE + O4 + D1_32, \
    NC + O4 + D1_32, \
    NE + O4 + D1_32, \
    NC + O1 + D1_16, // using this low note to get around gating problems

#define ARPE_1_8 \
    NE + O4 + D1_32, \
    NG + O4 + D1_32, \
    NE + O1 + D1_16,

#define ARPE_5_8 \
    NE + O4 + D1_32, \
    NG + O4 + D1_32, \
    NE + O4 + D1_32, \
    NG + O4 + D1_32, \
    NE + O4 + D1_32, \
    NG + O4 + D1_32, \
    NE + O4 + D1_32, \
    NG + O4 + D1_32, \
    NE + O4 + D1_32, \
    NG + O4 + D1_32, \
    NE + O4 + D1_32, \
    NG + O4 + D1_32, \
    NE + O4 + D1_32, \
    NG + O4 + D1_32, \
    NE + O4 + D1_32, \
    NG + O4 + D1_32, \
    NE + O4 + D1_32, \
    NG + O4 + D1_32, \
    NE + O1 + D1_16,

#define ARPC_1_2D \
    NC + O4 + D1_32, \
    NE + O4 + D1_32, \
    NC + O4 + D1_32, \
    NE + O4 + D1_32, \
    NC + O4 + D1_32, \
    NE + O4 + D1_32, \
    NC + O4 + D1_32, \
    NE + O4 + D1_32, \
    NC + O4 + D1_32, \
    NE + O4 + D1_32, \
    NC + O4 + D1_32, \
    NE + O4 + D1_32, \
    NC + O4 + D1_32, \
    NE + O4 + D1_32, \
    NC + O4 + D1_32, \
    NE + O4 + D1_32, \
    NC + O4 + D1_32, \
    NE + O4 + D1_32, \
    NC + O4 + D1_32, \
    NE + O4 + D1_32, \
    NC + O4 + D1_32, \
    NE + O4 + D1_32, \
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

#define ARPF_5_4D \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O1 + D1_16,

#define ARPF_1_8 \
    NF + O4 + D1_32, \
    NGs + O4 + D1_32, \
    NF + O1 + D1_16,

#define ARPG_1_8 \
    NG + O4 + D1_32, \
    NAs + O4 + D1_32, \
    NG + O1 + D1_16,

#define ARPDs_1_4 \
    NDs + O4 + D1_32, \
    NG + O4 + D1_32, \
    NDs + O4 + D1_32, \
    NG + O4 + D1_32, \
    NDs + O4 + D1_32, \
    NG + O4 + D1_32, \
    NDs + O1 + D1_16,

#define ARPCs_1_8 \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O1 + D1_16,

#define ARPCs_5_4D \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O4 + D1_32, \
    NF + O4 + D1_32, \
    NCs + O1 + D1_16,

// voice 1
static int v1[] =
{
  0
};


#define CHORUS_V2 0
#define DITTY_V2  1

static int v2[] =
{
  DO_CMD_SET_ADSR(1, 13, 0, 0),
  DO_CMD_AUTO_GATE_OFF(1),

  ARPF_5_4D

  DO_CMD_SET_ADSR(1, 10, 0, 0),
  ARPC_1_8

  ARPF_1_8

  ARPDs_1_4
  ARPDs_1_4

  ARPCs_1_8

  DO_CMD_SET_ADSR(1, 13, 0, 0),
  ARPCs_5_4D

  DO_CMD_SET_ADSR(1, 10, 0, 0),
  ARPCs_1_8

  ARPC_1_8

  DO_CMD_SET_ADSR(1, 13, 0, 0),
  ARPAs_3_2D

  DO_CMD_SET_ADSR(1, 10, 0, 0),
  ARPAs_1_4

  DO_CMD_SET_ADSR(1, 13, 0, 0),
  ARPC_1_2D

  DO_CMD_SET_ADSR(1, 10, 0, 0),
  ARPC_1_4

  DO_CMD_SET_ADSR(1, 13, 0, 0),
  ARPE_5_8

  DO_CMD_SET_ADSR(1, 10, 0, 0),
  ARPE_1_8
  ARPF_1_8
  ARPG_1_8

  DO_CMD_SET_ADSR(1, 13, 0, 0),
  NF + O4 + D1_32,
  NGs + O4 + D1_32,
  NF + O4 + D1_32,
  NGs + O4 + D1_32,

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
	DO_CMD_GOSUB(DRUM_PAT1),

	DO_CMD_GOSUB(DRUM_PAT2),
	DO_CMD_GOSUB(DRUM_PAT2),
	DO_CMD_GOSUB(DRUM_PAT2),
  //DO_CMD_GOSUB(DRUM_PAT3),

	// DO_CMD_GOSUB(DRUM_PAT2),
	// DO_CMD_GOSUB(DRUM_PAT2),
	// DO_CMD_GOSUB(DRUM_PAT2),
	// DO_CMD_GOSUB(DRUM_PAT2),
	// DO_CMD_GOSUB(DRUM_PAT2),
	// DO_CMD_GOSUB(DRUM_PAT2),
  REPEAT_TO_BEGINNING,
  0
};

void prepare_game_song(void)
{
  prepare_song(v1, v2, v3, 32, 3, 0);
}
