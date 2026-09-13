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
// 2177: Green bush (left)
// 2178: Green bush (right)
// 1387: Authentic C64 grey beveled block
// 3:    Floating blue crystal diamond
// 250:  Water chasm surface
// 251:  Water chasm deep

static const int TILE_COBBLE_TOP   = 2170;
static const int TILE_COBBLE_BOT   = 2171;
static const int TILE_BRICK_WALL   = 2172;
static const int TILE_HATCH_PIPE   = 2173;
static const int TILE_HATCH_CAP    = 2174;
static const int TILE_WATER_G      = 2175;
static const int TILE_MUSHROOM     = 2176;
static const int TILE_BUSH_L       = 2177;
static const int TILE_BUSH_R       = 2178;
static const int TILE_BEVEL_BLOCK  = 2179;
static const int TILE_DIAMOND      = 2182;
static const int TILE_WATER_SURF   = 250;
static const int TILE_WATER_DEEP   = 256;

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
    "11111",
    "..1..",
    "..1..",
    "..1..",
    "..1.."
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
    // 1. "GIANA" (x=4..34)
    // ==========================================
    
    // 'G': x=4..7 (Brick wall tile + water box in bottom opening)
    DrawLetter(stg, 4, letterY, FONT_G, 4, TILE_BRICK_WALL);
    StageCache_C64.TileNumber[stg][5][letterY + 3] = TILE_WATER_G; // Water box in G!
    StageCache_C64.TileNumber[stg][6][letterY + 3] = TILE_WATER_G;

    // 'I': x=10 (Slanted hatched pipe + cap)
    StageCache_C64.TileNumber[stg][10][letterY + 0] = TILE_HATCH_CAP;
    StageCache_C64.TileNumber[stg][10][letterY + 1] = TILE_HATCH_PIPE;
    StageCache_C64.TileNumber[stg][10][letterY + 2] = TILE_HATCH_PIPE;
    StageCache_C64.TileNumber[stg][10][letterY + 3] = TILE_HATCH_PIPE;
    StageCache_C64.TileNumber[stg][10][letterY + 4] = TILE_HATCH_PIPE;

    // 'A': x=13..16 (Grey beveled blocks + bush on top)
    DrawLetter(stg, 13, letterY, FONT_A, 4, TILE_BEVEL_BLOCK);
    StageCache_C64.TileNumber[stg][14][letterY - 1] = TILE_BUSH_L;
    StageCache_C64.TileNumber[stg][15][letterY - 1] = TILE_BUSH_R;

    // 'N': x=19..22 (Grey beveled blocks)
    DrawLetter(stg, 19, letterY, FONT_N, 4, TILE_BEVEL_BLOCK);

    // 'A': x=25..28 (Grey beveled blocks + bush on top)
    DrawLetter(stg, 25, letterY, FONT_A, 4, TILE_BEVEL_BLOCK);
    StageCache_C64.TileNumber[stg][26][letterY - 1] = TILE_BUSH_L;
    StageCache_C64.TileNumber[stg][27][letterY - 1] = TILE_BUSH_R;

    // ==========================================
    // 2. DIAMOND SPACERS & CHASM (x=30..44)
    // ==========================================
    StageCache_C64.TileNumber[stg][32][letterY + 2] = TILE_DIAMOND;
    StageCache_C64.TileNumber[stg][36][letterY + 2] = TILE_DIAMOND;
    StageCache_C64.TileNumber[stg][40][letterY + 2] = TILE_DIAMOND;

    // ==========================================
    // 3. "SISTERS" (x=46..84)
    // ==========================================

    // 'S': x=46..49 (Grey beveled blocks)
    DrawLetter(stg, 46, letterY, FONT_S, 4, TILE_BEVEL_BLOCK);

    // 'I': x=52 (Grey masonry brick column)
    for (int y = 0; y < 5; ++y) {
        StageCache_C64.TileNumber[stg][52][letterY + y] = TILE_BRICK_WALL;
    }

    // 'S': x=55..58 (Grey beveled blocks)
    DrawLetter(stg, 55, letterY, FONT_S, 4, TILE_BEVEL_BLOCK);

    // 'T': x=61..65 (Grey beveled blocks + bush on top)
    DrawLetter(stg, 61, letterY, FONT_T, 5, TILE_BEVEL_BLOCK);
    StageCache_C64.TileNumber[stg][62][letterY - 1] = TILE_BUSH_L;
    StageCache_C64.TileNumber[stg][63][letterY - 1] = TILE_BUSH_R;

    // 'E': x=68..71 (Grey brick wall + bush sitting in lower shelf)
    DrawLetter(stg, 68, letterY, FONT_E, 4, TILE_BRICK_WALL);
    StageCache_C64.TileNumber[stg][69][letterY + 3] = TILE_BUSH_L;
    StageCache_C64.TileNumber[stg][70][letterY + 3] = TILE_BUSH_R;

    // 'R': x=74..77 (Grey beveled blocks)
    DrawLetter(stg, 74, letterY, FONT_R, 4, TILE_BEVEL_BLOCK);

    // 'S': x=80..83 (Grey beveled blocks)
    DrawLetter(stg, 80, letterY, FONT_S, 4, TILE_BEVEL_BLOCK);

    // ==========================================
    // 4. TRAILING DIAMONDS & SPACERS (x=85..110)
    // ==========================================
    StageCache_C64.TileNumber[stg][86][letterY + 2] = TILE_DIAMOND;
    StageCache_C64.TileNumber[stg][89][letterY + 2] = TILE_DIAMOND;
    StageCache_C64.TileNumber[stg][92][letterY + 2] = TILE_DIAMOND;

    // 3x3 diamond formation at x=96..100
    for (int dr = 0; dr < 3; ++dr) {
        for (int dc = 0; dc < 3; ++dc) {
            StageCache_C64.TileNumber[stg][96 + dc * 2][letterY + 1 + dr] = TILE_DIAMOND;
        }
    }

    // ==========================================
    // 5. GROUND, BUSHES, MUSHROOMS & WATER (y=7..9)
    // ==========================================
    const int stageWidth = 140;

    for (int x = 0; x < stageWidth; ++x) {
        // Water chasms at x=34..38 and x=114..118
        if ((x >= 34 && x <= 38) || (x >= 114 && x <= 118)) {
            StageCache_C64.TileNumber[stg][x][8] = TILE_WATER_SURF;
            StageCache_C64.TileNumber[stg][x][9] = TILE_WATER_DEEP;
        } else {
            StageCache_C64.TileNumber[stg][x][8] = TILE_COBBLE_TOP;
            StageCache_C64.TileNumber[stg][x][9] = TILE_COBBLE_BOT;
        }
    }

    // Ground surface decorations at y=7:
    // Green Bushes:
    int bushPositions[] = { 1, 16, 28, 43, 58, 71, 84, 108, 124 };
    for (int bp : bushPositions) {
        if (bp + 1 < stageWidth && StageCache_C64.TileNumber[stg][bp][8] == TILE_COBBLE_TOP) {
            StageCache_C64.TileNumber[stg][bp][7] = TILE_BUSH_L;
            StageCache_C64.TileNumber[stg][bp + 1][7] = TILE_BUSH_R;
        }
    }

    // Red Spotted Mushrooms:
    int mushroomPositions[] = { 8, 23, 50, 66, 78, 93, 128 };
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
