#include <stdio.h>
#include <stdlib.h>

#include "bitmap_ids.h"

#define Poke(A,X)  (*(unsigned char *)(A)) = (X)
#define Peek(A)    (*(unsigned char *)(A))
#define PokeW(A,X) (*(unsigned int *)(A)) = (X)
#define PeekW(A)   (*(unsigned int *)(A))

// ================================
// GLOBALS
// ================================
unsigned int screen_loc;
unsigned char a, b, gk, gtmp, num_repairs;
unsigned char* ptr;

unsigned char firedown=0;
unsigned char jumping=0;
unsigned char walkingright=0;
unsigned char walkingback=0;
unsigned char crouching=0;
unsigned char punching=0;


enum anim_ids
{
  RYU_IDLE,
  RYU_WALK, RYU_WALKB,
  RYU_JUMP, RYU_FJUMP,
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
  RYU_MUGSHOT,
  RYU_MAX
};

#define FIRST_ATTACK  RYU_LPUNCH // RYU_HADOUKEN // RYU_JUMP_LMHPUNCH

unsigned char punch_style = FIRST_ATTACK;

unsigned int anim_ryu_idle[] = { RYU_IDLE2, RYU_IDLE3, RYU_IDLE4 };
unsigned int anim_ryu_walk[] = { RYU_WALK1, RYU_WALK2, RYU_WALK3, RYU_WALK4, RYU_WALK5, RYU_WALK6, RYU_WALK7, RYU_WALK8 };
unsigned int anim_ryu_walkb[] = { RYU_WALK8, RYU_WALK7, RYU_WALK6, RYU_WALK5, RYU_WALK4, RYU_WALK3, RYU_WALK2, RYU_WALK1 };
unsigned int anim_ryu_jump[] = { RYU_JUMP2, RYU_JUMP3, RYU_JUMP4 };
unsigned int anim_ryu_fjump[] = { RYU_FJUMP2, RYU_FJUMP3, RYU_FJUMP4, RYU_FJUMP5, RYU_FJUMP6, RYU_FJUMP7 };
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
unsigned int anim_ryu_mugshot[] = { RYU_MUGSHOT1, RYU_MUGSHOT2, RYU_MUGSHOT3 };

typedef struct
{
  unsigned char anim_idx;
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
  0,
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
  0,
  10,
  ryu_fjump_relx,
  ryu_fjump_rely,
  ryu_fjump_frame
};

char ryu_bjump_relx[]  = {  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1 };
char ryu_bjump_rely[]  = { -3, -3, -2, -1, -1,  1, 1, 2, 3, 3 };
char ryu_bjump_frame[] = {  6,  5,  4,  3,  3,  2, 2, 1, 1, 0 };

anim_movement ryu_anim_bjump =
{
  0,
  10,
  ryu_bjump_relx,
  ryu_bjump_rely,
  ryu_bjump_frame
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
  { anim_ryu_idle,  3, 1, 7,  13 }, // RYU_IDLE
  { anim_ryu_walk,  8, 0, 8,  13 }, // RYU_WALK
  { anim_ryu_walkb,  8, 0, 8,  13 }, // RYU_WALKB
  { anim_ryu_jump,  3, 1, 7,  13 }, // RYU_JUMP
  { anim_ryu_fjump,  7, 0, 10, 13 }, // RYU_FJUMP
  { anim_ryu_crouchblock,  4, 0, 7, 13 },  // RYU_CROUCHBLOCK
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
  { anim_ryu_hadproj,  12, 0, 9, 8  }, // RYU_HADPROJ
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
  { anim_ryu_mugshot,  3, 0, 11, 15 }, // RYU_MUGSHOT
};

typedef struct
{
  unsigned char posx, posy;
  unsigned char anim;
  unsigned char anim_idx;
  unsigned char anim_dir;
  unsigned char visible;
  anim_movement *anim_movement;
} sprite_detail;

#define SPR_MAX 2

sprite_detail sprites[SPR_MAX] =
{
  { 4,  23,  RYU_IDLE, 0, 1, 1, NULL },
  { 10, 15,  RYU_HADPROJ_START, 0, 1, 0, NULL },

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

void reu_simple_copy(int c64loc, unsigned long reuloc, unsigned int length)
{
  // c64base = 8192 = 0x2000
  Poke(REC_C64_ADDR_LO, c64loc & 0xff);
  Poke(REC_C64_ADDR_HI, c64loc >> 8);
  Poke(REC_REU_ADDR_LO, reuloc & 0xff);
  Poke(REC_REU_ADDR_HI, (reuloc >> 8) & 0xff);
  Poke(REC_REU_ADDR_BANK, (reuloc >> 16) & 0xff);
  Poke(REC_TXFR_LEN_LO, length & 0xff);
  Poke(REC_TXFR_LEN_HI, length >> 8);
  // REU to c64 with immediate execution
  Poke(REC_COMMAND, 0x91); // %10010001
}

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
 
unsigned int vicbase = 0x0000;
unsigned int draw_page = 0;

void clear_screen(void)
{
  unsigned int i;
  // Clear bitmap
  for (i = vicbase+0x2000; i < vicbase+0x2000+8000; i++)
  {
    Poke(i, 0x00);
  }

  // Set bitmap colours
  for (i = vicbase+1024; i < vicbase+2024; i++)
  {
    Poke(i, (0 << 4) + 1);  // black and white
  }
  //
  // Set bitmap colours
  for (i = vicbase+0x4000+1024; i < vicbase+0x4000+2024; i++)
  {
    Poke(i, (0 << 4) + 1);  // black and white
  }
}

void animate_sprite(sprite_detail* spr)
{
  if (spr->anim_movement)
  {
    gk = spr->anim_movement->anim_idx;
    spr->posx += spr->anim_movement->relx[gk];
    spr->posy += spr->anim_movement->rely[gk];
    spr->anim_idx = spr->anim_movement->frame[gk];
    spr->anim_movement->anim_idx++;

    // test for end of jump (probably not the nicest place to put this, but it'll do)
    if (jumping && spr->anim_movement->anim_idx == spr->anim_movement->anim_length)
    {
      jumping=0;
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

void get_keyboard_input(void)
{
  unsigned char key;

  // JOYSTICK: left=4, right=8, up=1, down=2, fire=16
  key  = (~Peek(56320U)) & 31; //cgetc();

  if (!(key & 16) && firedown)  // test if fire was released
  {
    firedown=0;
//    sprites[0].anim = RYU_IDLE;
//    sprites[0].anim_idx = 0;
//    sprites[0].anim_dir = 1;
  }

  // did we release down key?
  if (!(key & 2) && crouching)
  {
    crouching = 0;
    sprites[0].anim = RYU_IDLE;
    sprites[0].anim_idx = 0;
    sprites[0].anim_dir = 1;
  }

  if (key != 0)
  {
    if (key & 16 && !firedown) // fire button
    {
      firedown=1;
      punching=1;
      sprites[0].anim = punch_style;
      sprites[0].anim_idx = 0;
      sprites[0].anim_dir = 1;
      punch_style++;
      if (punch_style == RYU_MAX)
        punch_style = RYU_LPUNCH;
    }
    if (key & 1 && !jumping) // up
    {
      jumping = 1;
      walkingright = 0; // TODO: consider how far we've walked in deciding where to jump
      walkingback = 0;
      if (key & 8) // are we walking right too?
      {
        sprites[0].anim = RYU_FJUMP;
        sprites[0].anim_idx = 0;
        sprites[0].anim_dir = 1;
        sprites[0].anim_movement = &ryu_anim_fjump;
        sprites[0].anim_movement->anim_idx = 0;
      }
      else if (key & 4) // left jump?
      {
        sprites[0].anim = RYU_FJUMP;
        sprites[0].anim_idx = 0;
        sprites[0].anim_dir = 1;
        sprites[0].anim_movement = &ryu_anim_bjump;
        sprites[0].anim_movement->anim_idx = 0;
      }
      else
      {
        sprites[0].anim = RYU_JUMP;
        sprites[0].anim_idx = 0;
        sprites[0].anim_dir = 1;
        sprites[0].anim_movement = &ryu_anim_jump;
        sprites[0].anim_movement->anim_idx = 0;
      }
      //vely=-6 << 5;
    }
    if (key & 2 && !jumping && !walkingback && !walkingright)
    {
      crouching = 1;
      sprites[0].anim = RYU_CROUCHBLOCK;
      sprites[0].anim_idx = 1;
      sprites[0].anim_dir = 2;
    }
    if (key & 4 && !walkingback && !walkingright && !jumping) // left
    {
      walkingback = 1;
      sprites[0].posx -= 3;
      sprites[0].anim = RYU_WALKB;
      sprites[0].anim_idx = 0;
      sprites[0].anim_dir = 1;
      //dir = 1;
      //girlx -= 2;
      //if (!(i % 6)) // modulo operation seems to be time consuming !(i % 6))
      //  frame = (frame+1) % 2; // & 0x01; // % 2;
      //if (girlx < 25) girlx = 25;
    }
    if (key & 8 && !walkingback && !walkingright && !jumping) // right
    {
      walkingright = 1;
      sprites[0].anim = RYU_WALK;
      sprites[0].anim_idx = 0;
      sprites[0].anim_dir = 1;
      //dir = 0;
      //girlx += 2;

      //if (!(i & 0x03)) // don't use modulo operation !(i % 6))
      //  frame = (frame+1) % 2; //& 0x01; // % 2;
      //if (girlx > 323) girlx = 323;
    }
  } // end if

  //Poke(8192L, walkingright);
  //Poke(8193L, walkingback);
  //Poke(8194L, jumping);
}

// returns 1 = skip next animation step
// returns 0 = perform next animation step
unsigned char post_draw_processing(unsigned char sprite)
{
  if (sprite == 0)
  {
    // extra movement-related processing
    if (walkingright && sprites[0].anim_idx==7)
    {
      walkingright=0;
      sprites[0].posx += 3;
      sprites[0].anim = RYU_IDLE;
      sprites[0].anim_idx = 0;
      sprites[0].anim_dir = 1;
    }

    if (walkingback && sprites[0].anim_idx==7)
    {
      walkingback=0;
      sprites[0].anim = RYU_IDLE;
      sprites[0].anim_idx = 0;
      sprites[0].anim_dir = 1;
    }

    // start hadouken projectile?
    if (sprites[0].anim == RYU_HADOUKEN && sprites[0].anim_idx == 4)
    {
      sprites[1].anim=RYU_HADPROJ_START;
      sprites[1].anim_idx=0;
      sprites[1].visible=1;
      sprites[1].posx = sprites[0].posx + 9;
      sprites[1].posy = sprites[0].posy - 4;
    }

    if (punching)
    {
      if (anims[sprites[0].anim].pingpong)
      {
        if (sprites[0].anim_idx == 0 && sprites[0].anim_dir==0)
        {
          punching = 0;
          sprites[0].anim = RYU_IDLE;
          sprites[0].anim_idx = 0;
          sprites[0].anim_dir = 1;
          return 1;
        }
      }
      else if (sprites[0].anim_idx == anims[sprites[0].anim].frame_count-1)
      {
        punching = 0;
        sprites[0].anim = RYU_IDLE;
        sprites[0].anim_idx = 0;
        sprites[0].anim_dir = 1;
        return 1;
      }
    }

  } // end sprite0 processing

  // hadouken projectile sprite?
  if (sprite == 1)
  {
    if (sprites[1].anim == RYU_HADPROJ_START && sprites[1].anim_idx == 1)
    {
      sprites[1].anim = RYU_HADPROJ;
      sprites[1].anim_idx = 0;
    }
    else if (sprites[1].anim == RYU_HADPROJ && sprites[1].anim_idx == 11)
    {
      if (sprites[1].posx > 28)
      {
        sprites[1].anim=RYU_HADPROJ_END;
        sprites[1].anim_idx=0;
        sprites[1].posx += 3;
      }
      else
        sprites[1].posx += 3;
    }
    else if (sprites[1].anim == RYU_HADPROJ_END && sprites[1].anim_idx == 3)
    {
      sprites[1].visible=0;
    }
  }

  return 0;
}

void draw_bitmap(unsigned char frame, unsigned char posx, unsigned char posy)
{
  unsigned char num;
  reu_row_segment* seg;
  unsigned int len;
  unsigned long bmp_data_loc;

  //reu_segged_bmp_obj* segbmps = (reu_segged_bmp_obj*)0x1000;
  num = segbmps[frame].num_segments;

  screen_loc = vicbase+0x2000 + posx*8 + posy*40*8;
  bmp_data_loc = segbmps[frame].reu_ptr; // reu bank 1 contains bitmap data

  // copy segments metadata from reu (in bank0) to main memory (at 0x1800)
  if (num > 0)
  {
    reu_simple_copy(0x1800, 0x0000 + segbmps[frame].start_segment_idx*sizeof(reu_row_segment), segbmps[frame].num_segments*sizeof(reu_row_segment));

    seg = (reu_row_segment*)0x1800;

    for (gk = 0; gk < num; gk++)
    {
      len = seg->length << 3;
      screen_loc += seg->reloffset;

      reu_simple_copy(screen_loc, bmp_data_loc, len);

    //printf("%d - %d - %lu\n", seg->reloffset, screen_loc, bmp_data_loc);
    //if (k == 1)
    //{
    //while(1)
    //  ;
    //}

      bmp_data_loc += len;
      screen_loc += len;
      seg++;
    } // end for k

  }

  // draw repairs
  num_repairs = segbmps[frame].num_repairs;
  if (num_repairs > 0)
  {
    screen_loc = vicbase+0x2000 + posx*8 + posy*40*8;

    // copy across repair data to temp memory
    reu_simple_copy(0x1800, bmp_data_loc, num_repairs*18);
    repair = (reu_repair_obj*)0x1800;
    for (gk = 0; gk < num_repairs; gk++)
    {
      screen_loc += repair->reloffset;

      ptr = &(repair->vals[0]);
      //for (l = 0; l < 8; l++)
      {
        gtmp = Peek(screen_loc);
        a = *(ptr);
        ptr++;
        b = *(ptr);
        ptr++;
        gtmp &= b;
        gtmp |= (a & ~b);
        Poke(screen_loc, gtmp);
        screen_loc++;
        // --------------
        gtmp = Peek(screen_loc);
        a = *(ptr);
        ptr++;
        b = *(ptr);
        ptr++;
        gtmp &= b;
        gtmp |= (a & ~b);
        Poke(screen_loc, gtmp);
        screen_loc++;
        // --------------
        gtmp = Peek(screen_loc);
        a = *(ptr);
        ptr++;
        b = *(ptr);
        ptr++;
        gtmp &= b;
        gtmp |= (a & ~b);
        Poke(screen_loc, gtmp);
        screen_loc++;
        // --------------
        gtmp = Peek(screen_loc);
        a = *(ptr);
        ptr++;
        b = *(ptr);
        ptr++;
        gtmp &= b;
        gtmp |= (a & ~b);
        Poke(screen_loc, gtmp);
        screen_loc++;
        // --------------
        gtmp = Peek(screen_loc);
        a = *(ptr);
        ptr++;
        b = *(ptr);
        ptr++;
        gtmp &= b;
        gtmp |= (a & ~b);
        Poke(screen_loc, gtmp);
        screen_loc++;
        // --------------
        gtmp = Peek(screen_loc);
        a = *(ptr);
        ptr++;
        b = *(ptr);
        ptr++;
        gtmp &= b;
        gtmp |= (a & ~b);
        Poke(screen_loc, gtmp);
        screen_loc++;
        // --------------
        gtmp = Peek(screen_loc);
        a = *(ptr);
        ptr++;
        b = *(ptr);
        ptr++;
        gtmp &= b;
        gtmp |= (a & ~b);
        Poke(screen_loc, gtmp);
        screen_loc++;
        // --------------
        gtmp = Peek(screen_loc);
        a = *(ptr);
        ptr++;
        b = *(ptr);
        ptr++;
        gtmp &= b;
        gtmp |= (a & ~b);
        Poke(screen_loc, gtmp);
        screen_loc++;
        // --------------
      }
      repair++;
      screen_loc -= 8;
    } // end for
  } // end if
}

void calc_absolute_addresses(void)
{
  // TODO: early on, iterate through the bitmap meta objects to figure out the
  // missing address fields
  // Will probably need to refer to segdata too
}


enum { GAME_TITLE, GAME_MAIN };

unsigned char gamestate = GAME_MAIN;

void game_title(void)
{
  unsigned char key;

  //draw_anim_frame(TITLE, 0, 0, 25);
  draw_bitmap(TITLE, 0, 0);
  // TODO: draw title screen via new segmented bitmap technique...
  // ...need to locate segged_bitmap array at 0x1000
  // ...that are should match the content of the "bmp_meta.bin" file

  while(1)
  {
    // wait for fire-button
    key  = (~Peek(56320U)) & 31; //cgetc();

    if (!(key & 16) && firedown)  // test if fire was released
    {
      firedown=0;

      clear_screen();
      //draw_anim_frame(RYU_MUGSHOT, 1, 20, 15);
      //draw_anim_frame(RYU_STAGE_CROPPED, 0, 0, 25);

      vicbase=0x4000;
      draw_page = 1;

      gamestate = GAME_MAIN;
      return;
    }

    if (key != 0)
    {
      if (key & 16 && !firedown) // fire button
      {
        firedown=1;
      }
    }
  }
}

void game_main(void)
{
  unsigned int i, k;

  get_keyboard_input();
  
  // draw scenery first
  draw_bitmap(RYU_STAGE_CROPPED, 0, 0);

  for (i = 0; i < SPR_MAX; i++)
  {
    if (sprites[i].visible)
    {
  draw_bitmap(anims[sprites[i].anim].frames[sprites[i].anim_idx], sprites[i].posx, sprites[i].posy - anims[sprites[i].anim].rows);
  // reu_copy(0x2000 + posx*8 + (posy - anims[anim].rows)*40*8,
  //   anims[anim].reu_loc + frame * anims[anim].frame_size,
  //   anims[anim].cols*8, anims[anim].rows);

//      reu_copy(0x2000 + sprites[i].posx*8 + (sprites[i].posy - anims[sprites[i].anim].rows)*40*8,
//        anims[sprites[i].anim].reu_loc + sprites[i].anim_idx * anims[sprites[i].anim].frame_size,
//        anims[sprites[i].anim].cols*8, anims[sprites[i].anim].rows);

      if (post_draw_processing(i) == 0)
      {
        animate_sprite(&(sprites[i]));
      }
    }
  }

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

  //if (anims[sprites[i].anim].frames[sprites[i].anim_idx]== RYU_IDLE2)
  //  exit(0);

  // add a delay
  //for (k=0; k < 500; k++)
  //  ;
}

void main(void)
{
  unsigned int i, j, seg_idx;
  unsigned long loc;
  reu_row_segment* seg;

  Poke(53280L,0);

  // Select Bank1 for bitmap pages
  //Poke(56578, Peek(56578) | 3); // make sure CIA bits 0+1 are set to outputs (DDR)
  //Poke(56576, (Peek(56576) & 252) | 2); // change to vic bank1 ($4000-$7fff)

  // Put bitmap at 8192
  Poke(53272L, Peek(53272L) | 8);

  // Enter bitmap mode
  Poke(53265L, Peek(53265L) | 32);

  // Try a REU ram transfer
  Poke(REC_ADDR_CTRL, 0); // make sure both addresses are counted up

  clear_screen();

  // size is 7 x 14 char-blocks
  // = 784 bytes = 0x0310
  // draw it one row at a time

  loc = 0x00010000;
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

    // copy segments metadata from reu (in bank0) to main memory (at 0x1800)
    if (segbmps[i].num_segments > 0)
    {
      reu_simple_copy(0x1800, 0x0000 + seg_idx*sizeof(reu_row_segment), segbmps[i].num_segments*sizeof(reu_row_segment));

      seg = (reu_row_segment*)0x1800;
      for (j = 0; j < segbmps[i].num_segments; j++)
      {
        loc += (seg[j].length << 3);
      }
      seg_idx += segbmps[i].num_segments; // get this ready for the next segged bmp
    }
    loc += segbmps[i].num_repairs * 18; // 18 = sizeof(reu_repair_obj)
  }

  while(1)
  {
    switch(gamestate)
    {
      case GAME_TITLE:
        game_title();
        break;

      case GAME_MAIN:
        game_main();
        break;
    }
  } // end while
}
