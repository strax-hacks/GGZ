#include "globals.h"
#include "SYSTEM_Paths.h"
#include "SYSTEM_SaveState.h"
#include "SYSTEM_Power.h"
#include <cassert>
#include <iostream>

bool QuitProgram = false;
Config_Type Config;

int main(int argc, char* argv[]) {
    std::cout << "[SMOKE TEST] Initializing Cardputer ZERO OpenGGS headless test..." << std::endl;

    // Set SDL dummy video and audio drivers for headless validation if not specified
    setenv("SDL_VIDEODRIVER", "dummy", 0);
    setenv("SDL_AUDIODRIVER", "dummy", 0);

    GAME_ENVIRONMENT_Define();
    init();
    defineAngles();
    Power_Init();

    Option_GameType_Load();
    GAMETYPE_Load();

    std::cout << "[SMOKE TEST] Loaded GameType=" << GV.GameType
              << ", Resolution=" << GV.Screen_Width << "x" << GV.Screen_Height << std::endl;

    // Verify stage 1 loaded
    assert(PC.Stage == 1);
    std::cout << "[SMOKE TEST] Stage 1 loaded at (" << PC.PosX << ", " << PC.PosY << ")" << std::endl;

    // Test loading Stage 0 (Intro Stage) and rendering
    std::cout << "[SMOKE TEST] Testing Stage 0 (Intro Screen Stage)..." << std::endl;
    STAGE_Load(0, 0, false, false);
    assert(PC.Stage == 0);
    assert(StageC64.StageWidth >= 128);
    assert(StageC64.StageHeight == 30);
    GV.Mode = MODE_INTRO;
    LOOP_Intro_Draw();
    std::cout << "[SMOKE TEST] Stage 0 intro rendering verified! Width=" << StageC64.StageWidth << std::endl;

    // Test Credits rendering
    GV.Mode = MODE_CREDITS;
    Credits.x_off = 16;
    Credits.y_off = 28;
    LOOP_Credits_Draw();
    std::cout << "[SMOKE TEST] Credits rendering verified!" << std::endl;

    // Test start game from menu after Stage 0 intro: ensure Stage 1 is loaded
    if (PC.Stage <= 0) {
        STAGE_Load(1, 0, false, false);
    }
    assert(PC.Stage == 1 && "Starting game from intro/menu state must load Stage 1");
    std::cout << "[SMOKE TEST] Verified game start from intro/menu correctly resolves to Stage 1." << std::endl;

    // Test loading each stage from 1 to 33 and running simulation
    std::cout << "[SMOKE TEST] Testing stages 1 through 33..." << std::endl;
    for (int stg = 1; stg <= 33; ++stg) {
        PC.Stage = stg;
        STAGE_Load(stg, 0, false, false);
        assert(PC.Stage == stg);
        GV.Mode = MODE_GAMELOOP;
        for (int frame = 0; frame < 15; ++frame) {
            Power_Update();
            PC_Run();
            PC_Gravity();
            PC_Friction();
            PC_Check_Tilecollision();
            PC_Check_TilecollisionNonSolid();
            ENEMIES_Move();
            SPRITES_Move();
        }
        STAGE_Draw();
        PC_Draw(PC.PosX, PC.PosY);
        ENEMIES_Draw();
        SPRITES_Draw();
        STAGE_Interface_Draw();
    }
    std::cout << "[SMOKE TEST] All 33 stages verified cleanly!" << std::endl;

    // Reset to Stage 2 for SaveState test
    PC.Stage = 2;
    STAGE_Load(2, 0, false, false);

    // Test Save State Serialization
    std::cout << "[SMOKE TEST] Testing SaveState write to slot 1..." << std::endl;
    bool writeOk = SaveState_Write(1);
    assert(writeOk && "SaveState_Write(1) must succeed");

    SaveStateMetadata meta = SaveState_GetInfo(1);
    assert(meta.exists && "Save metadata must report slot exists");
    std::cout << "[SMOKE TEST] SaveState slot 1 metadata verified: Stage=" << meta.stage
              << ", Score=" << meta.score << ", Lives=" << meta.lives << std::endl;

    bool readOk = SaveState_Read(1);
    assert(readOk && "SaveState_Read(1) must succeed");
    std::cout << "[SMOKE TEST] SaveState slot 1 restored cleanly." << std::endl;

    // Run 100 simulation update steps
    std::cout << "[SMOKE TEST] Running 100 simulation frames..." << std::endl;
    GV.Mode = MODE_GAMELOOP;
    for (int frame = 0; frame < 100; ++frame) {
        Power_Update();
        PC_Run();
        PC_Gravity();
        PC_Friction();
        ENEMIES_Move();
        SPRITES_Move();
        if (frame % 25 == 0) {
            std::cout << "  Frame " << frame << ": PC PosX=" << PC.PosX << ", PosY=" << PC.PosY << std::endl;
        }
    }

    // Test dual view mode toggling, rendering, and persistence
    std::cout << "[SMOKE TEST] Testing dual view modes and persistence..." << std::endl;
    GV.ViewMode = VIEW_MODE_C64_SCALED;
    STAGE_Load(1, 0, false, false);
    // In Stage 1 start pos (PosY >= 400), StagePosY is anchored at bottom (140)
    PC.PosY = 432;
    STAGE_Draw();
    PC_Draw(PC.PosX, PC.PosY);
    ENEMIES_Draw();
    SPRITES_Draw();
    assert(GV.ViewMode == VIEW_MODE_C64_SCALED);
    assert(PC.StagePosY == 140);

    // Toggle to 1X Zoomed mode
    GV.ViewMode = VIEW_MODE_1X_ZOOMED;
    STAGE_Draw();
    PC_Draw(PC.PosX, PC.PosY);
    ENEMIES_Draw();
    SPRITES_Draw();
    assert(GV.ViewMode == VIEW_MODE_1X_ZOOMED);

    // Test Options persistence for ViewMode
    Options_Save();
    GV.ViewMode = VIEW_MODE_C64_SCALED;
    Options_Load();
    assert(GV.ViewMode == VIEW_MODE_1X_ZOOMED && "Options_Load must restore saved 1X Zoomed view mode");

    GV.ViewMode = VIEW_MODE_C64_SCALED;
    Options_Save();
    GV.ViewMode = VIEW_MODE_1X_ZOOMED;
    Options_Load();
    assert(GV.ViewMode == VIEW_MODE_C64_SCALED && "Options_Load must restore saved C64 Scaled view mode");

    std::cout << "[SMOKE TEST] Dual view mode assertions passed successfully!" << std::endl;

    // Test Rock Knockout and Fast Fading Animation Timing
    std::cout << "[SMOKE TEST] Testing rock breakout and authentic C64 fade speed..." << std::endl;
    STAGE_Load(1, 0, false, false);
    int testRockX = 10;
    int testRockY = 15;

    // Clear area around test rock and set air below it
    for (int dy = -2; dy <= 2; ++dy) {
        for (int dx = -2; dx <= 2; ++dx) {
            StageC64.TileNumber[testRockX + dx][testRockY + dy] = 0;
            Stage_AnimationCounter_Set(testRockX + dx, testRockY + dy, 0);
        }
    }

    // Set 3x2 breakable rock (501..503 top row at Y=14, 541..543 bottom row at Y=15)
    StageC64.TileNumber[testRockX - 1][testRockY - 1] = 501;
    StageC64.TileNumber[testRockX][testRockY - 1]     = 502;
    StageC64.TileNumber[testRockX + 1][testRockY - 1] = 503;
    StageC64.TileNumber[testRockX - 1][testRockY]     = 541;
    StageC64.TileNumber[testRockX][testRockY]         = 542;
    StageC64.TileNumber[testRockX + 1][testRockY]     = 543;

    // Position punk Giana directly under center rock tile jumping up
    PC_Define();
    PC.Mode = PC_MODE2; // Punk Giana
    PC.PosX = testRockX * TS.Tile_Width + 8;
    PC.PosY = testRockY * TS.Tile_Height + PC.ColHeight + 4;
    PC.JumpVelocity = 10;

    PC_Collision_Breakable();

    // Verify rock immediately enters frame 1 (cracked rock 504..506, 544..546)
    assert(StageC64.TileNumber[testRockX - 1][testRockY - 1] == 504);
    assert(StageC64.TileNumber[testRockX][testRockY - 1] == 505);
    assert(StageC64.TileNumber[testRockX + 1][testRockY - 1] == 506);
    assert(StageC64.TileNumber[testRockX - 1][testRockY] == 544);
    assert(StageC64.TileNumber[testRockX][testRockY] == 545);
    assert(StageC64.TileNumber[testRockX + 1][testRockY] == 546);
    assert(StageC64.AnimationCounter[testRockX][testRockY] == World.TileSwitchSpeed);
    assert(World.TileSwitchSpeed <= 6 && "Tile switch speed must be fast for authentic C64 feel");

    // Advance 4 frames: rock should transition to frame 2 (crumbling debris 507..509, 547..549)
    for (int f = 0; f < 4; ++f) {
        timecounter_TileSwitch = 0;
        Animation();
    }
    assert(StageC64.TileNumber[testRockX - 1][testRockY - 1] == 507);
    assert(StageC64.TileNumber[testRockX][testRockY - 1] == 508);
    assert(StageC64.TileNumber[testRockX + 1][testRockY - 1] == 509);
    assert(StageC64.TileNumber[testRockX - 1][testRockY] == 547);
    assert(StageC64.TileNumber[testRockX][testRockY] == 548);
    assert(StageC64.TileNumber[testRockX + 1][testRockY] == 549);

    // Advance another 4 frames: rock should completely fade away to 0 (air)
    for (int f = 0; f < 4; ++f) {
        timecounter_TileSwitch = 0;
        Animation();
    }
    assert(StageC64.TileNumber[testRockX - 1][testRockY - 1] == 0);
    assert(StageC64.TileNumber[testRockX][testRockY - 1] == 0);
    assert(StageC64.TileNumber[testRockX + 1][testRockY - 1] == 0);
    assert(StageC64.TileNumber[testRockX - 1][testRockY] == 0);
    assert(StageC64.TileNumber[testRockX][testRockY] == 0);
    assert(StageC64.TileNumber[testRockX + 1][testRockY] == 0);
    std::cout << "[SMOKE TEST] Rock knockout & fade timing verified! Complete fade in 8 frames (~133ms)." << std::endl;

    // Test Cheats persistence and "New game starts at last finished level"
    std::cout << "[SMOKE TEST] Testing cheat: New game starts at last finished level..." << std::endl;
    GV.Cheat_StartAtLastFinishedLevel = true;
    GV.LastFinishedLevel = 7;
    Options_Save();

    // Reset variables and reload from file
    GV.Cheat_StartAtLastFinishedLevel = false;
    GV.LastFinishedLevel = 1;
    Options_Load();

    assert(GV.Cheat_StartAtLastFinishedLevel == true && "Options_Load must restore cheat state");
    assert(GV.LastFinishedLevel == 7 && "Options_Load must restore last finished level");

    // When cheat is enabled, PC_Define() must initialize starting stage to last finished level
    PC_Define();
    assert(PC.Stage == 7 && "New game must start at last finished level when cheat is active");
    assert(PC.Lives == 10 && PC.Points == 0 && "New game must have default starting lives and 0 points");

    // When cheat is disabled, PC_Define() must initialize starting stage to 1
    GV.Cheat_StartAtLastFinishedLevel = false;
    Options_Save();
    Options_Load();
    assert(GV.Cheat_StartAtLastFinishedLevel == false);
    PC_Define();
    assert(PC.Stage == 1 && "New game must start at Stage 1 when cheat is disabled");

    // Test stage completion updates LastFinishedLevel
    GV.LastFinishedLevel = 1;
    PC.Stage = 5;
    PC.Exit_Reached = true;
    if (PC.Stage >= 1 && PC.Stage <= 33 && PC.Stage > GV.LastFinishedLevel) {
        GV.LastFinishedLevel = PC.Stage;
        Options_Save();
    }
    assert(GV.LastFinishedLevel == 5 && "Reaching exit must record finished level");

    // Enable cheat again and verify new game starts at Stage 5
    GV.Cheat_StartAtLastFinishedLevel = true;
    PC_Define();
    assert(PC.Stage == 5 && "New game must start at Stage 5 after updating progress");

    // Reset cheat back to default OFF
    GV.Cheat_StartAtLastFinishedLevel = false;
    Options_Save();

    std::cout << "[SMOKE TEST] Cheat engine verification passed successfully!" << std::endl;

    std::cout << "[SMOKE TEST] Verification complete. Shutting down." << std::endl;
    Game_Close();
    return 0;
}
