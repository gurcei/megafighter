// GURCE'S MUSIC ENGINE
// ====================

// Encoding = 0rxd dddd dooo nnnn  (where d=duration, o=octave, n=note)
//
// I've added an extra bit so that I can have 1/32 notes too
//
// My extra fields are:
// - x = special token, it will override the purpose of the bit-fields in order to accomodate more commands
// - r = repeat back to marker

#define _SID_ 0xD400U

#define REPEAT_TO_BEGINNING 16384U
#define SPECIAL_TOKEN 8192U

// Special command encoding
// Encoding = 001D DDDD DDDC CCCC
// - C = Command (5-bit: 0-31)
// - D = Data for the command (8-bit)
#define CMD_CONTROL           0x01
#define CMD_AUTO_GATE_OFF     0x02  // a flag to decide whether notes are automatically gated off after their duration completes
#define CMD_PULSEWIDTH        0x03  // set the 12-bit pulse width (0 to 4095) of the voice
#define CMD_RESONANCE_FILTER  0x04  // set the 4-bit filter resonance (0-15) and filter on/off for all channels
#define CMD_SET_ADSR          0x05  // set the ADSR values for current channel
#define CMD_DEFINE_SUB        0x06  // defines a subroutine (a note sequence chunk that can be re-used) with an id number (specific to one voice/channel)
#define CMD_SUB_RETURN        0x07  // return from a subroutine back to the calling point
#define CMD_GOSUB             0x08  // call a subroutine chunk of notes
#define CMD_VOLUME_AND_FILTER_MODE 0x09 // sets volume and filter mode
#define CMD_FILTER_CUTOFF_FREQ     0x0a // specify the filter cutoff frequency
#define CMD_NEXT_SCREEN						 0x0b // show the next screen in the animation

#define DO_CMD_CONTROL(val)           (SPECIAL_TOKEN + CMD_CONTROL + ((val) << 5) )   // Sets the control register for the current voice
#define DO_CMD_AUTO_GATE_OFF(val)     (SPECIAL_TOKEN + CMD_AUTO_GATE_OFF + ((val) << 5) )
#define DO_CMD_PULSEWIDTH(val)        (SPECIAL_TOKEN + CMD_PULSEWIDTH), (val)         // Sets the 12-bit pulse width (0 to 4095) for the current voice
#define DO_CMD_RESONANCE_FILTER(res, channels)  (SPECIAL_TOKEN + CMD_RESONANCE_FILTER + ((((res) << 4U) + channels) << 5U))
#define DO_CMD_SET_ADSR(a,d,s,r)      (SPECIAL_TOKEN + CMD_SET_ADSR), ( (((unsigned int)s) << 12U) + (((unsigned int)r) << 8U) + (((unsigned int)a) << 4U) + d)
#define DO_CMD_DEFINE_SUB(val)        (SPECIAL_TOKEN + CMD_DEFINE_SUB + ((val) << 5) )
#define DO_CMD_SUB_RETURN             (SPECIAL_TOKEN + CMD_SUB_RETURN)
#define DO_CMD_GOSUB(val)             (SPECIAL_TOKEN + CMD_GOSUB + ((val) << 5) )
#define DO_CMD_VOLUME_AND_FILTER_MODE(volume, filter_mode) (SPECIAL_TOKEN + CMD_VOLUME_AND_FILTER_MODE + (((filter_mode) << 4U + volume) << 5)
#define DO_CMD_FILTER_CUTOFF_FREQ(freq) (SPECIAL_TOKEN + CMD_FILTER_CUTOFF_FREQ), freq
#define DO_CMD_NEXT_SCREEN            (SPECIAL_TOKEN + CMD_NEXT_SCREEN)

#define FILTER_VOICE1 1
#define FILTER_VOICE2 2
#define FILTER_VOICE3 4

#define FILTER_MODE_LOW_PASS  1
#define FILTER_MODE_BAND_PASS 2
#define FILTER_MODE_HIGH_PASS 4
#define FILTER_MODE_CUT_OFF_V3_OUTPUT 8

// durations
#define D1_32 128
#define D1_16 256
#define D1_8  512
#define D1_8D 768
#define D1_4  1024
#define D1_4_1_16 1280
#define D1_4D 1536
#define D1_2  2048
#define D1_2_1_16 2304
#define D1_2_1_8  2560
#define D1_2D     3072
#define D1        4096

// notes in scale
#define NC  0
#define NCs 1
#define ND  2
#define NDs 3
#define NE  4
#define NF  5
#define NFs 6
#define NG  7
#define NGs 8
#define NA  9
#define NAs 10
#define NB  11

// octaves
#define O0  0
#define O1  16
#define O2  32
#define O3  48
#define O4  64
#define O5  80
#define O6  96
#define O7  112

// values for control register
#define C_RANDOM    128
#define C_PULSE     64
#define C_SAWTOOTH  32
#define C_TRIANGLE  16
#define C_DISABLE   8
#define C_RINGMOD   4
#define C_SYNCH     2
#define C_GATE      1

void prepare_sid(void);
void music_loop_iteration(void);
void prepare_ADSRs(void);
void music_loop_preparation(void);
void prepare_song(int *pv1, int *pv2, int *pv3, unsigned char p_bar_length, unsigned char p_delay_cnt, unsigned char p_use_bug);
