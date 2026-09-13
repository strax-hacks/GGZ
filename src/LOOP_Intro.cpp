#include "globals.h"
#include "CONTENT_Intro_Stage0.h"

static float IntroScrollX = 0.0f;

void LOOP_Intro_Draw()
{
  int virtualWidth = GV.Screen_Width;
  int virtualHeight = GV.Screen_Height;

  int screenWTiles = (virtualWidth / 16);

  // Set camera for intro: Y offset 0 aligns rows 0..9 across the 160px height
  PC.StagePosY = 0;

  // Clear background to pure retro black
  SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
  SDL_RenderClear(gRenderer);

  // Draw Stage 0 tiles with seamless horizontal modulo wrapping
  if (StageC64.StageWidth > 0) {
    for (int y = 0; y < 10 && y < StageC64.StageHeight; ++y) {
      int drawY = (y * TS.Tile_Height);
      for (int x = (int)(PC.StagePosX / 16); x < (int)(PC.StagePosX / 16) + screenWTiles + 2; ++x) {
        int wrappedX = (x % StageC64.StageWidth + StageC64.StageWidth) % StageC64.StageWidth;
        if (StageC64.TileNumber[wrappedX][y] != 0) {
          TILE_Draw((x * TS.Tile_Width) - PC.StagePosX, drawY, StageC64.TileNumber[wrappedX][y]);
        }
      }
    }
  }

  // Render top retro C64 green banner text (Color 3 = Green)
  if (GV.Resolution == RESOLUTION_320x170) {
    Print((320 - 33 * 8) / 2, 0, 3, 0, "**** THE GREAT GIANA SISTERS ****");
    Print((320 - 25 * 8) / 2, 9, 3, 0, "SPACE TO PLAY  M FOR MENU");
    Print((320 - 29 * 8) / 2, 18, 3, 0, "(C) 1987 TIME WARP / SOFTGOLD");
  } else {
    Print(GV.Screen_Width / 2 - 160, 2, 3, 0, "**** THE GREAT GIANA SISTERS ****");
    Print(GV.Screen_Width / 2 - 135, 11, 3, 0, "SPACE TO PLAY    M FOR MENU");
    Print(GV.Screen_Width / 2 - 145, 20, 3, 0, "(C) 1987 TIME WARP / SOFTGOLD");
  }
}

void LOOP_Intro()
{
  QuitToMenu = false;
  GV.Mode = MODE_INTRO;
  
  // Load Stage 0 into runtime StageC64
  STAGE_Load(0, 0, false, false);
  
  // Start title theme
  AUDIO_Music_Play(MUSIC_MENU);

  IntroScrollX = 0.0f;

  int maxScroll = StageC64.StageWidthPixels;
  if (maxScroll <= 0) maxScroll = 140 * 16;

  while (!QuitProgram && !QuitToMenu)
  {
    getInput();
    INPUT_General();

    // Key handlers
    // SPACE, ENTER, 1, Gamepad OK -> Start Game
    if (Key_SPACE_pressed || Key_ENTER_pressed || Key_1_pressed || Joy_OK_pressed) {
      AUDIO_Sound_Play(AUDIO_CLICK);
      STAGE_Load(1, 0, false, false);
      LOOP_Gameloop_Standard();
      if (!QuitProgram) {
        // Return to Intro after finishing or quitting standard gameloop
        QuitToMenu = false;
        GV.Mode = MODE_INTRO;
        STAGE_Load(0, 0, false, false);
        AUDIO_Music_Play(MUSIC_MENU);
      }
    }

    // M, 2, ESC, Gamepad ESC/Select -> Enter Options / Main Menu
    if (currentKeyStates[SDL_SCANCODE_M] || Key_2_pressed || Key_ESCAPE_pressed || Joy_ESCAPE_pressed) {
      AUDIO_Sound_Play(AUDIO_CLICK);
      LOOP_Menu();
      if (!QuitProgram) {
        QuitToMenu = false;
        GV.Mode = MODE_INTRO;
        STAGE_Load(0, 0, false, false);
        AUDIO_Music_Play(MUSIC_MENU);
      }
    }

    // Autoscroll landscape smoothly across the screen at C64 retro pace (~2px/frame)
    IntroScrollX += 2.0f;
    if (IntroScrollX >= (float)maxScroll) {
      IntroScrollX = 0.0f; // Seamless loop back to left edge
    }
    PC.StagePosX = (int)IntroScrollX;

    // Update screen
    Update_Screen();
    SDL_Delay(16);
  }
}
