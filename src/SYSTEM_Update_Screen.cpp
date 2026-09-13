#include "globals.h"
#include "SYSTEM_Power.h"
Uint32 timecounter_Update_Screen;
void Hide_Borders_Widescreen();

// ##############################################
// ##############################################
// ##############################################

void Update_Screen()
{
  Animation();
  if(SDL_GetTicks() - timecounter_Update_Screen > 10)
  {
    timecounter_Update_Screen = SDL_GetTicks();                 // reset time counter

    if(GV.Mode == MODE_MENU)           {drawGround(0); LOOP_Menu_Draw();}
    if(GV.Mode == MODE_GAMELOOP)
    {
      if(GV.Resolution == RESOLUTION_320x170 && GV.ViewMode == VIEW_MODE_C64_SCALED && gameplayTargetTexture != NULL)
      {
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
        show_DebugInfos();
      }
      else
      {
        STAGE_Draw();
        PC_Draw(PC.PosX, PC.PosY);
        ENEMIES_Draw();
        SPRITES_Draw();
        Hide_Borders_Widescreen();
        STAGE_Interface_Draw();
        show_DebugInfos();
      }
    }
    if(GV.Mode == MODE_EDITOR_STAGES)  {drawGround(0); LOOP_Editor_Stages_Draw_Stage(); LOOP_Editor_Stages_Draw_Enemies(); LOOP_Editor_Stages_Draw_Interface();}
    if(GV.Mode == MODE_EDITOR_TILES)   {drawGround(0); LOOP_Editor_Tiles_Draw(); LOOP_Editor_Tiles_Interface_Draw();}
    if(GV.Mode == MODE_CREDITS)        {drawGround(0); LOOP_Credits_Draw();}
    if(GV.Mode == MODE_OPTIONS)        {drawGround(0); LOOP_Options_Draw();}
    if(GV.Mode == MODE_STORY)          {drawGround(0); LOOP_Story_Draw();}
    if(GV.Mode == MODE_INTRO)
    {
      LOOP_Intro_Draw();
    }
    if(GV.Mode == MODE_SPLASHSCREEN)   {drawGround(1); LOOP_Gameloop_Splashscreen_Draw();}
    if(GV.Mode == MODE_WARPSCREEN)     {drawGround(1); LOOP_Gameloop_Warpscreen_Draw();}
    if(GV.Mode == MODE_STAGEFILE_LOAD) {drawGround(0); LOOP_Stagefile_Load_Draw();}

    OVERLAY_Volume_Draw_Music();
    OVERLAY_Volume_Draw_Sound();

    if (Power_IsBatteryLow()) {
      // Draw subtle low-battery blinking indicator in corner
      if ((SDL_GetTicks() / 500) % 2 == 0) {
        Print(2, 2, 2, 0, "!");
      }
    }

    SDL_RenderPresent(gRenderer);  //Update screen
    Power_FramePace(60);
  }
}

// ##############################################
// ##############################################
// ##############################################

void Hide_Borders_Widescreen()
{
  // HIDE BOTTOM & RIGHT BORDER IN WIDESCREEN-MODE
  int x, y;
  if(GV.Resolution == RESOLUTION_800x600)
  {
    for(y=StageC64.StageHeight; y<StageC64.StageHeight+8; y++)
    {
      for(x=0; x<GV.ScreenWidthTiles; x++)
      {
        TILE_Draw((x*TS.Tile_Width), y*TS.Tile_Height, 40);
      }
    }
  }

  if(GV.Resolution == RESOLUTION_1280x720)
  {
    for(y=StageC64.StageHeight; y<StageC64.StageHeight+15; y++)
    {
      for(x=0; x<GV.ScreenWidthTiles; x++)
      {
        TILE_Draw((x*TS.Tile_Width), y*TS.Tile_Height, 40);
      }
    }
  }
  // HIDE BOTTOM & RIGHT BORDER IN WIDESCREEN-MODE
}
