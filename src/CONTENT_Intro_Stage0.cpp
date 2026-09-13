#include "globals.h"
#include "CONTENT_Intro_Stage0.h"
#include "SYSTEM_Paths.h"

// Authentic C64 Intro Tile IDs:
// 2170: Cobblestone ground surface
// 2171: Cobblestone ground foundation
// 2172: Grey masonry brick wall
// 2173: Diagonal hatched pipe / column
// 2174: Hatched column pedestal cap
// 2175: Blue water / wave box in 'G'
// 2176: Red spotted mushroom with stem
// 2177: Small bush left (from T_1B)
// 2178: Small bush center with trunk (from T_1B)
// 2179: Authentic C64 grey beveled block
// 2180: Water chasm surface
// 2181: Water chasm deep
// 2182..2187: Floating blue crystal diamond (6 animation frames)
// 2188: Small bush right (from T_1B)
// 2189: Round bush left (from T_12)
// 2190: Round bush right (from T_12)
// 2191: Red cone spike left (from T_29)
// 2192: Red cone spike right (from T_29)
// 2193: Moss shelf left 1 (from T_0D)
// 2194: Moss shelf left 2 (from T_0D)
// 2195: Moss shelf right 1 (from T_0E)
// 2196: Moss shelf right 2 (from T_0E)
// 2197: Round bush variant 2 left (from T_13)
// 2198: Round bush variant 2 center (from T_13)
// 2199: Round bush variant 2 right (from T_13)

static const int TILE_COBBLE_TOP   = 2170;
static const int TILE_COBBLE_BOT   = 2171;
static const int TILE_BRICK_WALL   = 2172;
static const int TILE_HATCH_PIPE   = 2173;
static const int TILE_HATCH_CAP    = 2174;
static const int TILE_WATER_G      = 2175;
static const int TILE_MUSHROOM     = 2176;
static const int TILE_BUSH_SM_L    = 2177;
static const int TILE_BUSH_SM_M    = 2178;
static const int TILE_BEVEL_BLOCK  = 2179;
static const int TILE_WATER_SURF   = 250;
static const int TILE_WATER_DEEP   = 256;
static const int TILE_DIAMOND      = 2182;
static const int TILE_BUSH_SM_R    = 2188;
static const int TILE_BUSH_RD_L    = 2189;
static const int TILE_BUSH_RD_R    = 2190;
static const int TILE_SPIKE        = 2191;
static const int TILE_MOSS_L1      = 2193;
static const int TILE_MOSS_L2      = 2194;
static const int TILE_MOSS_R1      = 2195;
static const int TILE_MOSS_R2      = 2196;
static const int TILE_BUSH_RD2_L   = 2197;
static const int TILE_BUSH_RD2_M   = 2198;
static const int TILE_BUSH_RD2_R   = 2199;

static const char* FONT_G[5] = {
    "1111",
    "1...",
    "1.11",
    "1..1",
    "1111"
};

static const char* FONT_A[5] = {
    "1111",
    "1..1",
    "1111",
    "1..1",
    "1..1"
};

static const char* FONT_N[5] = {
    "1..1",
    "11.1",
    "1.11",
    "1..1",
    "1..1"
};

static const char* FONT_S[5] = {
    "1111",
    "1...",
    "1111",
    "...1",
    "1111"
};

static const char* FONT_T[5] = {
    "1111",
    ".1..",
    ".1..",
    ".1..",
    ".1.."
};

static const char* FONT_E[5] = {
    "1111",
    "1...",
    "111.",
    "1...",
    "1111"
};

static const char* FONT_R[5] = {
    "111.",
    "1..1",
    "111.",
    "1.1.",
    "1..1"
};

static void DrawLetter(int stg, int startX, int startY, const char* pattern[5], int width, int blockTile) {
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < width; ++col) {
            if (pattern[row][col] == '1') {
                int tx = startX + col;
                int ty = startY + row;
                if (tx >= 0 && tx < 256 && ty >= 0 && ty < 30) {
                    StageCache_C64.TileNumber[stg][tx][ty] = blockTile;
                }
            }
        }
    }
}

static void DrawSmallBush(int stg, int startX, int y) {
    if (startX >= 0 && startX + 2 < 256 && y >= 0 && y < 30) {
        StageCache_C64.TileNumber[stg][startX + 0][y] = TILE_BUSH_SM_L;
        StageCache_C64.TileNumber[stg][startX + 1][y] = TILE_BUSH_SM_M;
        StageCache_C64.TileNumber[stg][startX + 2][y] = TILE_BUSH_SM_R;
    }
}

static void DrawRoundBush(int stg, int startX, int y) {
    if (startX >= 0 && startX + 1 < 256 && y >= 0 && y < 30) {
        StageCache_C64.TileNumber[stg][startX + 0][y] = TILE_BUSH_RD_L;
        StageCache_C64.TileNumber[stg][startX + 1][y] = TILE_BUSH_RD_R;
    }
}

static void DrawRoundBush3(int stg, int startX, int y) {
    if (startX >= 0 && startX + 2 < 256 && y >= 0 && y < 30) {
        StageCache_C64.TileNumber[stg][startX + 0][y] = TILE_BUSH_RD2_L;
        StageCache_C64.TileNumber[stg][startX + 1][y] = TILE_BUSH_RD2_M;
        StageCache_C64.TileNumber[stg][startX + 2][y] = TILE_BUSH_RD2_R;
    }
}

static void DrawSpike(int stg, int startX, int y) {
    if (startX >= 0 && startX < 256 && y >= 0 && y < 30) {
        StageCache_C64.TileNumber[stg][startX][y] = TILE_SPIKE;
    }
}

void STAGE_Intro_Generate() {
    const int stg = 0;
    snprintf(StageCache_C64.Name[stg], sizeof(StageCache_C64.Name[stg]), "INTRO PANORAMA");
    StageCache_C64.BackgroundColour[stg] = 1; // 1 = Black background (C64 authentic)
    StageCache_C64.TileType[stg] = 1;         // Underground / Black tile type

    // Clear all tiles
    for (int y = 0; y < 30; ++y) {
        for (int x = 0; x < 256; ++x) {
            StageCache_C64.TileNumber[stg][x][y] = 0;
        }
    }

    const int letterY = 2; // Letters sit at rows y=2..6

    // ==========================================
    // 1. "GIANA" (x=2..22) - 1-tile letter spacing
    // ==========================================
    
    // 'G': x=2..5 (Brick wall tile + water box in bottom opening)
    DrawLetter(stg, 2, letterY, FONT_G, 4, TILE_BRICK_WALL);
    StageCache_C64.TileNumber[stg][3][letterY + 3] = TILE_WATER_G; // Water box in G!
    StageCache_C64.TileNumber[stg][4][letterY + 3] = TILE_WATER_G;

    // 'I': x=7 (Slanted hatched pipe + cap)
    StageCache_C64.TileNumber[stg][7][letterY + 0] = TILE_HATCH_CAP;
    StageCache_C64.TileNumber[stg][7][letterY + 1] = TILE_HATCH_PIPE;
    StageCache_C64.TileNumber[stg][7][letterY + 2] = TILE_HATCH_PIPE;
    StageCache_C64.TileNumber[stg][7][letterY + 3] = TILE_HATCH_PIPE;
    StageCache_C64.TileNumber[stg][7][letterY + 4] = TILE_HATCH_PIPE;

    // 'A': x=9..12 (Grey beveled blocks + authentic small bush on top)
    DrawLetter(stg, 9, letterY, FONT_A, 4, TILE_BEVEL_BLOCK);
    DrawSmallBush(stg, 9, letterY - 1);

    // 'N': x=14..17 (Grey beveled blocks)
    DrawLetter(stg, 14, letterY, FONT_N, 4, TILE_BEVEL_BLOCK);

    // 'A': x=19..22 (Grey beveled blocks + authentic small bush on top)
    DrawLetter(stg, 19, letterY, FONT_A, 4, TILE_BEVEL_BLOCK);
    DrawSmallBush(stg, 19, letterY - 1);

    // ==========================================
    // 2. SPIKE, DIAMOND & WATER CHASM 1 (x=24..43)
    // ==========================================
    // Red spike with crystal diamond sitting right on top
    DrawSpike(stg, 24, 7);
    StageCache_C64.TileNumber[stg][24][6] = TILE_DIAMOND;

    // Small bush next to spike
    DrawSmallBush(stg, 27, 7);

    // Floating diamond before water
    StageCache_C64.TileNumber[stg][31][letterY + 2] = TILE_DIAMOND;

    // Floating diamonds across/after water chasm 1
    StageCache_C64.TileNumber[stg][39][letterY + 2] = TILE_DIAMOND;
    StageCache_C64.TileNumber[stg][41][letterY + 2] = TILE_DIAMOND;

    // ==========================================
    // 3. "SISTERS" (x=44..74) - 1-tile letter spacing
    // ==========================================

    // 'S': x=44..47 (Grey beveled blocks)
    DrawLetter(stg, 44, letterY, FONT_S, 4, TILE_BEVEL_BLOCK);

    // 'I': x=49 (Grey masonry brick column)
    for (int y = 0; y < 5; ++y) {
        StageCache_C64.TileNumber[stg][49][letterY + y] = TILE_BRICK_WALL;
    }

    // 'S': x=51..54 (Grey beveled blocks)
    DrawLetter(stg, 51, letterY, FONT_S, 4, TILE_BEVEL_BLOCK);

    // 'T': x=56..59 (Grey beveled blocks + authentic bush on top)
    DrawLetter(stg, 56, letterY, FONT_T, 4, TILE_BEVEL_BLOCK);
    DrawSmallBush(stg, 56, letterY - 1);

    // 'E': x=61..64 (Grey brick wall + moss in lower shelf)
    DrawLetter(stg, 61, letterY, FONT_E, 4, TILE_BRICK_WALL);
    StageCache_C64.TileNumber[stg][62][letterY + 3] = TILE_MOSS_L1;
    StageCache_C64.TileNumber[stg][63][letterY + 3] = TILE_MOSS_L2;

    // 'R': x=66..69 (Grey beveled blocks)
    DrawLetter(stg, 66, letterY, FONT_R, 4, TILE_BEVEL_BLOCK);

    // 'S': x=71..74 (Grey beveled blocks)
    DrawLetter(stg, 71, letterY, FONT_S, 4, TILE_BEVEL_BLOCK);

    // ==========================================
    // 4. TRAILING DIAMONDS, 3x3 FORMATION & WATER CHASM 2 (x=75..140)
    // ==========================================
    StageCache_C64.TileNumber[stg][76][letterY + 2] = TILE_DIAMOND;
    StageCache_C64.TileNumber[stg][78][letterY + 2] = TILE_DIAMOND;
    StageCache_C64.TileNumber[stg][80][letterY + 2] = TILE_DIAMOND;

    // Red spike with diamond sitting right on top
    DrawSpike(stg, 82, 7);
    StageCache_C64.TileNumber[stg][82][6] = TILE_DIAMOND;

    // Bush next to spike
    DrawSmallBush(stg, 85, 7);

    // 3x3 diamond formation at x=90..94
    for (int dr = 0; dr < 3; ++dr) {
        for (int dc = 0; dc < 3; ++dc) {
            StageCache_C64.TileNumber[stg][90 + dc * 2][letterY + 1 + dr] = TILE_DIAMOND;
        }
    }

    // Trailing decorations and spike
    DrawSpike(stg, 109, 7);
    StageCache_C64.TileNumber[stg][109][6] = TILE_DIAMOND;
    DrawSmallBush(stg, 112, 7);

    StageCache_C64.TileNumber[stg][117][letterY + 2] = TILE_DIAMOND;
    StageCache_C64.TileNumber[stg][120][letterY + 2] = TILE_DIAMOND;
    StageCache_C64.TileNumber[stg][123][letterY + 2] = TILE_DIAMOND;
    DrawRoundBush(stg, 126, 7);
    StageCache_C64.TileNumber[stg][131][letterY + 2] = TILE_DIAMOND;
    StageCache_C64.TileNumber[stg][137][letterY + 2] = TILE_DIAMOND;

    // ==========================================
    // 5. GROUND, BUSHES, MUSHROOMS & WATER (y=7..9)
    // ==========================================
    const int stageWidth = 140;

    for (int x = 0; x < stageWidth; ++x) {
        // Water chasms at x=33..37 and x=97..101
        // Sunk down: row 8 is empty opening (0), row 9 has water surface!
        if ((x >= 33 && x <= 37) || (x >= 97 && x <= 101)) {
            StageCache_C64.TileNumber[stg][x][8] = 0;               // Empty chasm opening
            StageCache_C64.TileNumber[stg][x][9] = TILE_WATER_SURF; // Water surface sunken in hole
        } else {
            StageCache_C64.TileNumber[stg][x][8] = TILE_COBBLE_TOP;
            StageCache_C64.TileNumber[stg][x][9] = TILE_COBBLE_BOT;
        }
    }

    // Ground surface decorations at y=7:
    // Authentic Round Bushes:
    DrawRoundBush(stg, 3, 7);
    DrawRoundBush3(stg, 10, 7);
    DrawRoundBush(stg, 20, 7);
    DrawRoundBush(stg, 45, 7);
    DrawRoundBush3(stg, 52, 7);
    DrawRoundBush(stg, 62, 7);
    DrawRoundBush(stg, 72, 7);
    DrawSmallBush(stg, 103, 7);

    // Red Spotted Mushrooms:
    int mushroomPositions[] = { 1, 7, 15, 49, 58, 67, 92, 107, 134 };
    for (int mp : mushroomPositions) {
        if (mp < stageWidth && StageCache_C64.TileNumber[stg][mp][8] == TILE_COBBLE_TOP) {
            StageCache_C64.TileNumber[stg][mp][7] = TILE_MUSHROOM;
        }
    }

    // Wrap-around boundary
    StageCache_C64.TileNumber[stg][stageWidth - 1][8] = TILE_COBBLE_TOP;
    StageCache_C64.TileNumber[stg][stageWidth - 1][9] = TILE_COBBLE_BOT;

    // Checkpoints & defaults
    StageCache_C64.StartPositionX[stg][0] = 32;
    StageCache_C64.StartPositionY[stg][0] = 7 * 16;
    for (int cp = 1; cp < 4; ++cp) {
        StageCache_C64.StartPositionX[stg][cp] = (32 + cp * 32 * 16);
        StageCache_C64.StartPositionY[stg][cp] = 7 * 16;
    }

    for (int e = 0; e < MAX_NUM_ENEMIES; ++e) {
        StageCache_C64.Enemy_InUse[stg][e] = 0;
        StageCache_C64.Enemy_Type[stg][e] = 0;
        StageCache_C64.Enemy_PosX[stg][e] = 0;
        StageCache_C64.Enemy_PosY[stg][e] = 0;
        StageCache_C64.Enemy_Direction[stg][e] = 0;
    }

    StageCache_C64.PlatformInUse[stg] = 0;
}
