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

    // Set SDL dummy video and audio drivers for headless validation
    setenv("SDL_VIDEODRIVER", "dummy", 1);
    setenv("SDL_AUDIODRIVER", "dummy", 1);

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
    Options_Load();
    assert(GV.ViewMode == VIEW_MODE_C64_SCALED && "Options_Load must restore saved C64 Scaled view mode");

    std::cout << "[SMOKE TEST] Dual view mode assertions passed successfully!" << std::endl;

    std::cout << "[SMOKE TEST] Verification complete. Shutting down." << std::endl;
    Game_Close();
    return 0;
}
