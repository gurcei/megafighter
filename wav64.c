#include <stdio.h>
#include <stdlib.h>

FILE* infile;
FILE* outfile;

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: wav64 infile outfile\n");
    exit(-1);
  }

  printf("infile = %s\n", argv[1]);
  printf("outfile = %s\n", argv[2]);

  infile = fopen(argv[1], "rb");

  char riff[5] = { 0 };
  fread(riff, 4, 1, infile);
  printf("%s\n", riff);

  unsigned int filesize;
  fread(&filesize, 4, 1, infile);
  printf("filesize = 0x%08X (%d)\n", filesize, filesize);

  char type[5] = { 0 };
  fread(type, 4, 1, infile);
  printf("%s\n", type);

  char marker[5] = { 0 };
  fread(marker, 4, 1, infile);
  printf("%s\n", marker);

  unsigned int formatlength;
  fread(&formatlength, 4, 1, infile);
  printf("formatlength = 0x%08X (%d)\n", formatlength, formatlength);

  unsigned short formattype;
  fread(&formattype, 2, 1, infile);
  printf("formattype = 0x%04X (%d)\n", formattype, formattype);

  unsigned short channels;
  fread(&channels, 2, 1, infile);
  printf("channels = 0x%04X (%d)\n", channels, channels);

  unsigned int samplerate;
  fread(&samplerate, 4, 1, infile);
  printf("samplerate = 0x%08X (%d)\n", samplerate, samplerate);

  unsigned int bytespersecond;
  fread(&bytespersecond, 4, 1, infile);
  printf("bytespersecond = 0x%08X (%d)\n", bytespersecond, bytespersecond);

  // bytespersample = 1 : 8-bit mono
  // bytespersample = 2 : 8-bit stereo/16-bit mono
  // bytespersample = 4 : 16-bit stereo
  unsigned short bytespersample;
  fread(&bytespersample, 2, 1, infile);
  printf("bytespersample = 0x%04X (%d)\n", bytespersample, bytespersample);

  unsigned short bitspersample;
  fread(&bitspersample, 2, 1, infile);
  printf("bitspersample = 0x%04X (%d)\n", bitspersample, bitspersample);

  char datamarker[5] = { 0 };
  fread(datamarker, 4, 1, infile);
  printf("%s\n", datamarker);

  unsigned int datasize;
  fread(&datasize, 4, 1, infile);
  printf("datasize = 0x%08X (%d)\n", datasize, datasize);

  outfile = fopen(argv[2], "wb");

  unsigned short len = datasize / bytespersample / 4;
  printf("outlength = %d\n", len);
  fwrite(&len, 2, 1, outfile);

  unsigned short sample;
  for (int k = 0; k < datasize; k += bytespersample)
  {
    fread(&sample, 2, 1, infile);
    sample >>= 12; // convert sample to 4-bits :)
    sample += 8;
    sample &= 0x0F;
    fputc(sample, outfile);

    fread(&sample, 2, 1, infile);
    k += bytespersample; // skip in fours to quarter the sample rate from 44100Hz to 11025Hz
    fread(&sample, 2, 1, infile);
    k += bytespersample;
    fread(&sample, 2, 1, infile);
    k += bytespersample;
  }
  
  fclose(outfile);
  fclose(infile);
  return 0;
}
