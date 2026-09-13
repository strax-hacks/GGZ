#include "globals.h"
#include "SYSTEM_Paths.h"
#include "SYSTEM_Power.h"
#include <SDL_image.h>
#include <iostream>

bool QuitProgram = false;
Config_Type Config;

int main(int argc, char* argv[]) {
    // Dummy video driver to allow rendering headless offscreen
    setenv("SDL_VIDEODRIVER", "dummy", 1);
    setenv("SDL_AUDIODRIVER", "dummy", 1);

    GAME_ENVIRONMENT_Define();
    Options_Load();
    init();
    defineAngles();
    Power_Init();

    Option_GameType_Load();
    GAMETYPE_Load();

    // Setup Menu for 320x170
    Menu.MenuEntries = (GV.Resolution == RESOLUTION_320x170) ? 6 : 10;
    LOOP_Menu_Define_Positions((GV.Resolution == RESOLUTION_320x170) ? 2 : 5);
    Menu.Active = 0;

    // Render Menu Frame
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);
    LOOP_Menu_Draw();
    SDL_RenderPresent(gRenderer);

    SDL_Surface* sshot = SDL_CreateRGBSurfaceWithFormat(0, 320, 170, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_RenderReadPixels(gRenderer, NULL, SDL_PIXELFORMAT_RGBA32, sshot->pixels, sshot->pitch);
    IMG_SavePNG(sshot, "/tmp/menu_320x170.png");
    SDL_FreeSurface(sshot);

    // Render Options Frame
    Menu.MenuEntries = 8;
    Menu.Active = 0;

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);
    LOOP_Options_Draw();
    SDL_RenderPresent(gRenderer);

    sshot = SDL_CreateRGBSurfaceWithFormat(0, 320, 170, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_RenderReadPixels(gRenderer, NULL, SDL_PIXELFORMAT_RGBA32, sshot->pixels, sshot->pitch);
    IMG_SavePNG(sshot, "/tmp/options_320x170.png");
    SDL_FreeSurface(sshot);

    // Render Story Frame
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);
    LOOP_Story_Draw();
    SDL_RenderPresent(gRenderer);

    sshot = SDL_CreateRGBSurfaceWithFormat(0, 320, 170, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_RenderReadPixels(gRenderer, NULL, SDL_PIXELFORMAT_RGBA32, sshot->pixels, sshot->pitch);
    IMG_SavePNG(sshot, "/tmp/story_320x170.png");
    SDL_FreeSurface(sshot);

    // Render Intro Screen Frame (Stage 0 DS Panorama Layout)
    GV.Mode = MODE_INTRO;
    GV.Resolution = RESOLUTION_320x170;
    STAGE_Load(0, 0, false, false);
    
    // Test multiple scroll offsets to capture GIANA and SISTERS
    int scrollOffsets[] = { 0, 150, 450, 850, 1300 };
    for (int i = 0; i < 5; ++i) {
        PC.StagePosX = scrollOffsets[i];
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        SDL_RenderClear(gRenderer);
        LOOP_Intro_Draw();
        SDL_RenderPresent(gRenderer);

        char filename[64];
        snprintf(filename, sizeof(filename), "/tmp/intro_scroll_%d.png", scrollOffsets[i]);
        sshot = SDL_CreateRGBSurfaceWithFormat(0, 320, 170, 32, SDL_PIXELFORMAT_RGBA32);
        SDL_RenderReadPixels(gRenderer, NULL, SDL_PIXELFORMAT_RGBA32, sshot->pixels, sshot->pitch);
        IMG_SavePNG(sshot, filename);
        if (i == 0) {
            IMG_SavePNG(sshot, "/tmp/intro_320x170.png");
        }
        SDL_FreeSurface(sshot);
    }

    // Render Gameplay Frame
    GV.Mode = MODE_GAMELOOP;
    STAGE_Load(1, 0, false, false);
    std::cout << "[DEBUG] PC.PosX=" << PC.PosX << ", PC.PosY=" << PC.PosY << std::endl;
    // Inspect stages
    for(int s=1; s<=5; s++) {
        STAGE_Load(s, 0, false, false);
        int minY = 99, maxY = -1;
        for(int y=0; y<30; y++) {
            for(int x=0; x<StageC64.StageWidth; x++) {
                if (StageC64.TileNumber[x][y] != 0 && StageC64.TileNumber[x][y] != 41) {
                    if (y < minY) minY = y;
                    if (y > maxY) maxY = y;
                }
            }
        }
        std::cout << "[STAGE " << s << "] StartPos=(" << PC.PosX << "," << PC.PosY << ") StageWidth=" << StageC64.StageWidth << " tileY range: [" << minY << ", " << maxY << "]" << std::endl;
    }
    // Load Stage 1 for Gameplay Frame (C64 Scaled View Mode)
    GV.ViewMode = VIEW_MODE_C64_SCALED;
    STAGE_Load(1, 0, false, false);
    // Advance PC into Stage 1 to showcase floating mystery blocks and ground simultaneously
    PC.PosX = 500;
    SDL_SetRenderTarget(gRenderer, gameplayTargetTexture);
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);
    STAGE_Draw();
    PC_Draw(PC.PosX, PC.PosY);
    ENEMIES_Draw();
    SPRITES_Draw();
    SDL_SetRenderTarget(gRenderer, NULL);
    SDL_Rect srcQuad = { 0, 0, 640, 340 };
    SDL_Rect dstQuad = { 0, 0, 320, 170 };
    SDL_RenderCopy(gRenderer, gameplayTargetTexture, &srcQuad, &dstQuad);
    STAGE_Interface_Draw();
    SDL_RenderPresent(gRenderer);

    sshot = SDL_CreateRGBSurfaceWithFormat(0, 320, 170, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_RenderReadPixels(gRenderer, NULL, SDL_PIXELFORMAT_RGBA32, sshot->pixels, sshot->pitch);
    IMG_SavePNG(sshot, "/tmp/gameplay_c64_320x170.png");
    IMG_SavePNG(sshot, "/tmp/gameplay_320x170.png");
    SDL_FreeSurface(sshot);

    // Load Stage 1 for Gameplay Frame (1:1 Zoomed View Mode)
    GV.ViewMode = VIEW_MODE_1X_ZOOMED;
    STAGE_Load(1, 0, false, false);
    PC.PosX = 500;
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);
    STAGE_Draw();
    PC_Draw(PC.PosX, PC.PosY);
    ENEMIES_Draw();
    SPRITES_Draw();
    STAGE_Interface_Draw();
    SDL_RenderPresent(gRenderer);

    sshot = SDL_CreateRGBSurfaceWithFormat(0, 320, 170, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_RenderReadPixels(gRenderer, NULL, SDL_PIXELFORMAT_RGBA32, sshot->pixels, sshot->pitch);
    IMG_SavePNG(sshot, "/tmp/gameplay_1x_320x170.png");
    SDL_FreeSurface(sshot);

    // Load Stage 2 for Gameplay Frame (C64 Scaled View Mode)
    GV.ViewMode = VIEW_MODE_C64_SCALED;
    STAGE_Load(2, 0, false, false);
    SDL_SetRenderTarget(gRenderer, gameplayTargetTexture);
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);
    STAGE_Draw();
    PC_Draw(PC.PosX, PC.PosY);
    ENEMIES_Draw();
    SPRITES_Draw();
    SDL_SetRenderTarget(gRenderer, NULL);
    SDL_RenderCopy(gRenderer, gameplayTargetTexture, &srcQuad, &dstQuad);
    STAGE_Interface_Draw();
    SDL_RenderPresent(gRenderer);

    sshot = SDL_CreateRGBSurfaceWithFormat(0, 320, 170, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_RenderReadPixels(gRenderer, NULL, SDL_PIXELFORMAT_RGBA32, sshot->pixels, sshot->pitch);
    IMG_SavePNG(sshot, "/tmp/gameplay_stage2_320x170.png");
    SDL_FreeSurface(sshot);

    std::cout << "[CAPTURE] Captured menu_320x170.png, options_320x170.png, story_320x170.png, gameplay_c64_320x170.png, gameplay_1x_320x170.png, gameplay_stage2_320x170.png" << std::endl;

    Game_Close();
    return 0;
}
