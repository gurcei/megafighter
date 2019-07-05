#include <stdio.h>

#define Poke(A,X)  (*(unsigned char *)(A)) = (X)
#define Peek(A)    (*(unsigned char *)(A))
#define PokeW(A,X) (*(unsigned int *)(A)) = (X)
#define PeekW(A)   (*(unsigned int *)(A))

// ================================

#define RYU_IDLE  0
#define RYU_WALK  1
#define RYU_WALKB 2
#define RYU_JUMP  3
#define RYU_FJUMP 4
#define RYU_CROUCHBLOCK 5
#define RYU_LPUNCH  6
#define RYU_MHPUNCH 7
#define RYU_FLPUNCH 8
#define RYU_FMPUNCH 9
#define RYU_FHPUNCH 10
#define RYU_LMKICK  11
#define RYU_HKICK   12
#define RYU_FLKICK  13
#define RYU_FMKICK  14
#define RYU_FHKICK  15
#define RYU_CROUCH_LPUNCH 16
#define RYU_CROUCH_MPUNCH 17
#define RYU_MAX     18

unsigned char punch_style = RYU_LMKICK;

typedef struct
{
  unsigned long reu_loc;
  unsigned int frame_size;  // in bytes
  unsigned char frame_count;
  unsigned char pingpong;
  unsigned int  cols;
  unsigned char rows;
} anim_detail;

anim_detail anims[RYU_MAX] =
{
  // let the first two fields be auto-generated by the details in the other fields
  // (reu_loc and frame_size)
  { 0, 0,  3, 1, 7,  14 }, // RYU_IDLE
  { 0, 0,  8, 0, 8,  14 }, // RYU_WALK
  { 0, 0,  8, 0, 8,  14 }, // RYU_WALKB
  { 0, 0,  4, 1, 7,  14 }, // RYU_JUMP
  { 0, 0,  7, 0, 10, 14 }, // RYU_FJUMP
  { 0, 0,  4, 0, 7, 14 },  // RYU_CROUCHBLOCK
  { 0, 0,  3, 0, 8, 15 },  // RYU_LPUNCH
  { 0, 0,  3, 1, 10, 15 }, // RYU_MHPUNCH
  { 0, 0,  3, 1, 7, 15 },  // RYU_FLPUNCH
  { 0, 0,  4, 1, 8, 15 },  // RYU_FMPUNCH
  { 0, 0,  4, 1, 8, 15 },  // RYU_FHPUNCH
  { 0, 0,  2, 1, 9, 17 },  // RYU_LMKICK
  { 0, 0,  5, 0, 10, 17 }, // RYU_HKICK
  { 0, 0,  2, 1, 10, 17 }, // RYU_FLKICK
  { 0, 0,  3, 1, 8, 17 },  // RYU_FMKICK
  { 0, 0,  3, 1, 11, 17 }, // RYU_FHKICK
  { 0, 0,  2, 1, 9, 16 },  // RYU_CROUCH_LPUNCH
  { 0, 0,  2, 1, 9, 16 },  // RYU_CROUCH_MPUNCH
};

typedef struct
{
  unsigned char posx, posy;
  unsigned char anim;
  unsigned char anim_idx;
  unsigned char anim_dir;
} sprite_detail;

#define SPR_MAX 1

sprite_detail sprites[SPR_MAX] =
{
/*  { 0,  10, RYU_LPUNCH, 0, 1 },
  { 7, 10, RYU_MHPUNCH, 0, 1 },
  { 17, 10, RYU_FLPUNCH, 0, 1 },
  { 24, 10, RYU_FMPUNCH, 0, 1 },
  { 32, 10, RYU_FHPUNCH, 0, 1 }, */
  { 4,  22,  RYU_IDLE, 0, 1 },
/*  { 7,  0,  RYU_WALK, 0, 1 },
  { 14, 0,  RYU_JUMP, 0, 1 },
  { 21, 0,  RYU_FJUMP, 0, 1 },
  { 31, 0,  RYU_CROUCHBLOCK, 0, 1 }, */
};

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

void get_keyboard_input(void)
{
  unsigned char key;

  // JOYSTICK: left=4, right=8, up=1, down=2, fire=16
  key  = (~Peek(56320U)) & 31; //cgetc();

  if (!(key & 16) && firedown)  // test if fire was released
  {
    firedown=0;
    sprites[0].anim = RYU_IDLE;
    sprites[0].anim_idx = 0;
    sprites[0].anim_dir = 1;
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
      sprites[0].anim = punch_style;
      sprites[0].anim_idx = 0;
      sprites[0].anim_dir = 1;
      punch_style++;
      if (punch_style == RYU_MAX)
        punch_style = RYU_LMKICK;
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

void post_draw_processing(unsigned char sprite)
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
  }
}

void main(void)
{
  unsigned int base, i, k;
  unsigned long loc;

  base=2*4096;

  Poke(53280L,0);

  // Put bitmap at 8192
  Poke(53272L, Peek(53272L) | 8);

  // Enter bitmap mode
  Poke(53265L, Peek(53265L) | 32);

  // Set bitmap colours
  for (i = 1024; i < 2024; i++)
  {
    Poke(i, (0 << 4) + 1);  // black and white
  }

  // Clear bitmap
  for (i = base; i < base+8000; i++)
  {
    Poke(i, 0x00);
  }

  // Try a REU ram transfer
  Poke(REC_ADDR_CTRL, 0); // make sure both addresses are counted up

  // size is 7 x 14 char-blocks
  // = 784 bytes = 0x0310
  // draw it one row at a time

  loc = 0x0000;
  for (i = 0; i < RYU_MAX; i++)
  {
    anims[i].reu_loc = loc;
    anims[i].frame_size = anims[i].cols * 8 * anims[i].rows;
    loc += anims[i].frame_size * anims[i].frame_count;
  }

  while(1)
  {
    get_keyboard_input();
    for (i = 0; i < SPR_MAX; i++)
    {
      reu_copy(0x2000 + sprites[i].posx*8 + (sprites[i].posy - anims[sprites[i].anim].rows)*40*8,
        anims[sprites[i].anim].reu_loc + sprites[i].anim_idx * anims[sprites[i].anim].frame_size,
        anims[sprites[i].anim].cols*8, anims[sprites[i].anim].rows);

      post_draw_processing(i);

      animate_sprite(&(sprites[i]));
    }

    // add a delay
    for (k=0; k < 1000; k++)
      ;
  } // end while
}
