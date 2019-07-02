#include <stdio.h>

#define Poke(A,X)  (*(unsigned char *)(A)) = (X)
#define Peek(A)    (*(unsigned char *)(A))
#define PokeW(A,X) (*(unsigned int *)(A)) = (X)
#define PeekW(A)   (*(unsigned int *)(A))

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

void main(void)
{
  unsigned int base, i, k;
  int rowsize, c64loc, reuloc;
  unsigned char y;
  //printf("hello world\n");

  base=2*4096;

  Poke(53280L,1);

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
  c64loc = 0x2000;
  reuloc = 0x0000;
  for (y = 0; y < 14; y++)
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
