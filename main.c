#include <stdio.h>
#include "bitmap_ids.h"

#define Poke(A,X)  (*(unsigned char *)(A)) = (X)
#define Peek(A)    (*(unsigned char *)(A))
#define PokeW(A,X) (*(unsigned int *)(A)) = (X)
#define PeekW(A)   (*(unsigned int *)(A))

// ================================

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

#define FIRST_ATTACK RYU_SHOULDERTOSS // RYU_HADOUKEN // RYU_JUMP_LMHPUNCH

unsigned char punch_style = FIRST_ATTACK;

unsigned int anim_ryu_idle[] = { RYU_IDLE2, RYU_IDLE3, RYU_IDLE4 };
unsigned int anim_ryu_walk[] = { RYU_WALK1, RYU_WALK2, RYU_WALK3, RYU_WALK4, RYU_WALK5, RYU_WALK6, RYU_WALK7, RYU_WALK8 };
unsigned int anim_ryu_walkb[] = { RYU_WALK8, RYU_WALK7, RYU_WALK6, RYU_WALK5, RYU_WALK4, RYU_WALK3, RYU_WALK2, RYU_WALK1 };

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
  { anim_ryu_idle,  3, 1, 7,  14 }, // RYU_IDLE
  { anim_ryu_walk,  8, 0, 8,  14 }, // RYU_WALK
  { anim_ryu_walkb,  8, 0, 8,  14 }, // RYU_WALKB
  { 0,  4, 1, 7,  14 }, // RYU_JUMP
  { 0,  7, 0, 10, 14 }, // RYU_FJUMP
  { 0,  4, 0, 7, 14 },  // RYU_CROUCHBLOCK
  { 0,  3, 0, 8, 15 },  // RYU_LPUNCH
  { 0,  3, 1, 10, 15 }, // RYU_MHPUNCH
  { 0,  3, 1, 7, 15 },  // RYU_FLPUNCH
  { 0,  4, 1, 8, 15 },  // RYU_FMPUNCH
  { 0,  4, 1, 8, 15 },  // RYU_FHPUNCH
  { 0,  2, 1, 9, 17 },  // RYU_LMKICK
  { 0,  5, 0, 10, 17 }, // RYU_HKICK
  { 0,  2, 1, 10, 17 }, // RYU_FLKICK
  { 0,  3, 1, 8, 17 },  // RYU_FMKICK
  { 0,  3, 1, 11, 17 }, // RYU_FHKICK
  { 0,  2, 1, 9, 16 },  // RYU_CROUCH_LPUNCH
  { 0,  2, 1, 9, 16 },  // RYU_CROUCH_MPUNCH
  { 0,  3, 1, 7, 16 },  // RYU_CROUCH_HPUNCH
  { 0,  2, 1, 10, 16 }, // RYU_CROUCH_LKICK
  { 0,  2, 1, 12, 16 }, // RYU_CROUCH_MKICK
  { 0,  5, 0, 10, 16 }, // RYU_CROUCH_HKICK
  { 0,  3, 0, 7,  18 }, // RYU_JUMP_LMHPUNCH
  { 0,  2, 0, 7,  18 }, // RYU_JUMP_LMKICK
  { 0,  3, 0, 8,  18 }, // RYU_JUMP_HKICK
  { 0,  2, 0, 6,  18 }, // RYU_FJUMP_LPUNCH
  { 0,  3, 0, 9,  18 }, // RYU_FJUMP_MHKICK
  { 0,  7, 0, 7,  18 }, // RYU_SHOURYUKEN
  { 0,  9, 0, 12, 18 }, // RYU_TATSUMAKI
  { 0,  5, 0, 12, 13 }, // RYU_HADOUKEN
  { 0,  2, 0, 6,  8  }, // RYU_HADPROJ_START
  { 0,  12, 0, 9, 8  }, // RYU_HADPROJ
  { 0,  4, 0, 6,  8  }, // RYU_HADPROJ_END
  { 0,  5, 0, 9,  13 }, // RYU_SHOULDERTOSS
  { 0,  7, 0, 10, 13 }, // RYU_BACKROLL
  { 0,  4, 0, 7,  15 }, // RYU_HIT
  { 0,  4, 0, 8,  15 }, // RYU_FACEHIT
  { 0,  3, 0, 9,  15 }, // RYU_CROUCHHIT
  { 0,  8, 0, 10, 15 }, // RYU_KNOCKDOWN
  { 0,  3, 1, 8,  15 }, // RYU_STUNNED
  { 0,  5, 0, 10, 15 }, // RYU_KO
  { 0,  3, 0, 7,  16 }, // RYU_VICTORY
  { 0,  7, 1, 7,  16 }, // RYU_VICTORYALT
  { 0,  3, 0, 11, 16 }, // RYU_MUGSHOT
};

typedef struct
{
  unsigned char posx, posy;
  unsigned char anim;
  unsigned char anim_idx;
  unsigned char anim_dir;
  unsigned char visible;
} sprite_detail;

#define SPR_MAX 2

sprite_detail sprites[SPR_MAX] =
{
  { 4,  22,  RYU_IDLE, 0, 1, 1 },
  { 10, 15,  RYU_HADPROJ_START, 0, 1, 0 },

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
  unsigned long reu_ptr;  // pointer to reu-memory for the data for the first row-segment (for the next segment, it'll be adjecent to this one in reu memory)
												// I might have to leave this last field empty (relocatable?) and fill it in at run-time, depending on the ordering of all the objects
} reu_segged_bmp_obj;

reu_segged_bmp_obj* segbmps = (reu_segged_bmp_obj*)0x1000;

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
 
unsigned int base = 2*4096;

void clear_screen(void)
{
  unsigned int i;
  // Clear bitmap
  for (i = base; i < base+8000; i++)
  {
    Poke(i, 0x00);
  }

  // Set bitmap colours
  for (i = 1024; i < 2024; i++)
  {
    Poke(i, (0 << 4) + 1);  // black and white
  }
}

void animate_sprite(sprite_detail* spr)
{
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

unsigned char firedown=0;
unsigned char jumping=0;
unsigned char walkingright=0;
unsigned char walkingback=0;
unsigned char crouching=0;
unsigned char punching=0;

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
      sprites[0].anim = RYU_JUMP;
      sprites[0].anim_idx = 0;
      sprites[0].anim_dir = 1;
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
      sprites[0].posx -= 2;
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

  Poke(8192L, walkingright);
  Poke(8193L, walkingback);
  Poke(8194L, jumping);
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
      sprites[0].posx += 2;
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

    if (jumping && sprites[0].anim_idx==0)
    {
      if (jumping == 1)
      {
        jumping++;
      }
      else
      {
        jumping=0;
        sprites[0].anim = RYU_IDLE;
        sprites[0].anim_idx = 0;
        sprites[0].anim_dir = 1;
      }
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
  unsigned char k, num;
  reu_row_segment* seg;
  unsigned int screen_loc, len;
  unsigned long bmp_data_loc;

  //reu_segged_bmp_obj* segbmps = (reu_segged_bmp_obj*)0x1000;
  num = segbmps[frame].num_segments;

  // copy segments metadata from reu (in bank0) to main memory (at 0x1800)
  reu_simple_copy(0x1800, 0x0000 + segbmps[frame].start_segment_idx*sizeof(reu_row_segment), segbmps[frame].num_segments*sizeof(reu_row_segment));

  seg = (reu_row_segment*)0x1800;

  screen_loc = 0x2000 + posx*8 + posy*40*8;
  bmp_data_loc = segbmps[frame].reu_ptr; // reu bank 1 contains bitmap data
  for (k = 0; k < num; k++)
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
  }
}

void draw_anim_frame(unsigned char anim, unsigned char frame, unsigned char posx, unsigned char posy)
{
  draw_bitmap(anims[anim].frames[frame], 0, 0);
  // reu_copy(0x2000 + posx*8 + (posy - anims[anim].rows)*40*8,
  //   anims[anim].reu_loc + frame * anims[anim].frame_size,
  //   anims[anim].cols*8, anims[anim].rows);
}

void calc_absolute_addresses(void)
{
  // TODO: early on, iterate through the bitmap meta objects to figure out the
  // missing address fields
  // Will probably need to refer to segdata too
}


enum { GAME_TITLE, GAME_MAIN };

unsigned char gamestate = GAME_TITLE;

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
      draw_bitmap(RYU_STAGE_CROPPED, 0, 0);
      //draw_anim_frame(RYU_STAGE_CROPPED, 0, 0, 25);

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
  for (i = 0; i < SPR_MAX; i++)
  {
    if (sprites[i].visible)
    {
  draw_bitmap(anims[sprites[i].anim].frames[sprites[i].anim_idx], 0, 0);
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

  // add a delay
  for (k=0; k < 1000; k++)
    ;
}

void main(void)
{
  unsigned int i, j, seg_idx;
  unsigned long loc;
  reu_row_segment* seg;

  Poke(53280L,0);

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

    // copy segments metadata from reu (in bank0) to main memory (at 0x1800)
    reu_simple_copy(0x1800, 0x0000 + seg_idx*sizeof(reu_row_segment), segbmps[i].num_segments*sizeof(reu_row_segment));
    seg = (reu_row_segment*)0x1800;
    for (j = 0; j < segbmps[i].num_segments; j++)
    {
      loc += (seg[j].length << 3);
    }
    seg_idx += segbmps[i].num_segments; // get this ready for the next segged bmp
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
