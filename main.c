#include <stdio.h>
#include <stdlib.h>
#include <6502.h>
#include "common.h"
#include "util.h"
#include "music.h"

#include "bitmap_ids.h"

//#define SAVEMEM
//#define SHOW_OPTIONS
//#define DEBUG
//#define ANIMBKGND
#define DRAWSPRITES
//#define BLANKSCREEN
//#define HITBOXES

// ================================
// GLOBALS
// ================================
unsigned int screen_loc, rel_loc, gtmpw, gtmpw2, gtmpw3, gtmpw4;
unsigned char a, b, gk, gtmp, num_repairs;
unsigned char hit, hithead, hittorso, hitfeet;
unsigned char* ptr, *ptr2;
unsigned char snd_trigger = 0, snd_idx = 0, snd_delay = 0;
int sky_idx = 0;
int bx, by, cx, cy;
unsigned char energies_buffered = 0;
unsigned char energies[2] = { 0 };
unsigned char last_energies[2] = { 0 };
unsigned char screen_flag = 0;

unsigned char firedown[2]= { 0 };
unsigned char walkingright[2]= { 0 };
unsigned char walkingleft[2]= { 0 };
unsigned char crouching[2]= { 0 };
unsigned char punching[2]= { 0 };
unsigned char floor_idx=12;
unsigned char escdown = 0;

enum OPT_BKGND { BKGND_STATIC, BKGND_ANIM, BKGND_ANIM_REPAIR };
unsigned char option_background = BKGND_STATIC; //BKGND_ANIM;

int building_idx=0;
int fence_idx=0;
int temple_idx=0;

enum { GAME_INTRO, GAME_TITLE, GAME_MAIN, GAME_OPTIONS };

#ifdef DEBUG
void draw_sprintf(unsigned char posx, unsigned char posy, char* str, ...);
#endif
void draw_text(char* str, unsigned char posx, unsigned char posy, unsigned char invert);
void play_sound(unsigned char idx);

enum sound_ids
{
  SND_PUNCH1,
  SND_PUNCH2,
  SND_PUNCH3,
  SND_PUNCH4,
  SND_KICK1,
  SND_KICK2,
  SND_KICK3,
  SND_KICK4,
  SND_KICK5,
  SND_KICK6,
  SND_FALL,
  SND_SWOOSH,
  SND_OOH,
  SND_DIE1,
  SND_DIE2,
  SND_FLIGHT1,
  SND_FLIGHT2,
  SND_FLIGHT3,
  SND_SHOURYUKEN1,
  SND_SHOURYUKEN2,
  SND_TATSUMAKI1,
  SND_TATSUMAKI2,
  SND_HADOUKEN1,
  SND_HADOUKEN2,
  SND_YOU,
  SND_WIN,
  SND_LOSE,
  SND_PERFECT,
  SND_FIGHT,
  SND_ROUND,
  SND_ONE,
  SND_TWO,
  SND_THREE,
  SND_DING1,
  SND_DING2,
  SND_DING3A,
  SND_DING3B,
  SND_DING4,
  SND_DING5
};

unsigned char gamestate = GAME_TITLE;

enum anim_ids
{
  RYU_IDLE,
  RYU_WALK, RYU_WALKB,
  RYU_JUMP, RYU_FJUMP, RYU_BJUMP,
  RYU_CROUCHBLOCK,
  RYU_LPUNCH, RYU_MHPUNCH,
  RYU_FLPUNCH, RYU_FMPUNCH, RYU_FHPUNCH,
  RYU_LMKICK, RYU_HKICK,
  RYU_FLKICK, RYU_FMKICK, RYU_FHKICK,
  RYU_CROUCH_LPUNCH, RYU_CROUCH_MPUNCH, RYU_CROUCH_HPUNCH,
  RYU_CROUCH_LKICK, RYU_CROUCH_MKICK, RYU_CROUCH_HKICK,
  RYU_JUMP_LMHPUNCH,
  RYU_JUMP_LMKICK, RYU_JUMP_HKICK,
  RYU_FJUMP_LPUNCH,
  RYU_FJUMP_MHKICK,
  RYU_SHOURYUKEN, RYU_TATSUMAKI,
  RYU_HADOUKEN,
  RYU_HADPROJ_START, RYU_HADPROJ, RYU_HADPROJ_END,
  RYU_SHOULDERTOSS, RYU_BACKROLL,
  RYU_HIT, RYU_FACEHIT, RYU_CROUCHHIT,
  RYU_KNOCKDOWN, RYU_STUNNED, RYU_KO,
  RYU_VICTORY, RYU_VICTORYALT,
  //RYU_MUGSHOT,
  RYU_MAX
};

#define FIRST_ATTACK   RYU_LPUNCH

unsigned char punch_style = FIRST_ATTACK;

unsigned int anim_ryu_idle[] = { RYU_IDLE2, RYU_IDLE3, RYU_IDLE4 };
unsigned int anim_ryu_walk[] = { RYU_WALK1, RYU_WALK2, RYU_WALK3, RYU_WALK4, RYU_WALK5, RYU_WALK6, RYU_WALK7, RYU_WALK8 };
unsigned int anim_ryu_walkb[] = { RYU_WALK8, RYU_WALK7, RYU_WALK6, RYU_WALK5, RYU_WALK4, RYU_WALK3, RYU_WALK2, RYU_WALK1 };
unsigned int anim_ryu_jump[] = { RYU_JUMP2, RYU_JUMP3, RYU_JUMP4 };
unsigned int anim_ryu_fjump[] = { RYU_FJUMP2, RYU_FJUMP3, RYU_FJUMP4, RYU_FJUMP5, RYU_FJUMP6, RYU_FJUMP7 };
unsigned int anim_ryu_bjump[] = { RYU_FJUMP7, RYU_FJUMP6, RYU_FJUMP5, RYU_FJUMP4, RYU_FJUMP3, RYU_FJUMP2 };
unsigned int anim_ryu_crouchblock[] = { RYU_CROUCH1, RYU_CROUCH2, RYU_BLOCK1, RYU_BLOCK2 };
unsigned int anim_ryu_lpunch[] = { RYU_LPUNCH1, RYU_LPUNCH2, RYU_LPUNCH1 };
unsigned int anim_ryu_mhpunch[] = { RYU_MHPUNCH1, RYU_MHPUNCH2, RYU_MHPUNCH3 };
unsigned int anim_ryu_flpunch[] = { RYU_FLPUNCH1, RYU_FLPUNCH2, RYU_FLPUNCH3 };
unsigned int anim_ryu_fmpunch[] = { RYU_FMPUNCH1, RYU_FMPUNCH2, RYU_FMPUNCH3, RYU_FMPUNCH4 };
unsigned int anim_ryu_fhpunch[] = { RYU_FHPUNCH1, RYU_FHPUNCH2, RYU_FHPUNCH3, RYU_FHPUNCH4 };
unsigned int anim_ryu_lmkick[] = { RYU_LMKICK1, RYU_LMKICK2 };
unsigned int anim_ryu_hkick[] = { RYU_HKICK1, RYU_HKICK2, RYU_HKICK3, RYU_HKICK4, RYU_HKICK5 };
unsigned int anim_ryu_flkick[] = { RYU_FLKICK1, RYU_FLKICK2 };
unsigned int anim_ryu_fmkick[] = { RYU_FMKICK1, RYU_FMKICK2, RYU_FMKICK3 };
unsigned int anim_ryu_fhkick[] = { RYU_FHKICK1, RYU_FHKICK2, RYU_FHKICK3 };
unsigned int anim_ryu_crouch_lpunch[] = { RYU_CROUCH_LPUNCH1, RYU_CROUCH_LPUNCH2 };
unsigned int anim_ryu_crouch_mpunch[] = { RYU_CROUCH_MPUNCH1, RYU_CROUCH_MPUNCH2 };
unsigned int anim_ryu_crouch_hpunch[] = { RYU_CROUCH_HPUNCH1, RYU_CROUCH_HPUNCH2, RYU_CROUCH_HPUNCH3 };
unsigned int anim_ryu_crouch_lkick[] = { RYU_CROUCH_LKICK1, RYU_CROUCH_LKICK2 };
unsigned int anim_ryu_crouch_mkick[] = { RYU_CROUCH_MKICK1, RYU_CROUCH_MKICK2 };
unsigned int anim_ryu_crouch_hkick[] = { RYU_CROUCH_HKICK1, RYU_CROUCH_HKICK2, RYU_CROUCH_HKICK3, RYU_CROUCH_HKICK4, RYU_CROUCH_HKICK5 };
unsigned int anim_ryu_jump_lmhpunch[] = { RYU_JUMP_LMHPUNCH1, RYU_JUMP_LMHPUNCH2, RYU_JUMP_LMHPUNCH3 };
unsigned int anim_ryu_jump_lmkick[] = { RYU_JUMP_LMKICK1, RYU_JUMP_LMKICK2 };
unsigned int anim_ryu_jump_hkick[] = { RYU_JUMP_HKICK1, RYU_JUMP_HKICK2, RYU_JUMP_HKICK3 };
unsigned int anim_ryu_fjump_lpunch[] = { RYU_FJUMP_LPUNCH1, RYU_FJUMP_LPUNCH2 };
unsigned int anim_ryu_fjump_mhkick[] = { RYU_FJUMP_MHKICK1, RYU_FJUMP_MHKICK2, RYU_FJUMP_MHKICK3 };
unsigned int anim_ryu_shouryuken[] = { RYU_SHOURYUKEN1, RYU_SHOURYUKEN2, RYU_SHOURYUKEN3, RYU_SHOURYUKEN4, RYU_SHOURYUKEN5, RYU_SHOURYUKEN6, RYU_SHOURYUKEN7 };
unsigned int anim_ryu_tatsumaki[] = { RYU_TATSUMAKI1, RYU_TATSUMAKI2, RYU_TATSUMAKI3, RYU_TATSUMAKI4, RYU_TATSUMAKI5, RYU_TATSUMAKI6, RYU_TATSUMAKI7, RYU_TATSUMAKI8, RYU_TATSUMAKI9 };
unsigned int anim_ryu_hadouken[] = { RYU_HADOUKEN1, RYU_HADOUKEN2, RYU_HADOUKEN3, RYU_HADOUKEN4, RYU_HADOUKEN5 };
unsigned int anim_ryu_hadproj_start[] = { RYU_HADPROJ1, RYU_HADPROJ2 };
unsigned int anim_ryu_hadproj[] = { RYU_HADPROJ3, RYU_HADPROJ4, RYU_HADPROJ5, RYU_HADPROJ6, RYU_HADPROJ7, RYU_HADPROJ8, RYU_HADPROJ9, RYU_HADPROJ10, RYU_HADPROJ11, RYU_HADPROJ12, RYU_HADPROJ13, RYU_HADPROJ14 };
unsigned int anim_ryu_hadproj_end[] = { RYU_HADPROJ15, RYU_HADPROJ16, RYU_HADPROJ17, RYU_HADPROJ18 };
unsigned int anim_ryu_shouldertoss[] = { RYU_SHOULDERTOSS1, RYU_SHOULDERTOSS2, RYU_SHOULDERTOSS3, RYU_SHOULDERTOSS4, RYU_SHOULDERTOSS5 };
unsigned int anim_ryu_backroll[] = { RYU_BACKROLL1, RYU_BACKROLL2, RYU_BACKROLL3, RYU_BACKROLL4, RYU_BACKROLL5, RYU_BACKROLL6, RYU_BACKROLL7 };
unsigned int anim_ryu_hit[] = { RYU_HIT1, RYU_HIT2, RYU_HIT3, RYU_HIT4 };
unsigned int anim_ryu_facehit[] = { RYU_FACEHIT1, RYU_FACEHIT2, RYU_FACEHIT3, RYU_FACEHIT4 };
unsigned int anim_ryu_crouchhit[] = { RYU_CROUCHHIT1, RYU_CROUCHHIT2, RYU_CROUCHHIT3 };
unsigned int anim_ryu_knockdown[] = { RYU_KNOCKDOWN1, RYU_KNOCKDOWN2, RYU_KNOCKDOWN3, RYU_KNOCKDOWN4, RYU_KNOCKDOWN5, RYU_KNOCKDOWN6, RYU_KNOCKDOWN7, RYU_KNOCKDOWN8 };
unsigned int anim_ryu_stunned[] = { RYU_STUNNED1, RYU_STUNNED2, RYU_STUNNED3 };
unsigned int anim_ryu_ko[] = { RYU_KO1, RYU_KO2, RYU_KO3, RYU_KO4, RYU_KO5 };
unsigned int anim_ryu_victory[] = { RYU_VICTORY1, RYU_VICTORY2, RYU_VICTORY3 };
unsigned int anim_ryu_victoryalt[] = { RYU_VICTORYALT1, RYU_VICTORYALT2, RYU_VICTORYALT3, RYU_VICTORYALT4, RYU_VICTORYALT5, RYU_VICTORYALT6, RYU_VICTORYALT7 };
//unsigned int anim_ryu_mugshot[] = { RYU_MUGSHOT1, RYU_MUGSHOT2, RYU_MUGSHOT3 };

typedef struct
{
  unsigned int v1;
  unsigned int v2;
  unsigned int v3;
} voice_offset;

enum { SONG_INTRO, SONG_GAME };
voice_offset lstVoiceOffsets[2] =
{
  { 0xe000, 0xe000 + 0x0370, 0xe000 + 0x03f2 }, // INTRO SONG
  { 0xe000, 0xe000 + 0x0146, 0xe000 + 0x0796 }  // GAME SONG
};

typedef struct
{
  unsigned char anim_length;
  char *relx;
  char *rely;
  unsigned char *frame;
} anim_movement;

char ryu_jump_relx[]  = {  0,  0,  0,  0,  0,  0, 0, 0, 0, 0 };
char ryu_jump_rely[]  = { -3, -3, -2, -1, -1,  1, 1, 2, 3, 3 };
char ryu_jump_frame[] = {  0,  0,  1,  1,  2,  2, 1, 1, 0, 0 };

anim_movement ryu_anim_jump =
{
  10,
  ryu_jump_relx,
  ryu_jump_rely,
  ryu_jump_frame
};

char ryu_fjump_relx[]  = {  1,  1,  1,  1,  1,  1, 1, 1, 1, 1 };
char ryu_fjump_rely[]  = { -3, -3, -2, -1, -1,  1, 1, 2, 3, 3 };
char ryu_fjump_frame[] = {  0,  1,  1,  2,  2,  3, 3, 4, 5, 6 };

anim_movement ryu_anim_fjump =
{
  10,
  ryu_fjump_relx,
  ryu_fjump_rely,
  ryu_fjump_frame
};

char ryu_bjump_relx[]  = {  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1 };

anim_movement ryu_anim_bjump =
{
  10,
  ryu_bjump_relx,
  ryu_fjump_rely,
  ryu_fjump_frame
};

char ryu_tatsu_relx[] =  { 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0 };
char ryu_tatsu_rely[] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
char ryu_tatsu_frame[] = { 0, 1, 2, 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5, 6, 7, 8 };

anim_movement ryu_anim_tatsu =
{
  17,
  ryu_tatsu_relx,
  ryu_tatsu_rely,
  ryu_tatsu_frame
};

char ryu_shouryuken_relx[]  = {  0,  0,  1,  1,  1,  1,  0,  0,  0, 0, 0, 0, 0 };
char ryu_shouryuken_rely[]  = {  0,  0,  0,  0, -1, -2, -1, -1,  1, 1, 2, 1, 0 };
char ryu_shouryuken_frame[] = {  0,  1,  2,  3,  3,  4,  4,  5,  5, 6, 6, 6, 7 };

anim_movement ryu_anim_shouryuken =
{
  13,
  ryu_shouryuken_relx,
  ryu_shouryuken_rely,
  ryu_shouryuken_frame
};

typedef struct
{
  //unsigned long reu_loc;
  //unsigned int frame_size;  // in bytes
  unsigned int* frames;
  unsigned char frame_count;
  unsigned char pingpong;
  unsigned int  cols;
  unsigned char rows;
} anim_detail;

anim_detail anims[RYU_MAX] =
{
  // let the first two fields be auto-generated by the details in the other fields
  // (reu_loc and frame_size)
  { anim_ryu_idle,  3, 1, 6,  13 }, // RYU_IDLE
  { anim_ryu_walk,  8, 0, 9,  13 }, // RYU_WALK
  { anim_ryu_walkb,  8, 0, 9,  13 }, // RYU_WALKB
  { anim_ryu_jump,  3, 1, 6,  13 }, // RYU_JUMP
  { anim_ryu_fjump,  7, 0, 10, 13 }, // RYU_FJUMP
  { anim_ryu_bjump,  7, 0, 10, 13 }, // RYU_BJUMP
  { anim_ryu_crouchblock,  4, 0, 6, 13 },  // RYU_CROUCHBLOCK
  { anim_ryu_lpunch,  3, 0, 8, 14 },  // RYU_LPUNCH
  { anim_ryu_mhpunch,  3, 1, 10, 14 }, // RYU_MHPUNCH
  { anim_ryu_flpunch,  3, 0, 7, 14 },  // RYU_FLPUNCH
  { anim_ryu_fmpunch,  4, 1, 8, 14 },  // RYU_FMPUNCH
  { anim_ryu_fhpunch,  4, 1, 8, 14 },  // RYU_FHPUNCH
  { anim_ryu_lmkick,  2, 1, 9, 16 },  // RYU_LMKICK
  { anim_ryu_hkick,  5, 0, 10, 16 }, // RYU_HKICK
  { anim_ryu_flkick,  2, 1, 10, 16 }, // RYU_FLKICK
  { anim_ryu_fmkick,  3, 1, 8, 16 },  // RYU_FMKICK
  { anim_ryu_fhkick,  3, 1, 11, 16 }, // RYU_FHKICK
  { anim_ryu_crouch_lpunch,  2, 1, 9, 16 },  // RYU_CROUCH_LPUNCH
  { anim_ryu_crouch_mpunch,  2, 1, 9, 16 },  // RYU_CROUCH_MPUNCH
  { anim_ryu_crouch_hpunch,  3, 1, 7, 16 },  // RYU_CROUCH_HPUNCH
  { anim_ryu_crouch_lkick,  2, 1, 10, 16 }, // RYU_CROUCH_LKICK
  { anim_ryu_crouch_mkick,  2, 1, 12, 16 }, // RYU_CROUCH_MKICK
  { anim_ryu_crouch_hkick,  5, 0, 10, 16 }, // RYU_CROUCH_HKICK
  { anim_ryu_jump_lmhpunch,  3, 0, 7,  18 }, // RYU_JUMP_LMHPUNCH
  { anim_ryu_jump_lmkick,  2, 0, 7,  18 }, // RYU_JUMP_LMKICK
  { anim_ryu_jump_hkick,  3, 0, 8,  18 }, // RYU_JUMP_HKICK
  { anim_ryu_fjump_lpunch,  2, 0, 6,  12 }, // RYU_FJUMP_LPUNCH
  { anim_ryu_fjump_mhkick,  3, 0, 9,  12 }, // RYU_FJUMP_MHKICK
  { anim_ryu_shouryuken,  7, 0, 7,  18 }, // RYU_SHOURYUKEN
  { anim_ryu_tatsumaki,  9, 0, 12, 18 }, // RYU_TATSUMAKI
  { anim_ryu_hadouken,  5, 0, 12, 12 }, // RYU_HADOUKEN
  { anim_ryu_hadproj_start,  2, 0, 6,  8  }, // RYU_HADPROJ_START
  { anim_ryu_hadproj,  12, 0, 11, 8  }, // RYU_HADPROJ
  { anim_ryu_hadproj_end,  4, 0, 6,  8  }, // RYU_HADPROJ_END
  { anim_ryu_shouldertoss,  5, 0, 9,  13 }, // RYU_SHOULDERTOSS
  { anim_ryu_backroll,  7, 0, 10, 12 }, // RYU_BACKROLL
  { anim_ryu_hit,  4, 0, 7,  15 }, // RYU_HIT
  { anim_ryu_facehit,  4, 0, 8,  15 }, // RYU_FACEHIT
  { anim_ryu_crouchhit,  3, 0, 9,  15 }, // RYU_CROUCHHIT
  { anim_ryu_knockdown,  8, 0, 10, 15 }, // RYU_KNOCKDOWN
  { anim_ryu_stunned,  3, 1, 8,  15 }, // RYU_STUNNED
  { anim_ryu_ko,  5, 0, 10, 15 }, // RYU_KO
  { anim_ryu_victory,  3, 0, 7,  16 }, // RYU_VICTORY
  { anim_ryu_victoryalt,  7, 1, 7,  16 }, // RYU_VICTORYALT
  //{ anim_ryu_mugshot,  3, 0, 11, 15 }, // RYU_MUGSHOT
};

typedef struct
{
	unsigned char dir;	// if dir == 1 then use the mirrored version of the sprite
  unsigned char posx, posy;
  unsigned char anim;
  unsigned char anim_idx;  // the index within an animation
  unsigned char anim_tmr;  // how many ticks within the animation (i.e., you will sit on some frame-indexes for multiple ticks)
  unsigned char anim_dir;
  unsigned char visible;
  anim_movement *anim_movement;
	unsigned char jumping;
  unsigned int boxes[4][4];
} sprite_detail;

#define SPR_MAX 4

sprite_detail* cur_spr;
sprite_detail sprites[SPR_MAX] =
{
  { 0, 4,  23,  RYU_IDLE, 0, 0, 1, 1, NULL, 0 }, // player 1 (port 2)
  { 1, 30, 23,  RYU_IDLE, 0, 0, 1, 1, NULL, 0 }, // player 2 (port 1)
  { 0, 10, 15,  RYU_HADPROJ_START, 0, 0, 1, 0, NULL, 0 },
  { 0, 10, 15,  RYU_HADPROJ_START, 0, 0, 1, 0, NULL, 0 },

/*  { 7,  0,  RYU_WALK, 0, 1 },
  { 14, 0,  RYU_JUMP, 0, 1 },
  { 21, 0,  RYU_FJUMP, 0, 1 },
  { 31, 0,  RYU_CROUCHBLOCK, 0, 1 }, */
};

typedef struct
{
  unsigned int reloffset;    // this is the byte-offset starting from the top-right corner of the object, but will increment per row_segment
  unsigned char length;    // the number of length in chars for this row segment
} reu_row_segment;

typedef struct
{
  unsigned char num_segments;
  unsigned int start_segment_idx;   // NOTE: reu_loc for segment metadata = start_segment_idx * sizeof(reu_row_segment)
              // NOTE: this might have to be a relative value, and I make it absolute at run-time
  unsigned char num_repairs;  // how many repair chars are needed for this bitmap
  unsigned long reu_ptr;  // pointer to reu-memory for the data for the first row-segment (for the next segment, it'll be adjecent to this one in reu memory)
												// I might have to leave this last field empty (relocatable?) and fill it in at run-time, depending on the ordering of all the objects
} reu_segged_bmp_obj;

typedef struct
{
  unsigned int reloffset;    // this is the byte-offset starting from the top-left corner of the object, but will increment per row_segment
  unsigned char vals[16];     // 0=bmpval0, 1=maskval0, 2=bmpval1, 3=maskval1, etc...
} reu_repair_obj;

reu_segged_bmp_obj* segbmps = (reu_segged_bmp_obj*)0x1000;
reu_repair_obj *repair;


// ================================

// RAM Expansion Controller (REC) Registers
#define REC_STATUS        0xDF00
#define REC_COMMAND       0xDF01
#define REC_C64_ADDR_LO   0xDF02
#define REC_C64_ADDR_HI   0xDF03
#define REC_REU_ADDR_LO   0xDF04
#define REC_REU_ADDR_HI   0xDF05
#define REC_REU_ADDR_BANK 0xDF06
#define REC_TXFR_LEN_LO   0xDF07
#define REC_TXFR_LEN_HI   0xDF08
#define REC_INTRPT_MASK   0xDF09
#define REC_ADDR_CTRL     0xDF0A

int c64loc;
unsigned long reuloc;
unsigned char reudir=0; // 0=reu-to-c64, 1=c64-to-reu
int length;

void reu_simple_copy(void)
{
  // c64base = 8192 = 0x2000
  Poke(REC_C64_ADDR_LO, c64loc & 0xff);
  Poke(REC_C64_ADDR_HI, c64loc >> 8);
  Poke(REC_REU_ADDR_LO, reuloc & 0xff);
  Poke(REC_REU_ADDR_HI, (reuloc >> 8) & 0xff);
  Poke(REC_REU_ADDR_BANK, (reuloc >> 16) & 0xff);
  Poke(REC_TXFR_LEN_LO, length & 0xff);
  Poke(REC_TXFR_LEN_HI, length >> 8);
  // c64 to REU with immediate execution
  if (reudir)
  {
    Poke(REC_COMMAND, 0x90); // %10010001
  }
  else
  {
    // REU to c64 with immediate execution
    Poke(REC_COMMAND, 0x91); // %10010001
  }
}

/*
void reu_copy(int c64loc, unsigned long reuloc, int rowsize, unsigned char rows)
{
  unsigned char y;
  for (y = 0; y < rows; y++)
  {
    // c64base = 8192 = 0x2000
    Poke(REC_C64_ADDR_LO, c64loc & 0xff);
    Poke(REC_C64_ADDR_HI, c64loc >> 8);
    Poke(REC_REU_ADDR_LO, reuloc & 0xff);
    Poke(REC_REU_ADDR_HI, (reuloc >> 8) & 0xff);
    Poke(REC_REU_ADDR_BANK, (reuloc >> 16) & 0xff);
    Poke(REC_TXFR_LEN_LO, rowsize & 0xff);
    Poke(REC_TXFR_LEN_HI, rowsize >> 8);
    // REU to c64 with immediate execution
    Poke(REC_COMMAND, 0x91); // %10010001
    c64loc += 40*8;
    reuloc += rowsize;
  }
}
*/
 
int vicbase = 0x0000;
unsigned int draw_page = 1;

void clear_petscii(void)
{
  vicbase = 0xd800;

  while (vicbase < 0xd800+1000)
  {
    Poke(vicbase, 0);
    vicbase++;
  }
}

void clear_screen(void)
{
  unsigned int i;
  // Clear bitmap
	rel_loc = vicbase+0x2000+8000;
  for (screen_loc = vicbase+0x2000; screen_loc < rel_loc; screen_loc++)
  {
    Poke(screen_loc, 0x00);
  }

  // Set bitmap colours
  for (i = 1024; i < 2024; i++)
  {
    Poke(i, (0 << 4) + 1);  // black and white
  }
  //
  // Set bitmap colours
  for (i = 0x4000+1024; i < 0x4000+2024; i++)
  {
    Poke(i, (0 << 4) + 1);  // black and white
  }
}

void animate_sprite(sprite_detail* spr)
{
  if (spr->anim_movement)
  {
    gk = spr->anim_tmr;
    if (spr->dir)
      spr->posx -= spr->anim_movement->relx[gk];
    else
      spr->posx += spr->anim_movement->relx[gk];
    spr->posy += spr->anim_movement->rely[gk];
    spr->anim_idx = spr->anim_movement->frame[gk];
    spr->anim_tmr++;

    // test for end of jump (probably not the nicest place to put this, but it'll do)
    if (spr->anim_tmr == spr->anim_movement->anim_length)
    {
      spr->jumping=0;
      spr->anim = RYU_IDLE;
      spr->anim_idx = 0;
      spr->anim_dir = 1;
      spr->anim_movement = NULL;
    }

    return;
  }

  if (spr->anim_dir == 1)
  {
    if (spr->anim_idx == anims[spr->anim].frame_count-1)
    {
      if (anims[spr->anim].pingpong)
      {
        spr->anim_dir = 0;
        spr->anim_idx--;
      }
      else
        spr->anim_idx=0;
    }
    else
      spr->anim_idx++;
  }
  else if (spr->anim_dir == 0)
  {
    if (spr->anim_idx == 0)
    {
      spr->anim_dir = 1;
      spr->anim_idx++;
    }
    else
      spr->anim_idx--;
  }
}

unsigned keycode;
unsigned char col;

void check_real_keyboard(void)
{
	keycode = 0;
	__asm__ ( "jsr " FUNC_MINIKEY );
	__asm__ ( "bcc rkskip" );
	__asm__ ( "sta %v", keycode );
  __asm__ ( "rkskip:" );
	__asm__ ( "lda #$0" );	// set DDR for CIA-portA back to input so we can read joyport2 again
  __asm__ ( "sta $dc02" );
}

unsigned char pi;
unsigned char key;

void get_keyboard_input(void)
{
  check_real_keyboard();

#ifdef SHOW_OPTIONS
	// check for escape key
	if (keycode == 0x77)
	{
		if (!escdown)
		{
			escdown = 1;
			gamestate = GAME_OPTIONS;
		}
	}
	else
	{
		escdown = 0;
	}
#endif

  //draw_sprintf(0, 0, "key=0x%02X", keycode);

/*
  if (keycode == 0x21) bx--; // A
  if (keycode == 0x22) bx++; // D
  if (keycode == 0x11) by--; // W
  if (keycode == 0x07) by++; // S

  if (keycode == 0x24) cx--; // J
  if (keycode == 0x25) cx++; // L
  if (keycode == 0x14) cy--; // I
  if (keycode == 0x54) cy++; // K
*/

	for (pi = 0; pi < 2; pi++)
	{
		// JOYSTICK: left=4, right=8, up=1, down=2, fire=16
		key  = (~Peek(56320U + pi)) & 31; //cgetc();

		if (!(key & 16) && firedown[pi])  // test if fire was released
		{
			firedown[pi]=0;
	//    sprites[0].anim = RYU_IDLE;
	//    sprites[0].anim_idx = 0;
	//    sprites[0].anim_dir = 1;
		}

		// did we release down key?
		if (!(key & 2) && crouching[pi])
		{
			crouching[pi] = 0;
			sprites[pi].anim = RYU_IDLE;
			sprites[pi].anim_idx = 0;
			sprites[pi].anim_dir = 1;
		}

		if (key != 0)
		{
      if (punching[pi])
      {
        // ignore keyboard input while punching flag is on
      }
			else if (key & 16 && !firedown[pi]) // fire button
			{
				firedown[pi]=1;
				punching[pi]=1;

        // check direction of joystick to decide on which punch/kick to use
        if (key & 1) // up
        {
          if (sprites[pi].dir ? (key &  4) : (key & 8)) // right
          {
            snd_trigger = 2;
            snd_delay = 1;
            snd_idx = SND_SHOURYUKEN1;

            sprites[pi].anim = RYU_SHOURYUKEN;
            sprites[pi].anim_movement = &ryu_anim_shouryuken;
            sprites[pi].anim_tmr = 0;
          }
          else if (sprites[pi].dir ? (key & 8) : (key & 4)) // left
          {
            snd_trigger = 2;
            snd_delay = 1;
            snd_idx = SND_TATSUMAKI1;

            sprites[pi].anim = RYU_TATSUMAKI;
            //sprites[pi].anim = sprites[pi].dir ? RYU_FJUMP : RYU_BJUMP;
            sprites[pi].anim_idx = 0;
            sprites[pi].anim_dir = 1;
            sprites[pi].anim_movement = &ryu_anim_tatsu;
            sprites[pi].anim_tmr = 0;
          }
          else // purely up
          {
            sprites[pi].anim = RYU_HKICK;
          }
          key &= ~(1+4+8);  // remove up flag
        }
        else if (key & 2) // down
        {
          if (key &  8) // right
            sprites[pi].anim = sprites[pi].dir ? RYU_HADOUKEN : RYU_CROUCH_HKICK;
          else if (key & 4) // left
            sprites[pi].anim = sprites[pi].dir ? RYU_CROUCH_HKICK : RYU_HADOUKEN;
          else // purely down
            sprites[pi].anim = RYU_CROUCH_HPUNCH;

          key &= ~(4+8);
        }
        else if (key & 8) // right only
        {
          sprites[pi].anim = sprites[pi].dir ? RYU_LMKICK : RYU_MHPUNCH;
          key &= ~8;
        }
        else if (key & 4) // left only
        {
          sprites[pi].anim = sprites[pi].dir ? RYU_MHPUNCH : RYU_LMKICK;
          key &= ~4;
        }
        else
        {
          sprites[pi].anim = punch_style;
          punch_style++;
        }

        switch(sprites[pi].anim)
        {
          case RYU_HKICK:
          case RYU_MHPUNCH:
          case RYU_CROUCH_HPUNCH:
            snd_trigger = 1;
            snd_delay = 2;
            snd_idx = SND_FALL;
            break;
          case RYU_LMKICK:
            snd_trigger = 1;
            snd_delay = 1;
            snd_idx = SND_PUNCH2;
            break;
        }

				sprites[pi].anim_idx = 0;
				sprites[pi].anim_dir = 1;
				if (punch_style == RYU_MAX)
					punch_style = RYU_LPUNCH;
			}
			else if (key & 1 && !sprites[pi].jumping && sprites[pi].anim == RYU_IDLE) // up
			{
				sprites[pi].jumping = 1;
				walkingright[pi] = 0; // TODO: consider how far we've walked in deciding where to jump
				walkingleft[pi] = 0;
				if (key & 8) // are we walking right too?
				{
					sprites[pi].anim = sprites[pi].dir ? RYU_BJUMP : RYU_FJUMP;
					sprites[pi].anim_idx = 0;
					sprites[pi].anim_dir = 1;
					sprites[pi].anim_movement = sprites[pi].dir ? &ryu_anim_bjump : &ryu_anim_fjump;
					sprites[pi].anim_tmr = 0;
				}
				else if (key & 4) // left jump?
				{
					sprites[pi].anim = sprites[pi].dir ? RYU_FJUMP : RYU_BJUMP;
					sprites[pi].anim_idx = 0;
					sprites[pi].anim_dir = 1;
					sprites[pi].anim_movement = sprites[pi].dir ? &ryu_anim_fjump : &ryu_anim_bjump;
					sprites[pi].anim_tmr = 0;
				}
				else
				{
					sprites[pi].anim = RYU_JUMP;
					sprites[pi].anim_idx = 0;
					sprites[pi].anim_dir = 1;
					sprites[pi].anim_movement = &ryu_anim_jump;
					sprites[pi].anim_tmr = 0;
				}
				//vely=-6 << 5;
			}
      // DOWN KEY
			else if (key & 2 && !sprites[pi].jumping && !walkingleft[pi] && !walkingright[pi])
			{
				crouching[pi] = 1;
				sprites[pi].anim = RYU_CROUCHBLOCK;
				sprites[pi].anim_idx = 1;
				sprites[pi].anim_dir = 2;
			}
      // LEFT KEY
			else if (key & 4 && !walkingleft[pi] && !walkingright[pi] && !sprites[pi].jumping) // left
			{
				walkingleft[pi] = 1;
        if (sprites[pi].dir == 0)
          sprites[pi].posx -= 3;
				sprites[pi].anim = sprites[pi].dir ? RYU_WALK : RYU_WALKB;
				sprites[pi].anim_idx = 0;
				sprites[pi].anim_dir = 1;
				//dir = 1;
				//girlx -= 2;
				//if (!(i % 6)) // modulo operation seems to be time consuming !(i % 6))
				//  frame = (frame+1) % 2; // & 0x01; // % 2;
				//if (girlx < 25) girlx = 25;
			}
      // RIGHT KEY
			else if (key & 8 && !walkingleft[pi] && !walkingright[pi] && !sprites[pi].jumping) // right
			{
				walkingright[pi] = 1;
        if (sprites[pi].dir == 1)
          sprites[pi].posx += 3;
				sprites[pi].anim = sprites[pi].dir ? RYU_WALKB : RYU_WALK;
				sprites[pi].anim_idx = 0;
				sprites[pi].anim_dir = 1;
				//dir = 0;
				//girlx += 2;

				//if (!(i & 0x03)) // don't use modulo operation !(i % 6))
				//  frame = (frame+1) % 2; //& 0x01; // % 2;
				//if (girlx > 323) girlx = 323;
			}
			// floor animate
			if (key & 4) // floor-left animate
			{
				building_idx--;
				fence_idx--;
				temple_idx--;

				if (floor_idx < 24)
					floor_idx++;
			}
			if (key & 8) // floor-right animate
			{
				building_idx++;
				fence_idx++;
				temple_idx++;

				if (floor_idx > 0)
					floor_idx--;
			}
		} // end if
    // - - - - no key is pressed? still some checks to do.. - - - -
    else
    {
      // have we stopped walking left
      if (key != 4 && walkingleft[pi])
      {
        if (sprites[pi].dir == 0)
          sprites[pi].posx += 3 - sprites[pi].anim_idx / 3;
        else
          sprites[pi].posx -= sprites[pi].anim_idx / 3;
        sprites[pi].anim = RYU_IDLE;
        sprites[pi].anim_idx = 0;
        walkingleft[pi] = 0;
      }
      else if (key != 8 && walkingright[pi])
      {
        if (sprites[pi].dir == 1)
          sprites[pi].posx -= (3 - sprites[pi].anim_idx / 3);
        else
          sprites[pi].posx += (sprites[pi].anim_idx / 3);
        sprites[pi].anim = RYU_IDLE;
        sprites[pi].anim_idx = 0;
        walkingright[pi] = 0;
      }
    }
	} // end for

  //Poke(8192L, walkingright);
  //Poke(8193L, walkingleft);
  //Poke(8194L, jumping);
}

// returns 1 = skip next animation step
// returns 0 = perform next animation step
unsigned char post_draw_processing(unsigned char sprite)
{
  if (sprite < 2) // player 1 or 2?
  {
    // extra movement-related processing
    if (walkingright[sprite] && sprites[sprite].anim_idx==7)
    {
      walkingright[sprite]=0;
      if (sprites[sprite].dir == 0)
        sprites[sprite].posx += 3;
      sprites[sprite].anim = RYU_IDLE;
      sprites[sprite].anim_idx = 0;
      sprites[sprite].anim_dir = 1;
    }

    if (walkingleft[sprite] && sprites[sprite].anim_idx==7)
    {
      walkingleft[sprite]=0;
      if (sprites[sprite].dir == 1)
        sprites[sprite].posx -= 3;
      sprites[sprite].anim = RYU_IDLE;
      sprites[sprite].anim_idx = 0;
      sprites[sprite].anim_dir = 1;
    }

    // start hadouken projectile?
    if (sprites[sprite].anim == RYU_HADOUKEN)
    {
      if (sprites[sprite].anim_idx == 2)
      {
        snd_trigger = 2;
        snd_idx = SND_HADOUKEN1;
      }
      else if (sprites[sprite].anim_idx == 4)
      {
        sprites[sprite+2].anim=RYU_HADPROJ_START;
        sprites[sprite+2].anim_idx=0;
        sprites[sprite+2].visible=1;
        sprites[sprite+2].dir = sprites[sprite].dir;
        if (sprites[sprite].dir)
        {
          sprites[sprite+2].posx = sprites[sprite].posx - 8;
          sprites[sprite+2].posy = sprites[sprite].posy - 3;
        }
        else
        {
          sprites[sprite+2].posx = sprites[sprite].posx + 9;
          sprites[sprite+2].posy = sprites[sprite].posy - 3;
        }
      }
    }

    if (punching[sprite] && !sprites[sprite].anim_movement)
    {
      if (anims[sprites[sprite].anim].pingpong)
      {
        if (sprites[sprite].anim_idx == 0 && sprites[sprite].anim_dir==0)
        {
          punching[sprite] = 0;
          sprites[sprite].anim = RYU_IDLE;
          sprites[sprite].anim_idx = 0;
          sprites[sprite].anim_dir = 1;
          return 1;
        }
      }
      else if (sprites[sprite].anim_idx == anims[sprites[sprite].anim].frame_count-1)
      {
        punching[sprite] = 0;
        sprites[sprite].anim = RYU_IDLE;
        sprites[sprite].anim_idx = 0;
        sprites[sprite].anim_dir = 1;
        return 1;
      }
    }

  } // end sprite0 processing

  // hadouken projectile sprite?
  if (sprite == 2 || sprite == 3)
  {
    //draw_sprintf(0, 0, "posx=%d", sprites[sprite].posx);
    //draw_sprintf(0, 1, "posy=%d", sprites[sprite].posy);
    if (sprites[sprite].anim == RYU_HADPROJ_START && sprites[sprite].anim_idx == 1)
    {
      sprites[sprite].anim = RYU_HADPROJ;
      sprites[sprite].anim_idx = 0;
    }
    else if (sprites[sprite].anim == RYU_HADPROJ && sprites[sprite].anim_idx == 11)
    {
      if (sprites[sprite].posx <= 3 || sprites[sprite].posx > 28)
      {
        sprites[sprite].anim=RYU_HADPROJ_END;
        sprites[sprite].anim_idx=0;
        if (sprites[sprite].dir)
          sprites[sprite].posx -= 6;
        else
          sprites[sprite].posx += 6;
      }
      else
      {
        if (sprites[sprite].dir)
          sprites[sprite].posx -= 6;
        else
          sprites[sprite].posx += 6;
      }
    }
    else if (sprites[sprite].anim == RYU_HADPROJ_END && sprites[sprite].anim_idx == 3)
    {
      sprites[sprite].visible=0;
    }
  }

  return 0;
}

void drawbox(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned char white)
{
  unsigned int len = x2 - x1;
  unsigned char offs = x1 & 0x07;
  unsigned char offs2, offsy;

	rel_loc = vicbase+0x2000;
  gtmpw = rel_loc;
  gtmpw3 = rel_loc;

  // take offset to the start of which character block the first pixel resides in
  rel_loc += (y1 >> 3) * 320 + (x1 & 0xfff8);
  // now take offset to the start of the row within the char-block
  rel_loc += (y1 & 0x07);

  a = 0xff >> offs;
  b = a ^ 0xff;
  if (white)
    Poke(rel_loc, Peek(rel_loc) & b);
  else
    Poke(rel_loc, Peek(rel_loc) | a);
  // back up this location for later
  gtmpw2 = rel_loc;

  // do the same for the lower line
  gtmpw += (y2 >> 3) * 320 + (x1 & 0xfff8);
  gtmpw += (y2 & 0x07);

  if (white)
    Poke(gtmpw, Peek(gtmpw) & b);
  else
    Poke(gtmpw, Peek(gtmpw) | a);

  // draw the rest of the horizontal line, in 8-bit chunks
  offs2 = x2 & 0x07;
  len -= offs2;
  rel_loc += 8;
  gtmpw += 8;
  while (len > 8)
  {
    if (white)
    {
      Poke(rel_loc, 0x00);
      Poke(gtmpw, 0x00);
    }
    else
    {
      Poke(rel_loc, 0xff);
      Poke(gtmpw, 0xff);
    }
    len -=8;
    rel_loc += 8;
    gtmpw += 8;
  }

  // draw any trailing end
  a = 0xff << (7 - offs2);
  b = a ^ 0xff;
  if (white)
  {
    Poke(rel_loc, Peek(rel_loc) & b);
    Poke(gtmpw, Peek(gtmpw) & b);
  }
  else
  {
    Poke(rel_loc, Peek(rel_loc) | a);
    Poke(gtmpw, Peek(gtmpw) | a);
  }

  // draw the vertical lines...
  offsy = y1 & 0x07;
  gtmpw3 += (y1 >> 3) * 320 + (x2 & 0xfff8);
  gtmpw3 += (y1 & 0x07);
  len = y2-y1;

  a = 1 << (7-offs);
  b = a ^ 0xff;

  gk = 1 << (7-offs2);
  gtmp = gk ^ 0xff;

  while (len > 1)
  {
    len--;
    offsy++;
    if (offsy == 8)
    {
      gtmpw2 += 320;
      gtmpw2 &= 0xfff8;
      gtmpw3 += 320;
      gtmpw3 &= 0xfff8;
      offsy=0;
    }
    else
    {
      gtmpw2++;
      gtmpw3++;
    }

    if (white)
    {
      Poke(gtmpw2, Peek(gtmpw2) & b);
      Poke(gtmpw3, Peek(gtmpw3) & gtmp);
    }
    else
    {
      Poke(gtmpw2, Peek(gtmpw2) | a);
      Poke(gtmpw3, Peek(gtmpw3) | gk );
    }
  }

}


unsigned char num;
reu_row_segment* seg;
unsigned int len;
unsigned long bmp_data_loc;

void draw_fullwidth_bitmap(unsigned int frame, int posx, int posy)
{
  num = segbmps[frame].num_segments;

  // copy segments metadata from reu (in bank0) to main memory (at 0x4000)
  if (num > 0)
  {
    c64loc = 0x4000;
    reuloc= 0x0000 + segbmps[frame].start_segment_idx*sizeof(reu_row_segment);
    length = segbmps[frame].num_segments*sizeof(reu_row_segment);
		reu_simple_copy();

    seg = (reu_row_segment*)0x4000;
		c64loc = (signed int)(vicbase+0x2000) + posx*8 + posy*40*8;
		reuloc = segbmps[frame].reu_ptr; // reu bank 1 contains bitmap data

    length = (40 << 3) * num;
    //c64loc += seg->reloffset;

    reu_simple_copy();
  }
}

unsigned char xoff = 0;
unsigned char yoff = 0;
unsigned int startx = 0;
unsigned int nextx = 0;
unsigned int diffx = 0;

#define REPAIR_CHAR_NO_TRANSPARENCY \
			gtmp = Peek(c64loc); \
			a = *(ptr); \
			ptr++; \
			ptr++; \
			Poke(c64loc, a); \
			c64loc++;

#define REPAIR_CHAR \
			gtmp = Peek(c64loc); \
			a = *(ptr); \
			ptr++; \
			b = *(ptr); \
			ptr++; \
			gtmp &= b; \
			gtmp |= (a & ~b); \
			Poke(c64loc, gtmp); \
			c64loc++;

#ifdef ANIMBKGND
void draw_cropped_bitmap(unsigned int frame, int posx, int posy)
{
  xoff = 0;
  yoff = 0;
  //reu_segged_bmp_obj* segbmps = (reu_segged_bmp_obj*)0x1000;
  num = segbmps[frame].num_segments;

	if (posx < 0)
		xoff = -posx;

  // copy segments metadata from reu (in bank0) to main memory (at 0x4000)
  if (num > 0)
  {
    c64loc = 0x4000;
    reuloc= 0x0000 + segbmps[frame].start_segment_idx*sizeof(reu_row_segment);
    length = segbmps[frame].num_segments*sizeof(reu_row_segment);
		reu_simple_copy();

    seg = (reu_row_segment*)0x4000;

		c64loc = (signed int)(vicbase+0x2000) + posx*8 + posy*40*8;
    startx = (signed int)(vicbase+0x2000) + posy*40*8 + (seg->reloffset / 320 * 320);
    nextx = startx + 320;
		reuloc = segbmps[frame].reu_ptr; // reu bank 1 contains bitmap data

    if (posy < 0)
    {
      yoff = -posy;
      yoff -= seg->reloffset / 320;
    }

    for (gk = 0; gk < num; gk++)
    {
      length = seg->length << 3;
      c64loc += seg->reloffset;
      
      // check for left-side cropping
      if (c64loc < startx)
      {
        diffx = startx - c64loc;
        if (length > diffx)
        {
          length -= diffx;
          reuloc += diffx;
          c64loc += diffx;
        }
        else
        {
          //exit(0);
          goto skip;
        }
      }

      // completely cropped to the right?
      if (c64loc >= nextx)
      {
        goto skip;
      }

      // check for right-side cropping
      if (c64loc + length > nextx)
      {
        diffx = c64loc+length - nextx;
        length -= diffx;
        // check for top-side cropping
        if (gk >= yoff)
          reu_simple_copy();
        length += diffx;
        goto skip;
      }

      // check for top-side cropping
      if (gk >= yoff)
        reu_simple_copy();

skip:
    //printf("%d - %d - %lu\n", seg->reloffset, screen_loc, bmp_data_loc);
    //if (k == 1)
    //{
    //while(1)
    //  ;
    //}

      reuloc += length;
      c64loc += length;
      seg++;
      startx = nextx;
      nextx += 320; //40*8;
    } // end for k

  }

  // draw repairs
  num_repairs = segbmps[frame].num_repairs;
  //if (num_repairs > 0 && option_background == BKGND_ANIM_REPAIR)
  {
    if (posy < 0)
    {
      yoff = -posy;
    }

    // copy across hitbox + repair data to temp memory
		c64loc = 0x4000;
		length = (4*4*2) + num_repairs*18;
    reu_simple_copy();

    c64loc = vicbase+0x2000 + posx*8 + posy*40*8;
		screen_loc = vicbase+0x2000;
		rel_loc = 0;
    startx = (signed int)(vicbase+0x2000) + posy*40*8;
    nextx = startx + 320;

    repair = (reu_repair_obj*)(0x4000 + (4*4*2));
    for (gk = 0; gk < num_repairs; gk++)
    {
      c64loc += repair->reloffset;
			rel_loc += repair->reloffset;

			// move my line range up to where the repair is
			while (rel_loc > 320)
			{
				rel_loc -= 320;
				startx = nextx;
				nextx += 320;
			}

			// check for left-side cropping
			if (c64loc < startx || c64loc >= nextx)
			{
				goto skip2;
			}


			// check for top-side cropping
			if (c64loc < screen_loc)
				goto skip2;

			ptr = &(repair->vals[0]);

			REPAIR_CHAR
			REPAIR_CHAR
			REPAIR_CHAR
			REPAIR_CHAR
			REPAIR_CHAR
			REPAIR_CHAR
			REPAIR_CHAR
			REPAIR_CHAR

      c64loc -= 8;
skip2:
      repair++;
    } // end for
  } // end if

#ifdef HITBOXES
  // draw hitboxes
  for (gk = 0; gk < 4; gk++)
  {
    // TODO: replicate the logic in draw_bitmap, if I ever need it here...
    drawbox();
  }
#endif
}
#endif

#ifdef ANIMBKGND
void draw_sky_bitmap(int frame)
{
  //reu_segged_bmp_obj* segbmps = (reu_segged_bmp_obj*)0x1000;
  num = segbmps[frame].num_segments;

  frame += (sky_idx & 0x07);


  // copy segments metadata from reu (in bank0) to main memory (at 0x4000)
  if (num > 0)
  {
    c64loc = 0x4000;
    reuloc= 0x0000 + segbmps[frame].start_segment_idx*sizeof(reu_row_segment);
    length = segbmps[frame].num_segments*sizeof(reu_row_segment);
		reu_simple_copy();

    seg = (reu_row_segment*)0x4000;

    // loop-over point?
    if (sky_idx == (seg->length << 3)-320)
      sky_idx = 0;

		c64loc = (signed int)(vicbase+0x2000);
		reuloc = segbmps[frame].reu_ptr; // reu bank 1 contains bitmap data
    reuloc += ((sky_idx >> 3) << 3);


    for (gk = 0; gk < num; gk++)
    {
      length = 40*8;
      //c64loc += seg->reloffset;

      reu_simple_copy();

      reuloc += (seg->length << 3);
      c64loc += 40*8;
      seg++;
    } // end for k

  }

}
#endif

void draw_bitmap(unsigned int frame, int posx, int posy)
{
#ifndef SAVEMEM
  //reu_segged_bmp_obj* segbmps = (reu_segged_bmp_obj*)0x1000;
  num = segbmps[frame].num_segments;

  // copy segments metadata from reu (in bank0) to main memory (at 0x4000)
  if (num > 0)
  {
    c64loc = 0x4000;
    reuloc= 0x0000 + segbmps[frame].start_segment_idx*sizeof(reu_row_segment);
    length = segbmps[frame].num_segments*sizeof(reu_row_segment);
		reu_simple_copy();

    seg = (reu_row_segment*)0x4000;

		c64loc = (signed int)(vicbase+0x2000) + posx*8 + posy*40*8;
		reuloc = segbmps[frame].reu_ptr; // reu bank 1 contains bitmap data

#ifdef DRAWSPRITES
    for (gk = 0; gk < num; gk++)
    {
      length = seg->length << 3;
      c64loc += seg->reloffset;

      reu_simple_copy();

    //printf("%d - %d - %lu\n", seg->reloffset, screen_loc, bmp_data_loc);
    //if (k == 1)
    //{
    //while(1)
    //  ;
    //}

      reuloc += length;
      c64loc += length;
      seg++;
    } // end for k
#endif
  }
  else // num_segments == 0
  {
		reuloc = segbmps[frame].reu_ptr; // reu bank 1 contains bitmap data
  }

  // draw repairs
  num_repairs = segbmps[frame].num_repairs;
  //if (num_repairs > 0)
  //{
    // copy across hitbox + repair data to temp memory
		c64loc = 0x4000;
		length = (4*4*2) + num_repairs*18;
    reu_simple_copy();

    c64loc = vicbase+0x2000 + posx*8 + posy*40*8;

#ifdef DRAWSPRITES
    repair = (reu_repair_obj*)(0x4000 + (4*4*2));
    for (gk = 0; gk < num_repairs; gk++)
    {
      c64loc += repair->reloffset;

      ptr = &(repair->vals[0]);
      //for (l = 0; l < 8; l++)
      {
        REPAIR_CHAR
        REPAIR_CHAR
        REPAIR_CHAR
        REPAIR_CHAR
        REPAIR_CHAR
        REPAIR_CHAR
        REPAIR_CHAR
        REPAIR_CHAR
      }
      repair++;
      c64loc -= 8;
    } // end for
#endif
  //} // end if

  posx <<= 3;
  posy <<= 3;

  // draw hitboxes
  if (*((unsigned short*)0x4000) != 0)
  {
    *((unsigned int*)0x4000) += posx;
    *((unsigned int*)0x4002) += posy;
    *((unsigned int*)0x4004) += posx;
    *((unsigned int*)0x4006) += posy;

#ifdef HITBOXES
    drawbox(*((unsigned int*)0x4000),
            *((unsigned int*)0x4002),
            *((unsigned int*)0x4004),
            *((unsigned int*)0x4006), 0);
#endif
  }

  if (*((unsigned int*)0x4008) != 0)
  {
    *((unsigned int*)0x4008) += posx;
    *((unsigned int*)0x400a) += posy;
    *((unsigned int*)0x400c) += posx;
    *((unsigned int*)0x400e) += posy;

#ifdef HITBOXES
    drawbox(*((unsigned int*)0x4008),
            *((unsigned int*)0x400a),
            *((unsigned int*)0x400c),
            *((unsigned int*)0x400e), 0);
#endif
  }

  if (*((unsigned int*)0x4010) != 0)
  {
    *((unsigned int*)0x4010) += posx;
    *((unsigned int*)0x4012) += posy;
    *((unsigned int*)0x4014) += posx;
    *((unsigned int*)0x4016) += posy;

#ifdef HITBOXES
    drawbox(*((unsigned int*)0x4010),
            *((unsigned int*)0x4012),
            *((unsigned int*)0x4014),
            *((unsigned int*)0x4016), 0);
#endif
  }

  if (*((unsigned int*)0x4018) != 0)
  {
      *((unsigned int*)0x4018) += posx;
      *((unsigned int*)0x401a) += posy;
      *((unsigned int*)0x401c) += posx;
      *((unsigned int*)0x401e) += posy;

#ifdef HITBOXES
      drawbox(*((unsigned int*)0x4018),
              *((unsigned int*)0x401a),
              *((unsigned int*)0x401c),
              *((unsigned int*)0x401e), 0);
#endif
  }
#endif
}


#define OPTIONS_X	4
#define OPTIONS_Y 2
#define OPTIONS_H 5

#ifdef DEBUG
char dstr[40];
void draw_sprintf(unsigned char posx, unsigned char posy, char* str, ...)
{
  va_list args;
  va_start (args, str);
  vsprintf(dstr, str, args);
  draw_text(dstr, posx, posy, 0);
}

void draw_text(char* str, unsigned char posx, unsigned char posy, unsigned char invert)
{
	screen_loc = (vicbase+0x2000) + posx*8 + posy*40*8;

	a = 0;
	while (str[a] != 0)
	{
		c64loc = 0x5c00 + ( (str[a] & 0x3f) << 3); // base of my character data

		if (invert)
		{
#define INV_CHAR_LINE \
		Poke(screen_loc, ~Peek(c64loc)); \
		screen_loc++; \
		c64loc++;
			INV_CHAR_LINE
			INV_CHAR_LINE
			INV_CHAR_LINE
			INV_CHAR_LINE
			INV_CHAR_LINE
			INV_CHAR_LINE
			INV_CHAR_LINE
			INV_CHAR_LINE
		}
		else
		{
#define CHAR_LINE \
		Poke(screen_loc, Peek(c64loc)); \
		screen_loc++; \
		c64loc++;
			CHAR_LINE
			CHAR_LINE
			CHAR_LINE
			CHAR_LINE
			CHAR_LINE
			CHAR_LINE
			CHAR_LINE
			CHAR_LINE
		}

		a++;
	}
}
#endif

#ifdef SHOW_OPTIONS

unsigned char menu_highlight = 0;
unsigned char up_pressed = 0;
unsigned char down_pressed = 0;
char* strSelected = "(*)";
char* strUnselected = "( )";
#define TOTAL_MENU_ITEMS	5

void draw_options(void)
{
	draw_text(option_background == BKGND_STATIC ? strSelected : strUnselected, 6, 7, 0);
	draw_text("static background",           10, 7, menu_highlight==0);
	
	draw_text(option_background == BKGND_ANIM ? strSelected : strUnselected, 6, 8, 0);
	draw_text("anim. background",            10, 8, menu_highlight==1);

	draw_text(option_background == BKGND_ANIM_REPAIR ? strSelected : strUnselected, 6, 9, 0);
	draw_text("anim. background+repairs",    10, 9, menu_highlight==2);

	draw_text("return to game",              10, 11, menu_highlight==3);
	draw_text("exit to title",               10, 13, menu_highlight==4);
}

void game_options(void)
{
	unsigned char k;
  unsigned char key;

	menu_highlight = 0;

	// jump back to the currently visible page and draw on that
	if (draw_page == 1)
		vicbase=0x0000;
	else
		vicbase=0x4000;
	
	// draw border
	draw_bitmap(OPTIONS_BORDER_TOP, OPTIONS_X, OPTIONS_Y);
	for (k = 0; k < OPTIONS_H; k++)
		draw_bitmap(OPTIONS_BORDER_MID, OPTIONS_X, OPTIONS_Y+2+ k*2);
	draw_bitmap(OPTIONS_BORDER_BOTTOM, OPTIONS_X, OPTIONS_Y+4+OPTIONS_H*2);

	// draw actual options
	draw_text("options", 17, 4, 0);
	draw_text("=======", 17, 5, 0);

	draw_options();

	while(1)
	{
		check_real_keyboard();

		// check for escape key
		if (keycode == 0x77)
		{
			if (!escdown)
			{
				escdown = 1;
				gamestate = GAME_MAIN;

				// jump back to the currently visible page and draw on that
				if (draw_page == 0)
					vicbase=0x0000;
				else
					vicbase=0x4000;

				break;
			}
		}
		else
		{
			escdown = 0;
		}

		// JOYSTICK: left=4, right=8, up=1, down=2, fire=16
		key  = (~Peek(56320U)) & 31; //cgetc();

		// fire key
		if (key & 16)
		{
			if (!firedown[0])
			{
				firedown[0] = 1;
				if (menu_highlight == 0) option_background = BKGND_STATIC;
				else if (menu_highlight == 1) option_background = BKGND_ANIM;
				else if (menu_highlight == 2) option_background = BKGND_ANIM_REPAIR;
				// are we exiting the options menu?
				else
				{
					if (menu_highlight == 3)
						gamestate = GAME_MAIN;
					else if (menu_highlight == 4)
						gamestate = GAME_TITLE;

					// jump back to the currently visible page and draw on that
					if (draw_page == 0)
						vicbase=0x0000;
					else
						vicbase=0x4000;

					break;
				}

				draw_options();
			}
			else
			{
				firedown[0] = 0;
			}
		}

		// up key
		if (key & 1)
		{
			if (!up_pressed)
			{
				up_pressed = 1;
				if (menu_highlight == 0)
					menu_highlight = TOTAL_MENU_ITEMS - 1;
				else
					menu_highlight--;

				draw_options();
			}
		}
		else
		{
			up_pressed = 0;
		}

		// down key
		if (key & 2)
		{
			if (!down_pressed)
			{
				down_pressed = 1;
				menu_highlight++;
				if (menu_highlight == TOTAL_MENU_ITEMS)
					menu_highlight = 0;

				draw_options();
			}
		}
		else
		{
			down_pressed = 0;
		}
	} // end while(1)
}
#endif

void draw_screen(unsigned char idx)
{
  // copy screen 0 chars
  c64loc = 0x400;
  reuloc = 0x10000 + idx*2000L;
  length = 1000;
  reu_simple_copy();

  c64loc = 0xd800;
  reuloc = 0x10000 + 1000L + idx*2000L;
  length = 1000;
  reu_simple_copy();
}

unsigned char irqs;

unsigned char intro_irq(void)
{
  irqs = Peek(0xd019);

  __asm__ ( "jsr " FUNC_MUSIC_LOOP_ITERATION);

  return IRQ_HANDLED;
}

void prep_song(unsigned char idx)
{
  c64loc = 0xE000;
  reuloc = 0x20000 + idx * 0x0a00;
  length = 0x0a00;
  reu_simple_copy();
  prepare_song(lstVoiceOffsets[idx].v1, lstVoiceOffsets[idx].v2, lstVoiceOffsets[idx].v3, 32, 3, 0);
}

void game_intro(void)
{
  unsigned char key;
  unsigned char scr_idx = 0;
  unsigned int cnt = 0;
	
  Poke(53281L,0);

  __asm__ ( "sei" );
  Poke(0xd011, Peek(0xd011) & 127); // turning off raster register MSB bit-8
  Poke(0xdc0d, 127); // switch off interrupt signals from cia-1 (https://www.c64-wiki.com/wiki/Raster_interrupt)
  Poke(0xdd0d, 127); // stop the cursor flash interrupt? (http://codebase64.org/doku.php?id=base:introduction_to_raster_irqs)
  Poke(0xdd0d, 0x7f); // turn off cia interrupts (https://dustlayer.com/c64-coding-tutorials/2013/4/8/episode-2-3-did-i-interrupt-you)

  // speed up the tempo a bit :)
  Poke(0xdc04, 10); // LO: set CIA#1 TimerA value
  Poke(0xdc05, 52); // HI: set CIA#1 TimerA value

  Poke(0xdc0d, 0x81); // turn on CIA#2 timera interrupt only
  //Poke(0xdd0f, /*(1<<5) +*/ (1<<4) + (1<<3) + 1); // CIA#1 Control Register B (count positive transitions, force load, start timer)

/*
  Poke(0xdc0f, 0); (1<<5) + (1<<4) + 1); // CIA#1 Control Register B (count positive transitions, force load, start timer)


  Poke(0xdc0d, 0x7f);
  Poke(0xdc0d, 0x82); // CIA#1 Interrupt Register: turn on CIA#1 TimerB interrupt
  Peek(0xdc0d);
*/
  __asm__ ( "cli" );

  reset_irq();
  __asm__ ( "jsr " FUNC_PREPARE_SID);
  //prepare_intro_song();
  prep_song(SONG_INTRO);
  __asm__ ( "jsr " FUNC_MUSIC_LOOP_PREPARATION);
  set_irq(intro_irq, (void *)0xca00, 200);

  draw_screen(0);

  while(1)
  {
    if (screen_flag)
    {
      screen_flag = 0;
      cnt = 0;
      break;
    }
    cnt++;
    if (cnt == 1000)
    {
      cnt = 0;
      scr_idx = (scr_idx + 1) % 4;
      draw_screen(scr_idx);
    }
  }

  scr_idx = 4;
  draw_screen(scr_idx);
  play_sound(SND_PUNCH1);

  while(1)
  {
    cnt++;
    if (cnt == 1000)
    {
      cnt = 0;
      scr_idx++;
      if (scr_idx >= 28)
        scr_idx = 27;
      draw_screen(scr_idx);
    }

    // wait for fire-button
    key  = (~Peek(56320U)) & 31; //cgetc();

		if (key & 16)
		{
			if (!firedown[0])
			{
				firedown[0] = 1;
        clear_petscii();
				clear_screen();

				vicbase=0x4000;
				draw_page = 1;

				gamestate = GAME_TITLE;
        //reset_irq();
				return;
			}
		}
		else
		{
			firedown[0] = 0;
		}
  }
}

void play_sound(unsigned char idx)
{
  // copy sound into unused draw-buffer
  if (gamestate == GAME_MAIN)
    c64loc = ( (vicbase) +0x2000);
  else
    c64loc = ( (vicbase^0x4000) +0x2000);
  reuloc = 0x30000 + 0x2000L * idx;
  length = 0x2000;
  reu_simple_copy();

  // grab the number of samples in the wave file
  gtmpw = *((unsigned int*)c64loc);
  c64loc += 2;
  gtmpw2 = c64loc + gtmpw;
  gtmpw3 = c64loc;

  // now jiggle the main volume up and down according to each 4-bit sample :)
  while (gtmpw)
  {
    Poke(54296, *(unsigned char*)gtmpw3);
    //Poke(0xd020, *(unsigned char*)gtmpw3);
    gtmpw3++;
    gtmpw--;
  }

  //draw_sprintf(0, 0, "0x%04X", gtmpw3);
  // back to full volume
  Poke(54296, 0x0f);
}

//#ifndef SAVEMEM
void game_title(void)
{
  unsigned char key;

  // Select Bank1 for bitmap pages
  //Poke(56578, Peek(56578) | 3); // make sure CIA bits 0+1 are set to outputs (DDR)
  //Poke(56576, (Peek(56576) & 252) | 2); // change to vic bank1 ($4000-$7fff)

  // Try a REU ram transfer
  Poke(REC_ADDR_CTRL, 0); // make sure both addresses are counted up

  clear_screen();

  // Put bitmap at 8192
  Poke(53272L, Peek(53272L) | 8);

  // Enter bitmap mode
  Poke(53265L, Peek(53265L) | 32);

	// jump back to the currently visible page and draw on that
	if (draw_page == 1)
		vicbase=0x0000;
	else
		vicbase=0x4000;
	
  draw_bitmap(TITLE, 0, 0);

  a=0;
  while(1)
  {
    check_real_keyboard();

    // check for escape key
    if (keycode == 0x77)
    {
      if (!escdown)
      {
        escdown = 1;
        play_sound(a);
        a++;
        if (a >= 39)
          a = 0;
      }
    }
    else
    {
      escdown = 0;
    }

    // wait for fire-button
    key  = (~Peek(56320U)) & 31; //cgetc();

		if (key & 16)
		{
			if (!firedown[0])
			{
				firedown[0] = 1;

        draw_bitmap(PLAYER_SELECT, 0, 0);
        play_sound(SND_FLIGHT1);
        play_sound(SND_FLIGHT2);
        play_sound(SND_FLIGHT3);

        draw_fullwidth_bitmap(BLANK, 0, 0);
				//clear_screen();

				vicbase=0x4000;
				draw_page = 1;

				gamestate = GAME_MAIN;

        __asm__ ( "cli" );

        // speed up the tempo a bit :)
        Poke(0xdc04, 10); // LO: set CIA#1 TimerA value
        Poke(0xdc05, 52); // HI: set CIA#1 TimerA value

        reset_irq();
        __asm__ ( "jsr " FUNC_PREPARE_SID);
        //prepare_game_song();
        energies_buffered = 0;
        prep_song(SONG_GAME);
        __asm__ ( "jsr " FUNC_MUSIC_LOOP_PREPARATION);
        set_irq(intro_irq, (void *)0xca00, 200);

				return;
			}
		}
		else
		{
			firedown[0] = 0;
		}
  }
}
//#endif

// the caller should set reudir to decide the direction of copying
void prepare_energy_bar(void)
{
  if (reudir && !energies_buffered) // c64-to-buffered-reu
  {
    // draw it on-screen, then we will buffer it
    reudir = 0;
    draw_bitmap(ENERGY_BARS, 3, 2); //25, 20);
    reudir = 1;
  }

  // 272x16
  c64loc = (signed int)(vicbase+0x2000) + 3*8 + 2*40*8;
  reuloc = 0x2fd00;
  length = 0x110;
  reu_simple_copy();

  c64loc += 40*8;
  reuloc += 40*8;
  reu_simple_copy();

  reudir=0; // reset reudir to default
}

void draw_energy_bars(void)
{
  if (energies_buffered)
  {
    reudir = 0; // buffered-reu-to-c64
  }
  else
  {
    reudir = 1; // c64-to-buffered-reu
  }
  prepare_energy_bar();
  energies_buffered = 1;

  for (a = 0; a < 2; a++)
  {
    // check if energy level has changed
    if (last_energies[a] != energies[a])
    {
      if (last_energies[a] >= 120)
        continue;
      last_energies[a]++;
      //draw_sprintf(0, 0, "last=%d, energ=%d", last_energies[0], energies[0]);
      gtmpw = vicbase+0x2000 + 3*8 + 2*40*8;
      if (a == 0)
        gtmpw2 = 1 + last_energies[a];  // my x-pos
      else
        gtmpw2 = 270 - last_energies[a];  // my x-pos
      gtmpw3 = 4; // my y-pos

      for (gtmpw3 = 3; gtmpw3 < 14; gtmpw3++)
      {
        if (((gtmpw3 & 0x01) && (gtmpw2 & 0x01)) ||
            (!(gtmpw3 & 0x01) && ((gtmpw2+1) & 0x01)))
        {
          gtmpw4 = gtmpw + (gtmpw3>>3)*40*8 + (gtmpw2>>3)*8 + (gtmpw3 & 0x07);
          Poke(gtmpw4, Peek(gtmpw4) | (1 << (7- (gtmpw2 & 0x07))));
        }
      }
      reudir=1;
      prepare_energy_bar();
    }
  }
}

#define INTERSECT(b1, b2) \
  !(b2[0] > b1[2] \
  || b2[2] < b1[0] \
  || b2[1] > b1[3] \
  || b2[3] < b1[1])

//#ifndef SAVEMEM
void game_main(void)
{
  unsigned int i, k;

  get_keyboard_input();

  // draw scenery first

	// use static background?
	if (option_background == BKGND_STATIC)
	{
#ifdef BLANKSCREEN
  draw_fullwidth_bitmap(BLANK, 0, 0);
#else
    draw_fullwidth_bitmap(RYU_STAGE_CROPPED, 0, 0);
#endif
	}
#ifdef ANIMBKGND
	// use animated background?
	else
	{
		// draw sky
		draw_sky_bitmap(STAGE_RYU_SKY1);
		sky_idx++;

		//__asm__ ( "jsr $4800" );

		// draw temple
		draw_bitmap(STAGE_RYU_TEMPLE1 + ((unsigned int)(temple_idx>>2) & 0x07), 10 - (temple_idx>>5), 4);

		// draw fence
		draw_cropped_bitmap(STAGE_RYU_FENCE_LEFT1 + ((unsigned int)(fence_idx>>1) & 0x07), 0 - (fence_idx>>4), 12);

		draw_cropped_bitmap(STAGE_RYU_FENCE_RIGHT1 + ((unsigned int)(fence_idx>>1) & 0x07), 25 - (fence_idx>>4), -1);

		// draw building
		draw_cropped_bitmap(STAGE_RYU_BUILDING_LEFT1 + ((unsigned int)building_idx & 0x07), -8 - (building_idx>>3), -2);

		draw_cropped_bitmap(STAGE_RYU_BUILDING_RIGHT1 + ((unsigned int)building_idx & 0x07), 26 - (building_idx>>3), 11);

		// draw floor at desired index
		draw_fullwidth_bitmap(STAGE_RYU_FLOOR00 + floor_idx, 0, 20);
	}
#endif

	cur_spr = sprites;
	// draw all visible sprites
  for (i = 0; i < SPR_MAX; i++)
  {
    if (cur_spr->visible)
    {
			gtmpw = anims[cur_spr->anim].frames[cur_spr->anim_idx];
			if (cur_spr->dir) // mirror to other direction?
      {
				gtmpw += TITLE_REV;
        gtmpw2 = cur_spr->posx - anims[cur_spr->anim].cols;
        gtmpw3 = cur_spr->posy - anims[cur_spr->anim].rows;
      }
      else
      {
        gtmpw2 = cur_spr->posx;
        gtmpw3 = cur_spr->posy - anims[cur_spr->anim].rows;
      }
      draw_bitmap(gtmpw, gtmpw2, gtmpw3);

      // preserve hitboxes?
      //if (*((unsigned short*)0x4000) != 0)
      {
        ptr = (unsigned char*)&cur_spr->boxes;
        ptr2 = (unsigned char*)0x4000;

        for (a=0; a<32; a++)
        {
          *ptr = *ptr2;
          ptr++;
          ptr2++;
        }
      }

      if (post_draw_processing(i) == 0)
      {
        animate_sprite(&(sprites[i]));
      }
    }
		cur_spr++;
  }

  draw_energy_bars();

  // test if player1 hits player2
  for (gk = 0; gk < 2; gk++) // player1-to-2 or player2-to-1
  {
    if (gk == 0)
    {
      a = 0;
      b = 1;
    }
    else
    {
      a = 1;
      b = 0;
    }

    for (gtmp = 0; gtmp < 3; gtmp++)
    {
      hit = 0;
      // player hit opponent's head?
      if (sprites[a].boxes[3][0] != 0 && INTERSECT(sprites[a].boxes[3], sprites[b].boxes[gtmp]))
        hit = 1;
      
      // player's hadouken projectile hit head?
      if (sprites[a+2].boxes[3][0] != 0 && INTERSECT(sprites[a+2].boxes[3], sprites[b].boxes[gtmp])) // hadproj hit?
      {
        hit = 1;
        sprites[a+2].anim=RYU_HADPROJ_END;
        sprites[a+2].anim_idx=0;
        if (sprites[a+2].dir)
          sprites[a+2].posx = sprites[a+2].boxes[3][2] >> 3;
        else
          sprites[a+2].posx = sprites[a+2].boxes[3][0] >> 3;
      }

      if (hit)
      {
        energies[b] = last_energies[b] + 8;
        //Poke(0xd020, energies[0]);
        if (gtmp == 0) sprites[b].anim = RYU_FACEHIT;
        if (gtmp == 1) sprites[b].anim = RYU_HIT;
        if (gtmp == 2) sprites[b].anim = RYU_KNOCKDOWN;
        sprites[b].anim_idx = 0;
        sprites[b].anim_dir = 1;
        punching[b]=1;
        break;
      }
    }
  }

  //drawbox(50+bx, 50+by, 100+cx, 100+cy, 0);

  //draw_sprintf(0, 0, "anim_idx=%d", sprites[0].anim_idx);
  //draw_sprintf(0, 1, "frame=%d", anims[sprites[0].anim].frames[sprites[0].anim_idx]);

  // perform page flip
  if (draw_page == 0)
  {
    Poke(56578L, Peek(56578L) | 3); // set bits0+1 to be output bits (to permit bank selection)
    Poke(56576L, (Peek(56576L) & 252) + 3); // select bank 0
    vicbase=0x4000;
    draw_page = 1;
  }
  else
  {
    Poke(56578L, Peek(56578L) | 3); // set bits0+1 to be output bits (to permit bank selection)
    Poke(56576L, (Peek(56576L) & 252) + 2); // select bank 1
    vicbase=0x0000;
    draw_page = 0;
  }

  // sounds
  if (snd_trigger)
  {
    if (snd_delay)
    {
      snd_delay--;
    }
    else
    {
      play_sound(snd_idx);
      snd_trigger--;
      snd_idx++;
    }
  }
  //if (anims[sprites[i].anim].frames[sprites[i].anim_idx]== RYU_IDLE2)
  //  exit(0);

  // add a delay
  //for (k=0; k < 500; k++)
  //  ;
}
//#endif // SAVEMEM

void main(void)
{
  unsigned int i, j, seg_idx;
  unsigned long loc;
  reu_row_segment* seg;

  Poke(53280L,0);

  // switch back to upper-case
  // https://www.cc65.org/mailarchive/2004-09/4446.html
  Poke(0xd018, 0x15);

  // size is 7 x 14 char-blocks
  // = 784 bytes = 0x0310
  // draw it one row at a time

  // copy kernal into ram
  for (gtmpw = 0xe000; gtmpw < 0xffff; gtmpw++)
    Poke(gtmpw, Peek(gtmpw));

  Poke(0x01, 0x35); // turn off BASIC and KERNAL ROMs

  // ignore most of the guts of the kernal's standard irq handler
  Poke(0xea31, 0x4c);
  Poke(0xea32, 0x7e);
  Poke(0xea33, 0xea);

  loc = 0x00080000;
  seg_idx = 0;
  for (i = 0; i < BMP_MAX; i++)
  {
    segbmps[i].start_segment_idx = seg_idx;
    segbmps[i].reu_ptr = loc;

    //Poke(0x400L+i, 0);
    //if (i == 112)
    //{
    //  Poke(0x400L, segbmps[i].num_segments);
    //  PokeW(0x401L, &segbmps[i]);
    //  return;
    //}

    // copy segments metadata from reu (in bank0) to main memory (at 0x4000)
    if (segbmps[i].num_segments > 0)
    {
			c64loc = 0x4000;
			reuloc = 0x0000 + seg_idx*sizeof(reu_row_segment);
			length = segbmps[i].num_segments*sizeof(reu_row_segment);
      reu_simple_copy();

      seg = (reu_row_segment*)0x4000;
      for (j = 0; j < segbmps[i].num_segments; j++)
      {
        loc += (seg[j].length << 3);
      }
      seg_idx += segbmps[i].num_segments; // get this ready for the next segged bmp
    }
    loc += (4 * 4 * 2); // four hitboxes consisting of {x0, y0, x1, y1} (unsigned shorts)
    loc += segbmps[i].num_repairs * 18; // 18 = sizeof(reu_repair_obj)
  }

  while(1)
  {
    switch(gamestate)
    {
      case GAME_INTRO:
        game_intro();
        break;

      case GAME_TITLE:
//#ifndef SAVEMEM
        game_title();
//#endif
        break;

      case GAME_MAIN:
//#ifndef SAVEMEM
        game_main();
//#endif
        break;

			case GAME_OPTIONS:
#ifdef SHOW_OPTIONS
				game_options();
#endif
				break;
    }
  } // end while
}
