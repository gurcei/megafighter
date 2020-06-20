typedef struct
{
  char name[64];
  unsigned short boxes[4][4];
} type_hitbox;

type_hitbox lstHitBoxes[] =
{
  {
    "Graphics/Ken/ken_idle1.png",
    20, 18, 34, 31,
    9, 42, 34, 60,
    0, 0, 0, 0,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_crouch1.png",
    23, 24, 37, 37,
    12, 38, 35, 67,
    11, 78, 38, 96,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_crouch2.png",
    26, 44, 40, 57,
    16, 56, 44, 77,
    11, 80, 41, 96,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_walk8.png",
    43, 18, 57, 32,
    32, 30, 55, 71,
    25, 83, 53, 97,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_walk1.png",
    22, 19, 36, 32,
    11, 33, 35, 68,
    5, 80, 33, 96,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_jump_lmhpunch3.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    10, 73, 40, 119,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_walk3.png",
    30, 17, 44, 31,
    18, 32, 39, 67,
    15, 80, 37, 97,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_walk2.png",
    27, 17, 40, 30,
    16, 31, 39, 68,
    11, 78, 33, 97,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_walk5.png",
    34, 19, 47, 32,
    23, 30, 45, 71,
    25, 80, 50, 97,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_walk4.png",
    31, 19, 45, 31,
    20, 31, 42, 69,
    22, 79, 42, 97,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_walk7.png",
    42, 17, 56, 31,
    30, 28, 53, 68,
    27, 78, 47, 96,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_jump_lmhpunch2.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    4, 79, 32, 117,
    24, 85, 54, 109
  },
  {
    "Graphics/Ryu/ryu_jump_lmhpunch1.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    7, 71, 40, 115,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_crouch_lpunch2.png",
    31, 67, 43, 80,
    16, 76, 40, 103,
    11, 105, 42, 120,
    35, 78, 66, 89
  },
  {
    "Graphics/Ryu/ryu_shouryuken2.png",
    33, 68, 48, 81,
    22, 77, 44, 114,
    18, 115, 43, 136,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_shouryuken1.png",
    30, 84, 40, 96,
    18, 92, 43, 117,
    15, 118, 42, 136,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_crouch_lpunch1.png",
    29, 67, 42, 81,
    17, 75, 46, 102,
    12, 107, 41, 121,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_shouryuken7.png",
    18, 55, 27, 70,
    13, 65, 35, 111,
    13, 113, 37, 138,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_shouryuken6.png",
    23, 38, 32, 52,
    19, 49, 37, 91,
    19, 93, 38, 107,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_shouryuken5.png",
    20, 24, 31, 35,
    22, 33, 38, 76,
    23, 77, 39, 92,
    23, 4, 45, 41
  },
  {
    "Graphics/Ryu/ryu_shouryuken4.png",
    22, 42, 32, 54,
    19, 52, 40, 85,
    19, 87, 33, 107,
    26, 16, 48, 60
  },
  {
    "Graphics/Ryu/ryu_block1.png",
    0, 0, 0, 0,
    7, 36, 32, 70,
    8, 75, 40, 97,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_block2.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    8, 81, 40, 97,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_crouch_lkick2.png",
    21, 66, 35, 80,
    15, 77, 39, 107,
    10, 108, 29, 121,
    32, 104, 73, 121
  },
  {
    "Graphics/Ryu/ryu_crouch_lkick1.png",
    26, 66, 40, 79,
    15, 78, 41, 102,
    11, 108, 49, 121,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_hadproj14.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    57, 17, 72, 42
  },
  {
    "Graphics/Ryu/ryu_hadproj11.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    43, 19, 65, 42
  },
  {
    "Graphics/Ryu/ryu_hadproj10.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    35, 21, 45, 39
  },
  {
    "Graphics/Ryu/ryu_hadproj13.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    48, 25, 58, 35
  },
  {
    "Graphics/Ryu/ryu_hadproj12.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    50, 19, 69, 42
  },
  {
    "Graphics/Ryu/ryu_walk6.png",
    38, 19, 51, 32,
    26, 29, 49, 71,
    27, 78, 49, 97,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_lmkick2.png",
    10, 39, 22, 52,
    7, 47, 41, 76,
    25, 77, 37, 118,
    47, 38, 69, 53
  },
  {
    "Graphics/Ryu/ryu_lmkick1.png",
    26, 38, 38, 51,
    19, 49, 45, 90,
    23, 91, 37, 118,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_jump_lmkick2.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    11, 66, 36, 106,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fmpunch1.png",
    21, 25, 35, 39,
    9, 38, 34, 77,
    7, 92, 41, 106,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fmpunch2.png",
    27, 21, 43, 36,
    19, 31, 42, 76,
    7, 92, 52, 104,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fmpunch3.png",
    36, 22, 50, 35,
    24, 32, 48, 76,
    10, 91, 52, 104,
    42, 33, 63, 49
  },
  {
    "Graphics/Ryu/ryu_fmpunch4.png",
    32, 21, 47, 36,
    23, 29, 46, 70,
    10, 89, 50, 103,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_lpunch2.png",
    22, 25, 35, 39,
    11, 35, 34, 80,
    8, 92, 40, 105,
    31, 36, 61, 50
  },
  {
    "Graphics/Ryu/ryu_lpunch1.png",
    21, 26, 35, 39,
    9, 34, 33, 79,
    8, 93, 40, 106,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_jump4.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    14, 21, 40, 67,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_jump2.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    11, 19, 38, 81,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_jump3.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    13, 24, 39, 76,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_jump_hkick3.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    4, 57, 29, 108,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_jump_hkick2.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    4, 59, 25, 105,
    10, 74, 56, 95
  },
  {
    "Graphics/Ryu/ryu_jump_hkick1.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    6, 67, 29, 110,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_idle4.png",
    20, 17, 34, 31,
    9, 26, 31, 72,
    9, 73, 39, 96,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_crouch_hkick3.png",
    24, 71, 34, 80,
    22, 81, 41, 106,
    21, 107, 52, 120,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_crouch_hkick2.png",
    26, 70, 36, 81,
    17, 80, 34, 103,
    18, 104, 31, 115,
    36, 106, 71, 116
  },
  {
    "Graphics/Ryu/ryu_crouch_hkick1.png",
    31, 69, 42, 80,
    22, 78, 38, 105,
    31, 106, 46, 119,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_crouch_hkick5.png",
    14, 68, 23, 81,
    10, 78, 33, 105,
    9, 106, 38, 121,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_crouch_hkick4.png",
    13, 71, 24, 82,
    11, 80, 34, 105,
    5, 106, 35, 120,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_flpunch1.png",
    21, 25, 36, 40,
    10, 35, 33, 80,
    10, 93, 40, 105,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_flpunch3.png",
    21, 25, 36, 40,
    10, 35, 33, 80,
    10, 93, 40, 105,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_flpunch2.png",
    22, 21, 36, 34,
    17, 31, 41, 77,
    10, 91, 41, 105,
    31, 18, 51, 42
  },
  {
    "Graphics/Ryu/ryu_fjump_mhkick1.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    5, 27, 32, 72,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fjump_mhkick2.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    6, 31, 34, 78,
    29, 63, 66, 82
  },
  {
    "Graphics/Ryu/ryu_fjump_mhkick3.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    6, 30, 34, 75,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fjump_lpunch2.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    4, 36, 31, 77,
    25, 44, 42, 76
  },
  {
    "Graphics/Ryu/ryu_fjump_lpunch1.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    6, 31, 31, 73,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fmkick1.png",
    33, 41, 46, 54,
    23, 51, 43, 95,
    8, 108, 42, 120,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fmkick3.png",
    37, 30, 51, 44,
    30, 42, 52, 78,
    31, 104, 45, 121,
    43, 56, 63, 90
  },
  {
    "Graphics/Ryu/ryu_fmkick2.png",
    32, 39, 46, 53,
    25, 51, 45, 93,
    18, 107, 43, 120,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_flkick2.png",
    34, 40, 49, 53,
    28, 49, 49, 89,
    28, 106, 44, 120,
    44, 70, 73, 101
  },
  {
    "Graphics/Ryu/ryu_flkick1.png",
    29, 38, 43, 53,
    22, 51, 46, 92,
    24, 103, 44, 121,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_idle2.png",
    20, 18, 34, 32,
    9, 26, 31, 72,
    9, 73, 39, 96,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_idle3.png",
    20, 18, 34, 32,
    9, 26, 31, 72,
    9, 73, 39, 96,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_crouch_mpunch2.png",
    29, 67, 42, 78,
    17, 76, 38, 100,
    12, 107, 40, 121,
    34, 76, 63, 86
  },
  {
    "Graphics/Ryu/ryu_crouch_mpunch1.png",
    29, 67, 42, 81,
    16, 79, 42, 103,
    9, 109, 39, 120,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fhkick2.png",
    31, 31, 46, 44,
    28, 43, 50, 87,
    31, 98, 41, 119,
    61, 36, 81, 63
  },
  {
    "Graphics/Ryu/ryu_fhkick3.png",
    27, 29, 39, 42,
    26, 43, 48, 80,
    33, 98, 46, 120,
    37, 13, 56, 54
  },
  {
    "Graphics/Ryu/ryu_hkick5.png",
    6, 49, 16, 62,
    8, 59, 39, 95,
    15, 97, 42, 119,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fhkick1.png",
    29, 38, 43, 53,
    22, 51, 45, 90,
    25, 101, 43, 119,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_hkick3.png",
    5, 57, 15, 68,
    13, 62, 43, 84,
    32, 86, 43, 119,
    52, 38, 75, 63
  },
  {
    "Graphics/Ryu/ryu_hkick2.png",
    4, 60, 13, 70,
    14, 60, 37, 82,
    31, 85, 39, 119,
    41, 37, 57, 61
  },
  {
    "Graphics/Ryu/ryu_hkick1.png",
    7, 61, 15, 70,
    15, 58, 41, 85,
    29, 86, 40, 119,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_shouryuken3.png",
    35, 59, 46, 72,
    25, 68, 41, 111,
    20, 111, 42, 136,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_crouch_hpunch2.png",
    25, 42, 37, 55,
    17, 56, 38, 97,
    15, 98, 36, 119,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_crouch_hpunch3.png",
    29, 34, 42, 48,
    27, 49, 45, 92,
    25, 94, 39, 119,
    35, 10, 48, 46
  },
  {
    "Graphics/Ryu/ryu_crouch_hpunch1.png",
    22, 57, 35, 70,
    13, 69, 35, 96,
    13, 97, 38, 120,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_hadproj05.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    36, 25, 45, 37
  },
  {
    "Graphics/Ryu/ryu_hadproj04.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    20, 20, 37, 40
  },
  {
    "Graphics/Ryu/ryu_hadproj07.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    29, 18, 47, 41
  },
  {
    "Graphics/Ryu/ryu_hadproj06.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    29, 18, 40, 43
  },
  {
    "Graphics/Ryu/ryu_hadproj01.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    15, 10, 34, 48
  },
  {
    "Graphics/Ryu/ryu_hadproj03.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    17, 20, 32, 40
  },
  {
    "Graphics/Ryu/ryu_hadproj02.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    13, 16, 31, 43
  },
  {
    "Graphics/Ryu/ryu_hadproj09.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    43, 19, 53, 42
  },
  {
    "Graphics/Ryu/ryu_hadproj08.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    33, 19, 51, 42
  },
  {
    "Graphics/Ryu/ryu_hadouken5.png",
    46, 21, 59, 33,
    37, 27, 62, 62,
    18, 62, 63, 88,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_hadouken4.png",
    46, 21, 59, 33,
    37, 27, 62, 62,
    18, 62, 63, 88,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_jump_lmkick1.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    7, 61, 31, 111,
    24, 50, 48, 88
  },
  {
    "Graphics/Ryu/ryu_hadouken1.png",
    25, 9, 35, 22,
    16, 19, 41, 65,
    14, 65, 52, 88,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_hadouken3.png",
    33, 18, 42, 31,
    25, 24, 42, 63,
    16, 63, 55, 87,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_hadouken2.png",
    23, 15, 33, 28,
    15, 26, 38, 61,
    13, 62, 49, 87,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fhpunch1.png",
    21, 25, 35, 39,
    10, 37, 33, 76,
    6, 92, 41, 105,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fhpunch3.png",
    31, 21, 44, 35,
    23, 29, 46, 74,
    9, 93, 42, 104,
    48, 31, 63, 51
  },
  {
    "Graphics/Ryu/ryu_fhpunch2.png",
    29, 22, 43, 35,
    19, 32, 39, 73,
    7, 91, 39, 104,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fhpunch4.png",
    34, 17, 46, 34,
    28, 30, 48, 74,
    11, 92, 45, 105,
    46, 3, 60, 41
  },
  {
    "Graphics/Ryu/ryu_fjump3.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    11, 33, 55, 59,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fjump2.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    9, 20, 39, 78,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fjump1.png",
    25, 23, 39, 36,
    15, 33, 37, 72,
    16, 73, 40, 96,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fjump7.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    4, 26, 32, 74,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fjump6.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    5, 36, 41, 82,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fjump5.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    10, 30, 57, 57,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fjump4.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    9, 26, 37, 71,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_crouch_mkick1.png",
    25, 66, 39, 79,
    15, 74, 40, 103,
    14, 106, 46, 120,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_crouch_mkick2.png",
    22, 73, 36, 88,
    19, 89, 48, 105,
    18, 107, 35, 119,
    38, 101, 89, 119
  },
  {
    "Graphics/Ryu/ryu_tatsumaki8.png",
    15, 58, 30, 72,
    8, 69, 35, 104,
    11, 105, 27, 127,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_tatsumaki9.png",
    24, 63, 40, 78,
    15, 73, 37, 112,
    16, 113, 37, 137,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_tatsumaki6.png",
    51, 22, 72, 33,
    51, 35, 74, 61,
    51, 63, 72, 74,
    43, 47, 78, 68
  },
  {
    "Graphics/Ryu/ryu_tatsumaki7.png",
    17, 27, 35, 38,
    14, 39, 39, 72,
    22, 74, 37, 89,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_tatsumaki4.png",
    35, 20, 55, 33,
    33, 29, 59, 60,
    37, 62, 61, 79,
    37, 48, 67, 69
  },
  {
    "Graphics/Ryu/ryu_tatsumaki5.png",
    39, 22, 61, 34,
    43, 30, 69, 62,
    47, 64, 67, 75,
    14, 47, 50, 70
  },
  {
    "Graphics/Ryu/ryu_tatsumaki2.png",
    27, 28, 42, 39,
    21, 40, 42, 67,
    22, 69, 40, 85,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_tatsumaki3.png",
    31, 22, 51, 33,
    28, 34, 50, 68,
    31, 68, 48, 81,
    46, 47, 82, 72
  },
  {
    "Graphics/Ryu/ryu_tatsumaki1.png",
    20, 46, 33, 59,
    16, 57, 37, 95,
    18, 96, 28, 136,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_hkick4.png",
    7, 54, 17, 65,
    13, 60, 42, 96,
    32, 98, 43, 120,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_mhpunch1.png",
    21, 25, 34, 39,
    12, 36, 30, 79,
    12, 80, 30, 104,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_mhpunch2.png",
    30, 22, 43, 35,
    20, 33, 42, 74,
    15, 75, 48, 103,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_mhpunch3.png",
    35, 24, 48, 34,
    27, 34, 46, 74,
    17, 75, 48, 102,
    42, 36, 74, 42
  },
  {
    "Graphics/Zangief/zangief_idle1.png",
    31, 24, 44, 42,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
  },
  { 0 }
    };
