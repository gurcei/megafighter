typedef struct
{
  char name[64];
  unsigned short boxes[4][4];
} type_hitbox;

type_hitbox lstHitBoxes[] =
{
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
    "Graphics/Ryu/ryu_idle4.png",
    20, 18, 34, 32,
    9, 26, 31, 72,
    9, 73, 39, 96,
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
    "Graphics/Ryu/ryu_lmkick1.png",
    26, 38, 38, 51,
    19, 49, 45, 90,
    23, 91, 37, 118,
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
    "Graphics/Ryu/ryu_hadouken1.png",
    25, 9, 35, 22,
    16, 19, 41, 65,
    14, 65, 52, 88,
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
    "Graphics/Ryu/ryu_hadouken3.png",
    33, 18, 42, 31,
    25, 24, 42, 63,
    16, 63, 55, 87,
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
    "Graphics/Ryu/ryu_hadouken5.png",
    46, 21, 59, 33,
    37, 27, 62, 62,
    18, 62, 63, 88,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_crouch_hkick1.png",
    31, 69, 42, 80,
    22, 78, 38, 105,
    31, 106, 46, 119,
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
    "Graphics/Ryu/ryu_crouch_hkick3.png",
    24, 71, 34, 80,
    22, 81, 41, 106,
    21, 107,52, 120,
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
    "Graphics/Ryu/ryu_crouch_hkick5.png",
    14, 68, 23, 81,
    10, 78, 33, 105,
    9, 106, 38, 121,
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
    "Graphics/Ryu/ryu_shouryuken2.png",
    33, 68, 48, 81,
    22, 77, 44, 114,
    18, 115, 43, 136,
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
    "Graphics/Ryu/ryu_shouryuken4.png",
    22, 42, 32, 54,
    19, 52, 40, 85,
    19, 87, 33, 107,
    26, 16, 44, 60
  },
  {
    "Graphics/Ryu/ryu_shouryuken5.png",
    20, 24, 31, 35,
    22, 33, 38, 76,
    23, 77, 39, 92,
    23, 4, 43, 41
  },
  {
    "Graphics/Ryu/ryu_shouryuken6.png",
    23, 38, 32, 52,
    19, 49, 37, 91,
    19, 93, 38, 107,
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
    "Graphics/Ryu/ryu_hkick1.png",
    7, 61, 15, 70,
    15, 58, 41, 85,
    29, 86, 40, 119,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_hkick2.png",
    4, 60, 13, 70,
    14, 60, 37, 82,
    31, 85, 39, 119,
    41, 37, 57, 61
  },
  {
    "Graphics/Ryu/ryu_hkick3.png",
    5, 57, 15, 68,
    13, 62, 43, 84,
    32, 86, 43, 119,
    52, 38, 75, 63
  },
  {
    "Graphics/Ryu/ryu_hkick4.png",
    7, 54, 17, 65,
    13, 60, 42, 96,
    32, 98, 43, 120,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_tatsumaki1.png",
    20, 46, 33, 59,
    16, 57, 37, 95,
    18, 96, 28, 136,
    0, 0, 0, 0
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
    "Graphics/Ryu/ryu_crouch_hpunch1.png",
    22, 57, 35, 70,
    13, 69, 35, 96,
    13, 97, 38, 120,
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
    "Graphics/Ryu/ryu_hadproj1.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    19, 18, 30, 42
  },
  {
    "Graphics/Ryu/ryu_hadproj2.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    13, 16, 31, 43
  },
  {
    "Graphics/Ryu/ryu_hadproj3.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    17, 20, 32, 40
  },
  {
    "Graphics/Ryu/ryu_hadproj4.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    20, 20, 37, 40
  },
  {
    "Graphics/Ryu/ryu_hadproj5.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    36, 25, 45, 37
  },
  {
    "Graphics/Ryu/ryu_hadproj6.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    29, 18, 40, 43
  },
  {
    "Graphics/Ryu/ryu_hadproj7.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    29, 18, 47, 41
  },
  {
    "Graphics/Ryu/ryu_hadproj8.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    33, 19, 51, 42
  },
  {
    "Graphics/Ryu/ryu_hadproj9.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    43, 19, 53, 42
  },
  {
    "Graphics/Ryu/ryu_hadproj10.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    35, 21, 45, 39
  },
  {
    "Graphics/Ryu/ryu_hadproj11.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    43, 19, 65, 42
  },
  {
    "Graphics/Ryu/ryu_hadproj12.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    50, 19, 69, 42
  },
  {
    "Graphics/Ryu/ryu_hadproj13.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    48, 25, 58, 35
  },
  {
    "Graphics/Ryu/ryu_hadproj14.png",
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    57, 17, 72, 42
  },
  {
    "Graphics/Ryu/ryu_fjump1.png",
    25, 23, 39, 36,
    15, 33, 37, 72,
    16, 73, 40, 96,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fjump2.png",
    18, 11, 33, 24,
    11, 24, 38, 61,
    14, 63, 34, 79,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fjump3.png",
    48, 52, 60, 66,
    24, 31, 57, 56,
    11, 38, 23, 59,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fjump4.png",
    13, 67, 25, 80,
    14, 38, 37, 71,
    16, 26, 35, 37,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fjump5.png",
    2, 48, 14, 61,
    15, 36, 44, 54,
    45, 26, 60, 43,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fjump6.png",
    1, 25, 13, 37,
    7, 38, 36, 58,
    21, 59, 39, 78,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_fjump7.png",
    12, 11, 30, 24,
    6, 23, 31, 54,
    10, 55, 26, 79,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_jump2.png",
    19, 11, 31, 23,
    14, 24, 36, 52,
    16, 53, 32, 76,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_jump3.png",
    22, 14, 34, 26,
    14, 26, 36, 56,
    17, 58, 35, 70,
    0, 0, 0, 0
  },
  {
    "Graphics/Ryu/ryu_jump4.png",
    24, 13, 37, 26,
    15, 22, 38, 53,
    18, 54, 35, 66,
    0, 0, 0, 0
  },
  { 0 }
};
