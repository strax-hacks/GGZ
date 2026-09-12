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

    std::cout << "[SMOKE TEST] Verification complete. Shutting down." << std::endl;
    Game_Close();
    return 0;
}
