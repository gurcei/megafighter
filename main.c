#include <stdio.h>

#define Poke(A,X)  (*(unsigned char *)(A)) = (X)
#define Peek(A)    (*(unsigned char *)(A))
#define PokeW(A,X) (*(unsigned int *)(A)) = (X)
#define PeekW(A)   (*(unsigned int *)(A))

// ================================

#define RYU_IDLE  0
#define RYU_WALK  1
#define RYU_JUMP  2
#define RYU_FJUMP 3
#define RYU_MAX   4

typedef struct
{
  unsigned int reu_loc;
  unsigned int frame_size;  // in bytes
  unsigned char frame_count;
  unsigned char pingpong;
  unsigned int  rowsize;
  unsigned char rows;
} anim_detail;

anim_detail anims[RYU_MAX] =
{
  { 0x0000, 784,  3, 1, 7*8,  14 }, // RYU_IDLE
  { 0x0930, 784,  5, 1, 7*8,  14 }, // RYU_WALK
  { 0x1880, 784,  4, 1, 7*8,  14 }, // RYU_JUMP
  { 0x24C0, 1120, 7, 0, 10*8, 14 }  // RYU_FJUMP
};

typedef struct
{
  unsigned char posx, posy;
  unsigned char anim;
  unsigned char anim_idx;
  unsigned char anim_dir;
} sprite_detail;

#define SPR_MAX 4

sprite_detail sprites[SPR_MAX] =
{
  { 0, 0, RYU_IDLE, 0, 1 },
  { 7, 0, RYU_WALK, 0, 1 },
  { 14, 0, RYU_JUMP, 0, 1 },
  { 21, 0, RYU_FJUMP, 0, 1 },
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

void reu_copy(int c64loc, int reuloc, int rowsize, unsigned char rows)
{
  unsigned char y;
  for (y = 0; y < rows; y++)
  {
    // c64base = 8192 = 0x2000
    Poke(REC_C64_ADDR_LO, c64loc & 0xff);
    Poke(REC_C64_ADDR_HI, c64loc >> 8);
    Poke(REC_REU_ADDR_LO, reuloc & 0xff);
    Poke(REC_REU_ADDR_HI, reuloc >> 8);
    Poke(REC_REU_ADDR_BANK, 0x00);
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
  if (spr->anim_dir)
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
  else
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


void main(void)
{
  unsigned int base, i, k;
  int rowsize, c64loc, reuloc;
  int idle_loc, walk_loc;
  unsigned char y;
  unsigned char anim_idx, anim_dir;
  unsigned char walk_anim_idx, walk_anim_dir;
  //printf("hello world\n");

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
  rowsize = 7 * 8;
  // draw it one row at a time
  anim_dir = 1;
  anim_idx = 0;

  walk_anim_dir = 1;
  walk_anim_idx = 0;

  idle_loc = 0x0000;
  walk_loc = idle_loc + 784*3;
  while(1)
  {
    for (i = 0; i < SPR_MAX; i++)
    {
      reu_copy(0x2000 + sprites[i].posx*8 + sprites[i].posy*40*8,
        anims[sprites[i].anim].reu_loc + sprites[i].anim_idx * anims[sprites[i].anim].frame_size,
        anims[sprites[i].anim].rowsize, anims[sprites[i].anim].rows);

      animate_sprite(&(sprites[i]));
    }
    //reu_copy(0x2000, anims[RYU_IDLE].reuloc + anim_idx * 784, rowsize, 14);

    //reu_copy(0x2000 + 8*15, walk_loc + walk_anim_idx * 784, rowsize, 14);

    // rotate animation
    //rotate_anim(&anim_dir, &anim_idx, 2);
    //rotate_anim(&walk_anim_dir, &walk_anim_idx, 4);

    for (k=0; k < 1000; k++)
      ;
  } // end while
}
