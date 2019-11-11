#include "music.h"
#include "common.h"
#include "util.h"

int autogateflag[3] = {1, 1, 1}; // one flag per voice channel. By default, notes will automatically gate-off after duration expires

static unsigned int addr;
static unsigned char temp;
static unsigned char lo;

int** psub_idx;
int sub_idx_v1[6] = { 0 };  // indices of the subroutines found in each voice channel
int sub_idx_v2[6] = { 0 };  // indices of the subroutines found in each voice channel
int sub_idx_v3[6] = { 0 };  // indices of the subroutines found in each voice channel
int* psub_idxs[3] = { sub_idx_v1, sub_idx_v2, sub_idx_v3 };
int* pstart_idx;
int start_idx[3] = { -1, -1, -1 };   // the start indices of each channel (equates to the index of the first non-subroutine note or command)
int* pcaller_idx;
int caller_idx[3] = { -1 };  // the index of the caller to the subroutine

static int *v1;
static int *v2;
static int *v3;

static unsigned char delay_cnt = 3;
static unsigned char use_bug = 1;

extern unsigned char screen_flag;

// 30 DIM FQ(11) : REM Dimension array to contain base frequency for each note
// 60 FOR I=0 TO 11: READ FQ(I): NEXT : REM Read in base frequency for each note.

// define frequency table
unsigned int fq[12] =
{
  34334U, 36376U, 38539U, 40830U,
  43258U, 45830U, 48556U, 51443U,
  54502U, 57743U, 61176U, 64814U
};
// Taking a look at the Commodore 64 Programmer's Reference Guide
// "Appendix E - Musical Note Values" can be assesed to learn which notes
// are referenced in the array above
// C7  : 34334U = 0x861E : HI = 0x86 = 134 : LO = 0x1E = 30
// Cs7 : 36376U = 0x8E18 = HI = 0x8E = 142 : LO = 0x18 = 24

// Store waveform control byte for each voice
unsigned char v[3] =
{
  C_TRIANGLE + C_GATE,
  C_PULSE + C_GATE,
  C_RANDOM + C_GATE
};

                              // These are to be stored in: 
                              // (0xd404) v1 control register
                              // 0001 0001 (17)
                              // rpst omzg (r=random noise, p=pulse, s=sawtooth, t=triangle
                              //            o=disable oscillator, m=ring mod. v1 with v3,
                              //            z=synchronize v1 with v3, g=gate bit: 1=start attack+decay+sustain, 0=start release
                              // (0xd40b) v2 control register
                              // 0100 0001 (65)
                              // rpst omzg (r=random noise, p=pulse, s=sawtooth, t=triangle
                              //            o=disable oscillator, m=ring mod. v2 with v1,
                              //            z=synchronize v2 with v1, g=gate bit: 1=start attack+decay+sustain, 0=start release
                              // (0xd412) v3 control register
                              // 1000 0001 (129)
                              // rpst omzg (r=random noise, p=pulse, s=sawtooth, t=triangle
                              //            o=disable oscillator, m=ring mod. v3 with v2,
                              //            z=synchronize v3 with v2, g=gate bit: 1=start attack+decay+sustain, 0=start release


// vars to hold hfreq, lfreq and control register values for the current 1/32nd measure (per voice)
unsigned char *ph, *pl, *pc;
unsigned char h[3];   // current MSB frequency for each voice
unsigned char l[3];   // current LSB frequency for each voice
unsigned char c[3];   // current control register for each voice
// H = high-byte of frequency
// L = low-byte of frequency
// C = control byte of waveform

int marker_pos = 0;     // the positon of the marker to repeat back to
int repeat_to_marker_pos = 0;   // the position of the repeat-to-marker token
int repeat_to_marker_count = 0; // the number of times to repeat back to marker
int repeat_to_beginning_pos = 0; // the position of the repeat-to-beginning token
int rptcnt = 0;       // a counter for the number of times to repeat a marked section of music
int* pnm;
int nm[3] = { 0 };   // the current encoded note-data value for each voice
unsigned char *pwa, *pwb;
unsigned char wa[3] = { 0 };   // each decoded voice's control register value with gating on
unsigned char wb[3] = { 0 };   // each decoded voice's control register value with gating off
  
int dr = 0;   // duration of currently decoded note-data
int oc = 0;   // octave of currently decoded note-data
int nt = 0;   // note-in-octave of currently decoded note-data
unsigned long fr = 0; // 16-bit frequency of currently decoded note-data
unsigned int hf;      // MSB frequency of currently decoded note-data
unsigned int lf;      // LSB frequency of currently decoded note-data

unsigned char bar_idx = 0;  // which measure within the current bar are we

unsigned int music_idx = 0;   // the number of 1/32nd measures that have transpired since start of the song
unsigned char music_t = 0;  // loop variable used for timers

unsigned char k;
int j;

//void init_sid(void)
//{
//  // 10 S = 54272 : FOR L = S TO S+24 : POKE L, 0 : NEXT
//  for (addr = _SID_; addr <= (_SID_+24); addr++)
//  {
//    Poke(addr, 0);
//  }
//}

void prepare_sid(void)
{
  __asm__ ( "jsr " FUNC_INIT_SID );
  __asm__ ( "jsr " FUNC_PREPARE_ADSRS );

  __asm__ ( "lda #31" );
  __asm__ ( "sta $d400+24" );  // (0xd418) Set volume 15, low-pass filtering
                               // 0001 1111
                               // chbl vvvv (c=cutoff voice3 output 1=off/0=on, h=select high-pass filter, b=select band-pass filter,
                               // l=select low-pass filter, v = volume)
}

void decode_note(void)
{
  // Decode duration and octave.

  // Encoding = dddd dooo nnnn  (where d=duration, o=octave, n=note)
  //dr = nm[k] / 128;
  dr = *pnm >> 7;
  //oc = (nm[k] - 128*dr) / 16;  // ok, now I get what they're doing here, overly verbose masking :)
  oc = (*pnm >> 4) & 0x07;

  // Decode note.
  //nt = (nm[k] - 128*dr) - 16*oc;   // more overly verbose masking :)
  nt = *pnm & 0x0F;
}

void calculate_note_frequency(void)
{
  // Get base frequency for this note.
  fr = fq[nt];

  //if (k == 1) // voice 2 debugging
  //{
    //printf("nm=%d dr=%d oc=%d nt=%d fr=%lu\n", nm, dr, oc, nt, fr);
  //}

  //fr <<= 16;

  // If highest octave, skip division loop.
  if (oc != 7)
  {
    // Divide base frequency by 2 appropriate number of times.
    for (j = 6; j >= oc; j--)
      fr = fr >> 1;
  }

  //fr >>= 16;

  // Get high and low frequency bytes.
  hf = fr >> 8;
  lf = fr & 0xff;
}

unsigned char bar_length = 72;  // how many 1/32 measures within a bar (can vary depending on time signature of song)

// void prepare_song(int *pv1, int *pv2, int *pv3, unsigned char p_bar_length, unsigned char p_delay_cnt, unsigned char p_use_bug)
// {
//   v1 = pv1;
//   v2 = pv2;
//   v3 = pv3;
//   bar_length = p_bar_length;
//   delay_cnt = p_delay_cnt;
//   use_bug = p_use_bug;
// }

void prepare_waveform_control_registers(void)
{
  // Set waveform controls to proper voice. If silence, set waveform controls to 0.
  *pwa = v[k];   // set the waveform control to proper voice
  *pwb = *pwa - 1; // turn the gate-bit (bit0) of the voice's control-register off (releases the note from the sustain)

  if (*pnm < 0) // if encoded note value is negative, this equates to silence, so set waveform controls to 0.
  {
    *pnm = -*pnm; // invert it, and what remains equates to a duration value

    *pwa = 0;
    *pwb = 0;
  }
}

int cmd;
unsigned int data;
int* pidx;
int idx[3] = { 0 };  // index to the current note+octave+duration value of each voice
unsigned char* premain;
unsigned char remain[3] = { 0 };  // the duration remaining for the current note on this voice
unsigned char found_note = 0;
unsigned char within_sub = 0;
int **ppvoice;
int *pvoice;
int** pptr_voice;
int* ptr_voice[3];  // pointer to actual voice data;
int *pvoices[3];
unsigned char sid_offset[3] = { 0, 7, 14 };
int *pval;

// each loop iteration, let's just decode the bare-minimum of musical
// note information needed (rather than decoding it all up-front!)
// int update_decoded_music(void)
// {
//   // check for any special tokens/markers on any channels first (prior to decoding notes)
//   
// #ifdef DEBUG
//   Poke(0xd020, 10);
// #endif
// 
//   pidx = idx;
//   premain = remain;
//   pnm = nm;
//   pstart_idx = start_idx;
//   pcaller_idx = caller_idx;
//   ppvoice = pvoices;
//   pptr_voice = ptr_voice;
//   psub_idx = psub_idxs;
// 
//   for (k = 0; k <= 2; k++)
//   {
//     found_note = 0;
//     within_sub = 0;
// 
//     pvoice = *ppvoice;
//     pval = *pptr_voice;
// 
//     if (*premain == 0)
//     {
//       //char s[8];
//       while (!found_note)
//       {
//         // read the next encoded note
//         *pnm = *pval;
//         
//         /*
//         sprintf(s, "%d", pval); // pvoice[*pidx]); // *pnm);
//         Poke(BANK + 0x400, s[0]);
//         Poke(BANK + 0x401, s[1]);
//         Poke(BANK + 0x402, s[2]);
//         Poke(BANK + 0x403, s[3]);
//         Poke(BANK + 0x404, s[4]);
//         Poke(BANK + 0x405, s[5]);
//         while(1)
//         ;
//         */
//         //printf("idx[%d]=%d\n", k, idx[k]);
// 
//         if (*pnm < 0)  // is this a rest note?
//         {
//           //printf("[%d] rest note\n", k);
//           if (!within_sub)
//           {
//             found_note = 1;
//             if (*pstart_idx == -1)
//             {
//               //*pstart_idx = *pidx;
//               *pstart_idx = (int)pval;
//               //printf("start_idx[%d] = %d\n", k, idx[k]);
//             }
//             break;
//           }
//           pval++;
//           //(*pidx)++;
//         }
//         else if (*pnm == REPEAT_TO_BEGINNING)
//         {
//           //printf("[%d] rpt(%d)\n", k, start_idx[k]);
//           //*pidx = *pstart_idx;
//           pval = (int*)*pstart_idx; // back to start pointer of track
//           *premain = 0;
//         }
//         else if (*pnm & SPECIAL_TOKEN)
//         {
//           cmd = *pnm & 0x1F;
// #ifdef DEBUG
//           Poke(0xd020, 7);
// #endif
//           data = (*pnm >> 5) & 0xff;
//           //(*pidx)++;
//           pval++;
// 
//           if (!within_sub)  // only parse certain commands if we are not in the initial sub-hunting phase...
//           {
//             if (*pstart_idx == -1 && cmd != CMD_DEFINE_SUB)
//             { 
//               *pstart_idx = (int)(pval-1); // *pidx;
//               //printf("start_idx[%d] = %d\n", k, idx[k]);
//             }
//             switch(cmd)
//             {
//               case CMD_CONTROL:
//                 v[k] = data;
//                 break;
// 
//               case CMD_AUTO_GATE_OFF:
//                 autogateflag[k] = data;
//                 break;
// 
//               case CMD_PULSEWIDTH:
//                 data = *pval; // pvoice[*pidx];
//                 PokeW(_SID_+(sid_offset[k])+2, data);
//                 //(*pidx)++;
//                 pval++;
//                 break;
// 
//               case CMD_RESONANCE_FILTER:
//                 Poke(_SID_+0x17, data);
//                 break;
// 
//               case CMD_VOLUME_AND_FILTER_MODE:
//                 Poke(_SID_+0x18, data);
//                 break;
// 
//               case CMD_FILTER_CUTOFF_FREQ:
//                 data = *pval;
//                 Poke(_SID_+0x15, data & 0x07);  // low bits 0-2
//                 Poke(_SID_+0x16, data << 3); // high byte (bits 3-10)
//                 pval++;
//                 break;
// 
//               case CMD_SET_ADSR:
//                 data = *pval; // pvoice[*pidx];
//                 if (use_bug)
//                 {
//                 PokeW(_SID_+(sid_offset[k])+5, data);
//             // this is a bug in voice 3, but my ears got used to it, so I'm keeping it :)
//             Poke(_SID_+19, (0<<4) + 8);   // Set Attack/Decay for voice 3 (A=0, D=10)
//                                   // 0000 1010
//                                   // aaaa dddd (a=attack, d=decay)
//             Poke(_SID_+20, (0<<4) + 5);  // Set Sustain/Release for voice 3 (S=0, R=5)
//                 }
//                 else
//                 {
//                   Poke(_SID_+(sid_offset[k])+5, data & 0xff);
//                   Poke(_SID_+(sid_offset[k])+6, (data >> 8) & 0xff);
//                 }
//                 //(*pidx)++;
//                 pval++;
//                 break;
// 
// 							case CMD_NEXT_SCREEN:
// 								screen_flag = 1;
// 								break;
//             } // end switch
//           } // end if within sub
// 
//           switch(cmd)
//           {
//             case CMD_DEFINE_SUB:
//               //printf("[%d] define_sub(%d) at idx=%d\n", k, data, idx[k]);
//               (*psub_idx)[data] = (int)pval;
//               // sub_idx[k][data] = pval; // *pidx;
//               within_sub = 1;
//               break;
// 
//             case CMD_SUB_RETURN:
//               if (!within_sub)
//               {
//                 //printf("[%d] sub_return\n", k, caller_idx[k], within_sub);
//                 pval = (int*)*pcaller_idx;
//                 //*pidx = *pcaller_idx;
//               }
//               within_sub = 0;
//               break;
// 
//             case CMD_GOSUB:
//               //printf("[%d] cmd_gosub(%d)\n", k, data);
//               if (*pstart_idx == -1)
//               {
//                 *pstart_idx = (int)(pval - 1); // *pidx - 1;
//                 //printf("start_idx[%d] = %d\n", k, idx[k]);
//               }
//               *pcaller_idx = (int)(pval); // *pidx;
//               pval = (int*)((*psub_idx)[data]);
//               // pval = sub_idx[k][data];
//               //*pidx = sub_idx[k][data];
//               break;
//           }
// #ifdef DEBUG
//           Poke(0xd020, 10);
// #endif
//         }
//         else
//         {
//           if (!within_sub)
//           {
//             found_note = 1;
//             if (*pstart_idx == -1)
//             { 
//               *pstart_idx = (int)pval; // *pidx;
//               //printf("start_idx[%d] = %d\n", k, idx[k]);
//             }
//             break;
//           }
//           //(*pidx)++;
//           pval++;
//         }
//       } // end while(!found_note)
//     } // end if remain[k] == 0
// #ifdef DEBUG
//     Poke(0xd020, 1);
// #endif
//     *pptr_voice = pval;
//     premain++;
//     pnm++;
//     //__asm__ ( "inc $d020" );
//     pidx++;
//     pstart_idx++;
//     pcaller_idx++;
//     ppvoice++;
//     pptr_voice++;
//     psub_idx++;
// #ifdef DEBUG
//     Poke(0xd020, 10);
// #endif
//   } // end for k=0 to 2
// 
//     //__asm__ ( "lda $d021" );
//     //__asm__ ( "sta $d020" );
//  
// #ifdef DEBUG
//   Poke(0xd020, 11);
// #endif
//   // REM Begin decoding loop for each voice.
//   // 100 FOR K = 0 TO 2
//   pidx = idx;
//   pnm = nm;
//   premain = remain;
//   pl = l;
//   ph = h;
//   pc = c;
//   ppvoice = pvoices;
//   pptr_voice = ptr_voice;
//   pwa = wa;
//   pwb = wb;
// 
//   for (k = 0; k <= 2; k++)
//   {
//     pvoice = *ppvoice;
//     pval = *pptr_voice;
// 
//     if (*premain == 0)
//     {
//       // read the next encoded note
// #ifdef DEBUG
//       Poke(0xd020, 9);
// #endif
//       *pnm = *pval; // **pptr_voice; //pvoice[*pidx];
// 
//       // move index in preparation of next note (once this note finishes)
// #ifdef DEBUG
//       Poke(0xd020, 6);
// #endif
//       pval++; // (*pptr_voice)++;
// #ifdef DEBUG
//       Poke(0xd020, 11);
// #endif
//       //(*pidx)++;
// 
//       // If coded note is zero, then just pause at end of the track
//       if (*pnm == 0)
//       {
//         pval--; // (*pptr_voice)--;
//         //(*pidx)--;
// 
//         *pptr_voice = pval;
//         //pidx++;
//         pnm++;
//         premain++;
//         pl++;
//         ph++;
//         pc++;
//         ppvoice++;
//         pptr_voice++;
//         pwa++;
//         pwb++;
//         continue;
//       }
// 
//       // preparing wa and wb vars
// #ifdef DEBUG
//       Poke(0xd020, 0);
// #endif
//       prepare_waveform_control_registers();
// 
//       // figure out dr, oc & nt (duration, octave and note-within-octave)
// #ifdef DEBUG
//       Poke(0xd020, 1);
// #endif
//       decode_note();
// 
//       // figure out fr, lf an hf (16-bit frequency and 8-bit LSB and MSB frequency values)
// #ifdef DEBUG
//       Poke(0xd020, 2);
// #endif
//       calculate_note_frequency();
// #ifdef DEBUG
//       Poke(0xd020, 11);
// #endif
// 
//       // prepare start of note details
//       *pl = lf;
//       *ph = hf;
//       *pc = *pwa;
//       *premain = dr;
//     }
//     else if (*premain == 1 && autogateflag[k]) // are we on the last 1/32nd of the present note? Time to gate off?
//     {
//       // this is where wb (pwb) used to be used
//       *pc = *pc & 0xFE; // assure gate-bit is turned off
//     }
// 
//     (*premain)--;
// 
//     //printf("v=%d,l=%d,h=%d,c=%d\n", k, l[k], h[k], c[k]);
// 
//     *pptr_voice = pval;
//     pidx++;
//     pnm++;
//     premain++;
//     pl++;
//     ph++;
//     pc++;
//     ppvoice++;
//     pptr_voice++;
//     pwa++;
//     pwb++;
//   }
// 
//   return 1;
// }

//void prepare_ADSRs(void)
//{
//  // 500 POKE S+5, 0 : POKE S+6, 240 : REM Set Attack/Decay for voice 1 (A=0, D=0) : Set Sustain/Release for voice 1 (S=15, R=0)
//  Poke(_SID_+5, 0);   // (0xd405) Set Attack/Decay for voice 1 (A=0, D=0)
//                      // aaaa dddd (a=attack, d=decay)
//  Poke(_SID_+6, (15<<4) + 0); // Set Sustain/Release for voice 1 (S=15, R=0)
//                      // 1111 0000
//                      // ssss rrrr (s=sustain, r=release)
//
//  // 510 POKE S+12, 85 : POKE S+13, 133 : REM Set Attack/Decay for voice 2 (A=5, D=5) : Set Sustain/Release for voice 2 (S=8,  R=5)
//  Poke(_SID_+12, (2<<4) + 5);   // (0xd40c) Set Attack/Decay for voice 2 (A=5, D=5)
//                        // 0101 0101
//                        // aaaa dddd (a=attack, d=decay)
//  Poke(_SID_+13, (8<<4) + 1);  // (0xd40d) Set Sustain/Release for voice 2 (S=8, R=5)
//                        // 1000 0101
//                        // ssss rrrr (s=sustain, r=release)
//
//  // 520 POKE S+19, 10 : POKE S+20, 197 : REM Set Attack/Decay for voice 3 (A=0, D=10) : Set Sustain/Release for voice 3 (S=12, R=5)
//  Poke(_SID_+19, (0<<4) + 8);   // Set Attack/Decay for voice 3 (A=0, D=10)
//                        // 0000 1010
//                        // aaaa dddd (a=attack, d=decay)
//  Poke(_SID_+20, (0<<4) + 5);  // Set Sustain/Release for voice 3 (S=0, R=5)
//                        // 0000 0101
//                        // ssss rrrr (s=sustain, r=release)
//}


//void music_loop_preparation(void)
//{
//  //REM Start loop for every 1/32nd of a measure.
//  rptcnt = repeat_to_marker_count;
//
//  // initialise vars
//  start_idx[0] = -1;
//  start_idx[1] = -1;
//  start_idx[2] = -1;
//  
//  caller_idx[0] = -1;
//  caller_idx[1] = -1;
//  caller_idx[2] = -1;
//
//  bar_idx = 0;
//  music_idx = 0;
//  music_t = 0;
//  marker_pos = 0;
//  repeat_to_marker_pos = 0;
//  repeat_to_marker_count = 0;
//  repeat_to_beginning_pos = 0;
//  rptcnt = 0;
//  idx[0] = 0;
//  idx[1] = 0;
//  idx[2] = 0;
//  remain[0] = 0;
//  remain[1] = 0;
//  remain[2] = 0;
//  found_note = 0;
//  within_sub = 0;
//
//  autogateflag[0] = 1;
//  autogateflag[1] = 1;
//  autogateflag[2] = 1;
//
//  // reset default voice control values
//  v[0] = C_TRIANGLE + C_GATE;
//  v[1] = C_PULSE + C_GATE;
//  v[2] = C_RANDOM + C_GATE;
//
//  // set first row of colour ram to grey
//  for (addr = 0xd800; addr < 0xd800+40; addr++)
//    Poke(addr, 11);
//
//  pvoices[0] = v1;
//  pvoices[1] = v2;
//  pvoices[2] = v3;
//  // the dynamic pointer to the current note within each voice
//  ptr_voice[0] = v1;
//  ptr_voice[1] = v2;
//  ptr_voice[2] = v3;
//
///* Set the C level interrupt request vector to the given address */
//  // REM Set high pulse width for voice 2 : Set high frequency for filter cutoff : Set resonance for filter and filter voice 3
//  // 50 POKE S+10, 8: POKE S+22, 128: POKE S+23, 244 
//  Poke(_SID_+10, 8);   // (0xd40a) Set high pulse width for voice 2
//  Poke(_SID_+22, 128); // (0xd416) Set high frequency for filter cutoff
//  //Poke(_SID_+23, 244); // (0xd417) 1111 0100 Set resonance for filter and filter voice 3
//                                // rrrr e321 (r=filter resonance, e=filter external input, 1/2/3 = filter voice 1/2/3)
//
//}

void music_loop_iteration(void)
{
  // slow down the music loop
  music_t++;
  if (music_t < delay_cnt)
    return;
  music_t = 0;

  // update_decoded_music();

  //printf("fr=%u\n", l[0][i] + 256*h[0][i]);

  // i'm suspecting we lose the 'release' segment of the ADSR due to repeatedly
  // hitting these registers every iteration, rather than once per note action...
  if (use_bug)
  {
    // 550 POKE S,   L(0, I) : POKE S+7,  L(1, I) : POKE S+14, L(2, I) : REM POKE low frequency from activity array for all voices.
    Poke(_SID_,    l[0]);  // (0xd400) v1 freq lo-byte
    Poke(_SID_+7,  l[1]);  // (0xd407) v2 freq lo-byte
    Poke(_SID_+14, l[2]);  // (0xd40e) v3 freq lo-byte

    // 560 POKE S+1, H(0, I) : POKE S+8,  H(1, I) : POKE S+15, H(2, I) : REM POKE high frequency from activity array for all voices.
    Poke(_SID_+1,  h[0]);  // (0xd401) v1 freq hi-byte
    Poke(_SID_+8,  h[1]);  // (0xd408) v2 freq hi-byte
    Poke(_SID_+15, h[2]);  // (0xd40f) v3 freq hi-byte

    // 570 POKE S+4, C(0, I) : POKE S+11, C(1, I) : POKE S+18, C(2, I) : REM POKE waveform control from activity array for all voices.
    Poke(_SID_+4,  c[0]);  // (0xd404) v1 control register
                              // rpst omzg (r=random noise, p=pulse, s=sawtooth, t=triangle
                              //            o=disable oscillator, m=ring mod. v1 with v3,
                              //            z=synchronize v1 with v3, g=gate bit: 1=start attack+decay+sustain, 0=start release
    Poke(_SID_+11, c[1]);  // (0xd40b) v2 control register
                              // rpst omzg (r=random noise, p=pulse, s=sawtooth, t=triangle
                              //            o=disable oscillator, m=ring mod. v2 with v1,
                              //            z=synchronize v2 with v1, g=gate bit: 1=start attack+decay+sustain, 0=start release
    Poke(_SID_+18, c[2]);  // (0xd412) v3 control register
                              // rpst omzg (r=random noise, p=pulse, s=sawtooth, t=triangle
                              //            o=disable oscillator, m=ring mod. v3 with v2,
                              //            z=synchronize v3 with v2, g=gate bit: 1=start attack+decay+sustain, 0=start release
  } // end use bug
  else
  {
    if (c[0]) // only do for non-zero values
    {
      Poke(_SID_,    l[0]);  // (0xd400) v1 freq lo-byte
      Poke(_SID_+1,  h[0]);  // (0xd401) v1 freq hi-byte
      Poke(_SID_+4,  c[0]);  // (0xd404) v1 control register
    }
    if (c[1])
    {
      Poke(_SID_+7,  l[1]);  // (0xd407) v2 freq lo-byte
      Poke(_SID_+8,  h[1]);  // (0xd408) v2 freq hi-byte
      Poke(_SID_+11, c[1]);  // (0xd40b) v2 control register
    }
    if (c[2])
    {
      Poke(_SID_+14, l[2]);  // (0xd40e) v3 freq lo-byte
      Poke(_SID_+15, h[2]);  // (0xd40f) v3 freq hi-byte
      Poke(_SID_+18, c[2]);  // (0xd412) v3 control register
    }
  }

  /*
  printf("l0=%d,l1=%d,l2=%d\n",l[0][i], l[1][i], l[2][i]);
  printf("h0=%d,h1=%d,h2=%d\n",h[0][i], h[1][i], h[2][i]);
  printf("c0=%d,c1=%d,c2=%d\n",c[0][i], c[1][i], c[2][i]);
  cgetc();
  */

  // 580 FOR T = 1 TO 80 : NEXT : NEXT : REM Timing loop for 1/32nd of a measure and back for next 1/32nd measure.
  //wait_timer(4);
  //for (t = 0; t < 500; t++)
  //  ;


  music_idx++; // increment to next 1/32nd measure
  bar_idx++;
  if (bar_idx == bar_length)
    bar_idx = 0;
}


