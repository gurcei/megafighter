/*
 * Copyright 2002-2010 Guillaume Cottenceau.
 * Copyright 2015-2016 Paul Gardner-Stephen.
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */

/* ============================================================= */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define PNG_DEBUG 3
#include <png.h>

/* ============================================================= */

int x, y;
char* orig_fname;

int width, height;
int multiplier=-1;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * row_pointers;

FILE *infile;
FILE *outfile;

int segment = 25;
int offset = 0;

/* ============================================================= */

void abort_(const char * s, ...)
{
  va_list args;
  va_start(args, s);
  vfprintf(stderr, s, args);
  fprintf(stderr, "\n");
  va_end(args);
  abort();
}

/* ============================================================= */

void read_png_file(char* file_name)
{
  unsigned char header[8];    // 8 is the maximum size that can be checked

  /* open file and test for it being a png */
  infile = fopen(file_name, "rb");
  if (infile == NULL)
    abort_("[read_png_file] File %s could not be opened for reading", file_name);

  fread(header, 1, 8, infile);
  if (png_sig_cmp(header, 0, 8))
    abort_("[read_png_file] File %s is not recognized as a PNG file", file_name);

  /* initialize stuff */
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr)
    abort_("[read_png_file] png_create_read_struct failed");

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    abort_("[read_png_file] png_create_info_struct failed");

  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[read_png_file] Error during init_io");

  png_init_io(png_ptr, infile);
  png_set_sig_bytes(png_ptr, 8);

  // Convert palette to RGB values
  png_set_expand(png_ptr);

  png_read_info(png_ptr, info_ptr);

  width = png_get_image_width(png_ptr, info_ptr);
  height = png_get_image_height(png_ptr, info_ptr);
  color_type = png_get_color_type(png_ptr, info_ptr);
  bit_depth = png_get_bit_depth(png_ptr, info_ptr);

  printf("Input-file is: width=%d, height=%d.\n", width, height);

  number_of_passes = png_set_interlace_handling(png_ptr);
  png_read_update_info(png_ptr, info_ptr);

  /* read file */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[read_png_file] Error during read_image");

  row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
  for (y=0; y<height; y++)
    row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

  png_read_image(png_ptr, row_pointers);

  if (infile != NULL) {
    fclose(infile);
    infile = NULL;
  }

  printf("Input-file is read and now closed\n");
}

/* ============================================================= */

#pragma pack(push)
#pragma pack(1)

typedef struct
{
  unsigned short reloffset;    // this is the byte-offset starting from the top-left corner of the object, but will increment per row_segment
  unsigned char length;    // the number of length in chars for this row segment
} reu_row_segment;

typedef struct
{
  unsigned char num_segments;
  unsigned short start_segment_idx;   // NOTE: reu_loc for segment metadata = start_segment_idx * sizeof(reu_row_segment)
  unsigned char num_repairs;  // how many repair chars are needed for this bitmap
              // NOTE: this might have to be a relative value, and I make it absolute at run-time
  unsigned int reu_ptr;  // pointer to reu-memory for the data for the first row-segment (for the next segment, it'll be adjecent to this one in reu memory)
												// I might have to leave this last field empty (relocatable?) and fill it in at run-time, depending on the ordering of all the objects
} reu_segged_bmp_obj;

typedef struct
{
  unsigned short reloffset;    // this is the byte-offset starting from the top-left corner of the object, but will increment per row_segment
  unsigned char vals[16];     // 0=bmpval0, 1=maskval0, 2=bmpval1, 3=maskval1, etc...
} reu_repair_obj;

#define MAX_SEGS 1000

unsigned short seg_cnt = 0;
unsigned char repair_cnt = 0;
reu_row_segment rowsegs[MAX_SEGS] = { 0 };
reu_repair_obj repairs[MAX_SEGS] = { 0 };
reu_segged_bmp_obj seggedbmp = { 0 };

#pragma pack(pop)

// writing of png example taken from:
// http://www.labbookpages.co.uk/software/imgProc/libPNG.html#write

void setRGB(png_byte *ptr, float val)
{
	int v = (int)(val * 767);
	if (v < 0) v = 0;
	if (v > 767) v = 767;
	int offset = v % 256;

	if (v<256) {
		ptr[0] = 0; ptr[1] = 0; ptr[2] = offset;
	}
	else if (v<512) {
		ptr[0] = 0; ptr[1] = offset; ptr[2] = 255-offset;
	}
	else {
		ptr[0] = offset; ptr[1] = 255-offset; ptr[2] = 0;
	}
}

int writeImage(char* filename, int width, int height, png_bytep *new_row_pointers, char* title)
{
   int code = 0;
   FILE *fp = NULL;
   png_structp png_ptr = NULL;
   png_infop info_ptr = NULL;
   png_bytep row = NULL;

   // Open file for writing (binary mode)
   fp = fopen(filename, "wb");
   if (fp == NULL) {
      fprintf(stderr, "Could not open file %s for writing\n", filename);
      code = 1;
      goto finalise;
   }

   // Initialize write structure
   png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   if (png_ptr == NULL) {
      fprintf(stderr, "Could not allocate write struct\n");
      code = 1;
      goto finalise;
   }

   // Initialize info structure
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL) {
      fprintf(stderr, "Could not allocate info struct\n");
      code = 1;
      goto finalise;
   }

   // Setup Exception handling
   if (setjmp(png_jmpbuf(png_ptr))) {
      fprintf(stderr, "Error during png creation\n");
      code = 1;
      goto finalise;
   }

   png_init_io(png_ptr, fp);

   // Write header (8 bit colour depth)
   png_set_IHDR(png_ptr, info_ptr, width, height,
         8, multiplier==3 ? PNG_COLOR_TYPE_RGB : PNG_COLOR_TYPE_RGBA,
         PNG_INTERLACE_NONE,
         PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

   // Set title
   if (title != NULL) {
      png_text title_text;
      title_text.compression = PNG_TEXT_COMPRESSION_NONE;
      title_text.key = "Title";
      title_text.text = title;
      png_set_text(png_ptr, info_ptr, &title_text, 1);
   }

   png_write_info(png_ptr, info_ptr);

   // Allocate memory for one row (3 bytes per pixel - RGB)
   //row = (png_bytep) malloc(3 * width * sizeof(png_byte));

   // Write image data
   int x, y;
   for (y=0 ; y<height ; y++) {
	 	row = new_row_pointers[y];
      png_write_row(png_ptr, row);
   }

   // End write
   png_write_end(png_ptr, NULL);

   finalise:
   if (fp != NULL) fclose(fp);
   if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
   if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);

   return code;
}

void process_file(int mode, char *outputfilename)
{
  if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB)
    multiplier=3;

  if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGBA)
    multiplier=4;

  if (multiplier==-1) {
    fprintf(stderr,"Could not convert file to RGB or RGBA\n");
  }

  outfile=fopen(outputfilename,"w");
  if (outfile == NULL) {
    // could not open output file, so close all and exit
    if (infile != NULL) {
      fclose(infile);
      infile = NULL;
    }
    abort_("[process_file] File %s could not be opened for writing", outputfilename);
  }


  /* ============================ */

  if (mode==0) {
    printf("mode=0 (logo)\n");
    // Logo mode

    if (height!=64||width!=64) {
      fprintf(stderr,"Logo images must be 64x64\n");
    }
    for (y=0; y<height; y++) {
      png_byte* row = row_pointers[y];
      for (x=0; x<width; x++) {
	png_byte* ptr = &(row[x*multiplier]);
	int r=ptr[0],g=ptr[1],b=ptr[2]; // a=ptr[3];

	// Compute colour cube colour
	unsigned char c=(r&0xe0)|((g>>5)<<2)|(b>>6);

	/* work out where in logo file it must be written.
	   image is made of 8x8 blocks.  So every 8 pixels across increases address
	   by 64, and every 8 pixels down increases pixel count by (64*8), and every
	   single pixel down increases address by 8.
	*/
	int address=(x&7)+(y&7)*8;
	address+=(x>>3)*64;
	address+=(y>>3)*64*8;
	fseek(outfile,address,SEEK_SET);
	int n=fwrite(&c,1,1,outfile);
	if (n!=1) {
	  fprintf(stderr,"Could not write pixel (%d,%d) @ $%x\n",x,y,address);
          if (outfile != NULL) {
            fclose(outfile);
            outfile = NULL;
          }
	  exit(-1);
	}
      }
    }

    if (outfile != NULL) {
      fclose(outfile);
      outfile = NULL;
    }

  }

  /* ============================ */
  if (mode==1) {
    printf("mode=1 (charrom)\n");
    // charrom mode

    int bytes=0;
    if (width!=8) {
      fprintf(stderr,"Fonts must be 8 pixels wide\n");
    }

    int spots[8][8];

    for (y=0; y<height; y++) {
      png_byte* row = row_pointers[y];
      int byte=0;
      int yy=y&7;

      for (x=0; x<width; x++) {
	png_byte* ptr = &(row[x*multiplier]);
	int r=ptr[0]; // g=ptr[1],b=ptr[2], a=ptr[3];

	if (x<8) {
	  if (r>0x7f) {
	    byte|=(1<<(7-x));
	    spots[yy][x]=1;
	  } else spots[yy][x]=0;
	}
      }
      fflush(stdout);
      char comma = ',';
      if (y==height-1) comma=' ';
      fprintf(outfile,"%c",byte);
      bytes++;
    }
    // Fill in any missing bytes
    if (offset != 1) // disable padding
    {
      if (bytes<2048) {

        printf("Padding output file to 2048 bytes\n");

        for(;bytes<2048;bytes++) fprintf(outfile,"%c",0);
      }
    }

    if (outfile != NULL) {
      fclose(outfile);
      outfile = NULL;
    }
  }

  /* ============================ */
  if (mode==2) {
    printf("mode=2 (hi-res prep)\n");
    // hi-res image preparation mode

    // int bytes=0;
    if (width%8||height%8) {
      fprintf(stderr,"Image must be multiple of 8 pixels wide and high\n");
    }
    int problems=0;
    int total=0;
    int threes=0;
    int fours=0;
    int ones=0;

    int tiles[8000][8][8];
    int tile_count=0;

    int this_tile[8][8];

    for (y=0; y<height; y+=8) {
      for (x=0; x<width; x+=8) {
	int yy,xx;
	int i;
	int colour_count=0;
	int colours[64];

	printf("[%d,%d]\n",x,y);

	total++;

	for(yy=y;yy<y+8;yy++) {
	  png_byte* row = row_pointers[yy];
	  for(xx=x;xx<x+8;xx++) {
	    png_byte* ptr = &(row[xx*multiplier]);
	    int r=ptr[0], g=ptr[1],b=ptr[2]; // , a=ptr[3];
	    int c=r+256*g+65536*b;
	    this_tile[yy-y][xx-x]=c;
	    for(i=0;i<colour_count;i++) if (c==colours[i]) break;
	    if (i==colour_count) {
	      colours[colour_count++]=c;
	    }
	  }
	}

	for(i=0;i<tile_count;i++) {
	  int dud=0;
	  int xx,yy;
	  for(xx=0;xx<8;xx++)
	    for(yy=0;yy<8;yy++) {
	      if (this_tile[yy][xx]!=tiles[i][yy][xx]) dud=1;
	    }
	  if (!dud) break;
	}
	if (i==tile_count) {
	  int xx,yy;
	  for(xx=0;xx<8;xx++)
	    for(yy=0;yy<8;yy++) {
	      tiles[tile_count][yy][xx]=this_tile[yy][xx];
	    }
	  printf(".[%d]",tile_count); fflush(stdout);
	  tile_count++;
	  if (tile_count>=8000) {
	    fprintf(stderr,"Too many tiles\n");
            if (outfile != NULL) {
              fclose(outfile);
              outfile = NULL;
            }
	    exit(-1);
	  }
	}

	if (colour_count==1) ones++;
	if (colour_count==3) threes++;
	if (colour_count==4) fours++;
	if (colour_count>2) {
	  printf("%d colours in card\n",colour_count);
	  problems++;
	}
      }
    }
    printf("%d problem tiles out of %d total tiles\n",problems,total);
    printf("%d with 3, %d with 4, %d with only one colour\n",threes,fours,ones);
    printf("%d unique tiles\n",tile_count);
  }

  /* ============================ */
  if (mode==3) {
    printf("mode=3 (4-sprite multi-colour)\n");
    // 4-sprite multi-colour mode

    if (width!=24) {
      fprintf(stderr,"png must be 24 pixels wide\n");
    }

    if (height!= 42) {
      fprintf(stderr, "png must be 42 pixels high\n");
    }

    unsigned char sprdata[42][6] = { 0 };
    unsigned char outlinedata[42][6] = { 0 };

    for (y=0; y<height; y++) {
      png_byte* row = row_pointers[y];
      
      for (x=0; x<width; x++) {
        png_byte* ptr = &(row[x*multiplier]);
        int r=(ptr[0] + ptr[1] + ptr[2]) / 3;
        unsigned char bits;

#define DITHER(a, b) \
  if ((x%2) && (y%2)) bits=a; \
  if ((x%2) && !(y%2)) bits=b; \
  if (!(x%2) && (y%2)) bits=b; \
  if (!(x%2) && !(y%2)) bits=a;

        // black
        if (r < (offset+segment))
          bits=4;
        // black+grey1 dither
        else if (r < (offset+segment*2)) {
          DITHER(4,3)
        }
        // grey 1
        else if (r < (offset+segment*3))
          bits=3;
        // grey1+grey2 dither
        else if (r < (offset+segment*4)) {
          DITHER(3,2)
        }
        // grey 2
        else if (r < (offset+segment*5))
          bits=2;
        // grey2+grey3 dither
        else if (r < (offset+segment*6)) {
          DITHER(2,1)
        }
        // grey 3
        else if (r < (offset+segment*7))
          bits=1;
        // grey3+white dither
        else if (r < (offset+segment*8)) {
          DITHER(1,0)
        }
        // white
        else
          bits=0;

        // inject bits into sprite bitstream
        if (bits==4)
          outlinedata[y][x/4] = outlinedata[y][x/4] | (3 << (2*(3-(x%4))));
        else
          sprdata[y][x/4] = ( sprdata[y][x/4] & ( ~((unsigned char)3 << (2*(3-x%4))) ) ) + (bits << (2*(3-x%4)));
      } //end for x
    } //end for y

    // write outline to file
    for (int spr=0; spr<4; spr++)
    {
      int offsx=0, offsy=0;

      if (spr==1 || spr==3) offsx=3;
      if (spr==2 || spr==3) offsy=21;
      for (y = 0; y<21; y++) {
        for (x = 0; x<3; x++)
          fprintf(outfile,"%c", outlinedata[offsy+y][offsx+x]);
      }
      fprintf(outfile, "%c", 0);  // extra 64th byte at end of sprite
    }

    // write multicolours to file
    for (int spr=0; spr<4; spr++)
    {
      int offsx=0, offsy=0;

      if (spr==1 || spr==3) offsx=3;
      if (spr==2 || spr==3) offsy=21;
      for (y = 0; y<21; y++) {
        for (x = 0; x<3; x++)
          fprintf(outfile,"%c", sprdata[offsy+y][offsx+x]);
      }
      fprintf(outfile, "%c", 0);  // extra 64th byte at end of sprite
    }

    if (outfile != NULL) {
      fclose(outfile);
      outfile = NULL;
    }

  }//end if mode==3

  /* ============================ */
  printf("width=%d, height=%d\n", width, height);
  int cnt = 0;
  //fprintf(outfile, "%c%c", (width+7)/8, (height+7)/8);
  //cnt = 2;
  if (mode==4) { // mode == gihires
    for (y=0; y<height; y += 8) {
      for (x=0; x<width; x += 8) {
        // assess every 8x8 character-block from left to right
        for (int yd=0; yd<8; yd++) {
          unsigned char byteval = 0;
          png_byte* row = row_pointers[y+yd];
          for (int xd=0; xd<8; xd++) {
            if (y+yd < height)
            {
              // check threshold for black
              png_byte* ptr = &(row[(x+xd)*multiplier]);
              int r=(ptr[0] + ptr[1] + ptr[2]) / 3;
              if (r < 80 && (x+xd < width))
              {
                byteval |= (1 << (7 - xd));
              }
              else if (r < 160 && (x+xd < width))
              {
                if ( ((xd%2) && (yd%2)) || (!(xd%2) && !(yd%2)))
                  byteval |= (1 << (7 - xd));
              }
            }
          } // end for xd
          fprintf(outfile, "%c", byteval);
          cnt++;
        } // end for yd
      } // end for x
    } // end for y

    /*
    while (cnt < 16777216)
    {
      fprintf(outfile, "%c", 255);
      cnt++;
    }
    */
  } // if mode == gihires

  /* ============================ */

  if (mode==5)  // mode == gihires2
  {
    unsigned char found_start = 0;
    unsigned char charbytes[8];
    unsigned char out2name[256];
    seg_cnt = 0;
    repair_cnt = 0;

    for (int k = 0; k < MAX_SEGS; k++)
    {
      memset(&rowsegs[k], 0, sizeof(reu_row_segment));
      memset(&repairs[k], 0, sizeof(reu_repair_obj));
    }
    memset(&seggedbmp, 0, sizeof(reu_segged_bmp_obj));

    reu_row_segment* curseg = NULL;
    reu_repair_obj* currepair = &(repairs[0]);

    for (y=0; y<height; y += 8) {
      for (x=0; x<width; x += 8) {

      unsigned char maskbytes[8] = { 0 };
      int transparent_count = 0;
      curseg = &(rowsegs[seg_cnt]);

        for (int yd=0; yd<8; yd++) {
          unsigned char byteval = 0;
          png_byte* row = row_pointers[y+yd];

          for (int xd=0; xd<8; xd++) {
            png_byte* ptr = &(row[(x+xd)*multiplier]);
            // check if it's a transparent pixel
            if (y+yd>= height || (ptr[0] == 112 && ptr[1] == 136 && ptr[2] == 136)
              || (ptr[0] == 128 && ptr[1] == 128 && ptr[2] == 128))
            {
              transparent_count++;
              maskbytes[yd] |= 1 << (7 - xd);
            }
            if (y+yd < height)
            {
              // check threshold for black
              png_byte* ptr = &(row[(x+xd)*multiplier]);
              int r=(ptr[0] + ptr[1] + ptr[2]) / 3;
              if (r < 80 && (x+xd < width))
              {
                byteval |= (1 << (7 - xd));
              }
              else if (r < 160 && (x+xd < width))
              {
                if ( ((xd%2) && (yd%2)) || (!(xd%2) && !(yd%2)))
                  byteval |= (1 << (7 - xd));
              }
            }
          } // end for xd

          charbytes[yd] = byteval;
        } // end for yd

        // if all transparent, skip this char and move onto the next
        if (transparent_count == 64)
        {
          if (found_start)
          {
            found_start = 0;
            // if we already started a segment, then a transparent char indicates
            // the end of a segment. So move to next segment
            seg_cnt++;
            rowsegs[seg_cnt].reloffset += 8;
          }
          else
          {
            // if we haven't started a segment yet, then increment the upcoming
            // segment's reloffset
            curseg->reloffset += 8;
          }
        }
        else if (transparent_count > 16)
        {
          if (found_start)
          {
            found_start = 0;
            seg_cnt++;
            rowsegs[seg_cnt].reloffset += 8;
          }
          else
          {
            curseg->reloffset += 8;
          }
          //  spawn a 'mask' repair character, instead of
          //        being part of the segment
          // currepair->reloffset should be already set
          for (int l = 0; l < 8; l++)
          {
            currepair->vals[l*2] = charbytes[l];
            currepair->vals[l*2+1] = maskbytes[l];
          }
          currepair++;
          repair_cnt++;
        }
        else
        {
          if (!found_start)
            found_start = 1;
          // this is a non-transparent character, to let's save this data to the file
          fprintf(outfile, "%c%c%c%c%c%c%c%c", charbytes[0], charbytes[1], charbytes[2],
            charbytes[3], charbytes[4], charbytes[5], charbytes[6], charbytes[7]);
          curseg->length++;
        }
        currepair->reloffset += 8;
      } // end for x

      // if we finished this line and we're mid-segment, lets move to next segment
      if (found_start)
      {
        found_start = 0;
        seg_cnt++;
      }
      rowsegs[seg_cnt].reloffset += (40*8 - ((width+7)/8)*8);
      currepair->reloffset += (40*8 - ((width+7)/8)*8);
    } // end for y

    seggedbmp.num_segments = seg_cnt;
    seggedbmp.num_repairs = repair_cnt;

    // write the extra repair info to the main graphic data file
    for (int k = 0; k < seggedbmp.num_repairs; k++)
    {
      fwrite(&(repairs[k]), sizeof(reu_repair_obj), 1, outfile);
    }

    // write the meta files
    FILE *bmp_meta;
    FILE *segs_meta;
    char bmpMetaFname[256];
    char segsMetaFname[256];
    sprintf(bmpMetaFname, "%s.bmp_meta", outputfilename);
    sprintf(segsMetaFname, "%s.segs_meta", outputfilename);
    
    bmp_meta=fopen(bmpMetaFname,"w");
    segs_meta=fopen(segsMetaFname,"w");

    fwrite(&seggedbmp,sizeof(reu_segged_bmp_obj), 1, bmp_meta);
    fwrite(&rowsegs,sizeof(reu_row_segment), seg_cnt, segs_meta);

    printf("num_segments = %d\n", seggedbmp.num_segments);
    printf("segs-size = %d\n", sizeof(reu_row_segment));
    printf("num_repairs = %d\n", seggedbmp.num_repairs);

    fclose(bmp_meta);
    fclose(segs_meta);
  } // end mode==5

  /* ============================ */

  if (mode==6)  // mode == giparallax
  {
    // first 30 pixels should be affected by parallax scrolling
    // furthest away from player shall move the slowest (1 pixel per frame?)
    // closest to the player shall move the fastest (4 pixels per frame)

   // Allocate memory for new rows (3 bytes per pixel - RGB)
	png_bytep * new_row_pointers;

	char fname[256];

	for (int k = 0; k < strlen(orig_fname); k++)
		if (orig_fname[k] == '.') orig_fname[k] = '\0';

  new_row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
  for (int y=0; y<height; y++)
	{
    new_row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
	}

		int cnt = 0;
		for (int iter = -12; iter <= 12; iter++)
		{
			for (int y = 0; y < height; y++)
			{
				png_byte* orig_row = row_pointers[y];
				png_byte* new_row = new_row_pointers[y];
				memset(new_row, 0, png_get_rowbytes(png_ptr,info_ptr));
				double stepsize = (1.0 + 3.0 * (double)y / 29.0) * iter;
				int istep = (int)stepsize;
				if (y >= 30)
				{
						istep = 4*iter;
				}

				for (int x = 0; x < width; x++)
				{
					int newx = x+istep;
					if (newx < 0 || newx >= width)
						continue;
					
					png_byte* ptr1 = &(orig_row[(x)*multiplier]);
					png_byte* ptr2 = &(new_row[(newx)*multiplier]);
					ptr2[0] = ptr1[0];
					ptr2[1] = ptr1[1];
					ptr2[2] = ptr1[2];
				} // end for x
			} //end for y

			sprintf(fname, "%s%02d.png", orig_fname, cnt);
			writeImage(fname, width, height, new_row_pointers, "test");
			cnt++;
		} //end iter

  } // end if mode == 6
}
;
 
/* ============================================================= */

int main(int argc, char **argv)
{
  if (argc < 4) {
    fprintf(stderr,"Usage: program_name <logo|charrom|hires|4sprmulti|gihires|gihires2|giparallax> <file_in> <file_out> <options\n");
    exit(-1);
  }

  int mode=-1;

  if (!strcasecmp("logo",argv[1])) mode=0;
  if (!strcasecmp("charrom",argv[1])) mode=1;
  if (!strcasecmp("hires",argv[1])) mode=2;
  if (!strcasecmp("4sprmulti",argv[1])) mode=3;
  if (!strcasecmp("gihires",argv[1])) mode=4;
  if (!strcasecmp("gihires2",argv[1])) mode=5;
  if (!strcasecmp("giparallax", argv[1])) mode=6;
  if (mode==-1) {
    fprintf(stderr,"Usage: program_name <logo|charrom|hires|4sprmulti|gihires|gihires2|giparallax> <file_in> <file_out> <options>\n");
    exit(-1);
  }

  printf("argv[0]=%s\n", argv[0]);
  printf("argv[1]=%s\n", argv[1]);
  printf("argv[2]=%s\n", argv[2]);
  printf("argv[3]=%s\n", argv[3]);

  if (argc >= 5)
    offset = atoi(argv[4]);
  if (argc >= 6)
    segment = atoi(argv[5]);

  printf("Reading %s\n",argv[2]);
  read_png_file(argv[2]);

	orig_fname = argv[2];

  printf("Processing with mode=%d and output=%s\n", mode, argv[3]);

  //sprintf(out2name, "%s.mirror");
  //FILE* outfile2=fopen(out2name,"w");
  process_file(mode,argv[3]);

  // for gihires2, process the same file, but mirrored
  if (mode == 5)
  {
    char* orig_outfname = argv[3];
    char fname[256];
    char revpngname[256];

    for (int k = 0; k < strlen(orig_outfname); k++)
      if (orig_outfname[k] == '.') orig_outfname[k] = '\0';

    sprintf(fname, "%s_rev.bin", orig_outfname);
    sprintf(revpngname, "%s_rev.png", orig_outfname);

    // reverse/mirror the pixels in the png rows
    png_byte* tmp_row = (png_byte*) malloc(width*multiplier);
    for (y=0; y<height; y++)
    {
      png_byte* row = row_pointers[y];
      for (x=0; x<width; x++)
      {
        png_byte* ptr1 = &(row[x*multiplier]);
        png_byte* ptr2 = &(tmp_row[(width-x-1)*multiplier]);

        ptr2[0] = ptr1[0];
        ptr2[1] = ptr1[1];
        ptr2[2] = ptr1[2];
        if (multiplier == 4)
          ptr2[3] = ptr1[3];
      }
      for (x=0; x<width; x++)
      {
        png_byte* ptr1 = &(row[x*multiplier]);
        png_byte* ptr2 = &(tmp_row[x*multiplier]);

        ptr1[0] = ptr2[0];
        ptr1[1] = ptr2[1];
        ptr1[2] = ptr2[2];
        if (multiplier == 4)
          ptr1[3] = ptr2[3];
      }
    }
    //writeImage(revpngname, width, height, row_pointers, "blah");

    process_file(mode, fname);
  }

  printf("done\n");

  if (infile != NULL) {
    fclose(infile);
    infile = NULL;
  }

  if (outfile != NULL) {
    fclose(outfile);
    outfile = NULL;
  }

  return 0;
}

/* ============================================================= */
