#include "globals.h"

Options_Def Options;

// ##############################################
// ##############################################
// ##############################################

void LOOP_Options()
{
  int GameModeTemp = GV.Mode;
  GV.Mode = MODE_OPTIONS;
  Menu.Active = 0;

  Menu.MenuEntriesOptions = 8;
  Menu.ActiveOptions = 0;

  Options.x_off = 20;
  Options.y_off = 20;

  while(!QuitProgram && !QuitToMenu)
  {
    getInput();
    INPUT_General();

    Menu.Active = Menu.ActiveOptions;
    Menu.MenuEntries = Menu.MenuEntriesOptions;

    if(Key_ESCAPE_pressed || Joy_ESCAPE_pressed){QuitToMenu = true;}

    if(Key_RIGHT_pressed || Joy_RIGHT_pressed)
    {
      AUDIO_Sound_Play(AUDIO_CLICK);
      Menu.ActiveOptions += 1;
      if(Menu.ActiveOptions > Menu.MenuEntriesOptions-1){Menu.ActiveOptions = Menu.MenuEntriesOptions-1;}
    }

    if(Key_LEFT_pressed || Joy_LEFT_pressed)
    {
      AUDIO_Sound_Play(AUDIO_CLICK);
      Menu.ActiveOptions -= 1;
      if(Menu.ActiveOptions < 0){Menu.ActiveOptions = 0;}
    }

    if(Key_DOWN_pressed || Joy_DOWN_pressed)
    {
      AUDIO_Sound_Play(AUDIO_CLICK);
      if(Menu.ActiveOptions < Menu.MenuEntriesOptions-Menu.Cols){Menu.ActiveOptions = Menu.ActiveOptions + Menu.Cols;}
    }

    if(Key_UP_pressed || Joy_UP_pressed)
    {
      AUDIO_Sound_Play(AUDIO_CLICK);
      if(Menu.ActiveOptions > Menu.Cols-1){Menu.ActiveOptions = Menu.ActiveOptions - Menu.Cols;}
    }

    if(!Key_LALT && (Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveOptions == 0) {
      if (GV.Resolution == RESOLUTION_320x170) {
        GV.ViewMode = (GV.ViewMode == VIEW_MODE_C64_SCALED) ? VIEW_MODE_1X_ZOOMED : VIEW_MODE_C64_SCALED;
      } else {
        SYSTEM_SetResolution(RESOLUTION_320x170);
      }
      AUDIO_Sound_Play(AUDIO_CLICK);
    }
    if(!Key_LALT && (Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveOptions == 1) {
      toggleScreenmode();
      AUDIO_Sound_Play(AUDIO_CLICK);
    }
    if(!Key_LALT && (Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveOptions == 2) {
      AUDIO_Volume_Change_Music(VolumePercentage_Music += 10, true);
      AUDIO_Sound_Play(AUDIO_CLICK);
    }
    if(!Key_LALT && (Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveOptions == 3) {
      AUDIO_Volume_Change_Music(VolumePercentage_Music -= 10, true);
      AUDIO_Sound_Play(AUDIO_CLICK);
    }
    if(!Key_LALT && (Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveOptions == 4) {
      AUDIO_Volume_Change_Sound(VolumePercentage_Sound += 10, true);
      AUDIO_Sound_Play(AUDIO_CLICK);
    }
    if(!Key_LALT && (Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveOptions == 5) {
      AUDIO_Volume_Change_Sound(VolumePercentage_Sound -= 10, true);
      AUDIO_Sound_Play(AUDIO_CLICK);
    }
    if(!Key_LALT && (Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveOptions == 6) {
      GV.RandomLevels = !GV.RandomLevels;
      STAGES_Import();
      AUDIO_Sound_Play(AUDIO_CLICK);
    }
    if(!Key_LALT && (Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveOptions == 7) {
      LOOP_Controls_Help();
    }

    Update_Screen();
  }
  GV.Mode = GameModeTemp;
  Options_Save();
  getInput();
}

// ##############################################
// ##############################################
// ##############################################

void LOOP_Options_Draw()
{
  int x;
  int Colour = 0;
  SDL_Rect SrcRect;
  SDL_Rect DstRect;

  if (MenuBackgroundTexture)
  {
    SDL_Rect bgDst = { 0, 0, GV.Screen_Width, GV.Screen_Height };
    SDL_RenderCopy(gRenderer, MenuBackgroundTexture, NULL, &bgDst);
  }

  if (GV.Resolution == RESOLUTION_320x170)
  {
    Print((320 - 18 * 8) / 2, 8, 3, 0, "SETTINGS / OPTIONS");

    int optCols = 2;
    int optStartX = 14;
    int optStartY = 26;
    int optSpacingX = 152;
    int optSpacingY = 26;

    for(x = 0; x < Menu.MenuEntries; x++)
    {
      int r = x / optCols;
      int c = x % optCols;
      int bx = optStartX + c * optSpacingX;
      int by = optStartY + r * optSpacingY;

      SrcRect.x = (x == Menu.Active) ? 128 : 0;
      SrcRect.y = 208;
      SrcRect.w = 128;
      SrcRect.h = 32;
      DstRect.x = bx;
      DstRect.y = by;
      DstRect.w = 140;
      DstRect.h = 20;
      SDL_RenderCopyEx(gRenderer, InterfaceTexture, &SrcRect, &DstRect, 0, NULL, SDL_FLIP_NONE);

      char optBuf[64];
      if (x == 0) {
        snprintf(optBuf, sizeof(optBuf), "VIEW: %s", (GV.ViewMode == VIEW_MODE_C64_SCALED) ? "C64 0.5X" : "1:1 ZOOM");
      } else if (x == 1) {
        snprintf(optBuf, sizeof(optBuf), "SCREEN: %s", (CurrentScreenMode == 1) ? "FULL" : "WINDOW");
      } else if (x == 2) {
        snprintf(optBuf, sizeof(optBuf), "MUSIC +: %d%%", VolumePercentage_Music);
      } else if (x == 3) {
        snprintf(optBuf, sizeof(optBuf), "MUSIC -: %d%%", VolumePercentage_Music);
      } else if (x == 4) {
        snprintf(optBuf, sizeof(optBuf), "SOUND +: %d%%", VolumePercentage_Sound);
      } else if (x == 5) {
        snprintf(optBuf, sizeof(optBuf), "SOUND -: %d%%", VolumePercentage_Sound);
      } else if (x == 6) {
        snprintf(optBuf, sizeof(optBuf), "RANDOM: %s", GV.RandomLevels ? "ON" : "OFF");
      } else if (x == 7) {
        snprintf(optBuf, sizeof(optBuf), "KEYMAP / HELP");
      }

      int textLen = (int)strlen(optBuf);
      int textX = bx + (140 - textLen * 8) / 2;
      int textY = by + (20 - 8) / 2;

      Print(textX, textY, 0, 0, optBuf);

      if (x == Menu.Active) {
        Print(bx + 4, textY, 2, 0, ">");
        Print(bx + 140 - 12, textY, 2, 0, "<");
      }
    }

    Print((320 - 32 * 8) / 2, 146, 2, 0, "ARROWS:SELECT  ENTER:OK ESC:BACK");
  }
  else
  {
    // Desktop layout (640x480 / 800x600 / 1280x720)
    SrcRect.x = 0;
    SrcRect.y = 208;
    SrcRect.w = 128;
    SrcRect.h = 96;
    DstRect.w = SrcRect.w;
    DstRect.h = SrcRect.h;

    for(x=0; x<Menu.MenuEntries; x++)
    {
      DstRect.x = Menu.x[x];
      DstRect.y = Menu.y[x];
      SDL_RenderCopyEx(gRenderer, InterfaceTexture, &SrcRect, &DstRect, 0, NULL, SDL_FLIP_NONE );
    }

    SrcRect.x = 128;
    DstRect.x = Menu.x[Menu.Active];
    DstRect.y = Menu.y[Menu.Active];
    SDL_RenderCopyEx(gRenderer, InterfaceTexture, &SrcRect, &DstRect, 0, NULL, SDL_FLIP_NONE );

    // Option 0: View Mode / Resolution
    Colour = (Menu.Active == 0) ? 3 : 0;
    Print(Menu.x[0]+10, Menu.y[0]+10, Colour, 0, "View Mode");
    if (GV.ViewMode == VIEW_MODE_C64_SCALED) {
      Print(Menu.x[0]+10, Menu.y[0]+22, 3, 0, "C64 0.5X");
      Print(Menu.x[0]+10, Menu.y[0]+34, 2, 0, "Full Stage");
    } else {
      Print(Menu.x[0]+10, Menu.y[0]+22, 3, 0, "1:1 ZOOM");
      Print(Menu.x[0]+10, Menu.y[0]+34, 2, 0, "High Detail");
    }

    // Option 1: Display Mode
    Colour = 0;
    Print(Menu.x[1]+10, Menu.y[1]+10, Colour, 0, "Windowed/");
    Print(Menu.x[1]+10, Menu.y[1]+22, Colour, 0, "Fullscreen");

    // Option 2: Music Vol ++
    Print(Menu.x[2]+10, Menu.y[2]+10, Colour, 0, "Music ++");
    PrintInt(Menu.x[2]+10, Menu.y[2]+22, 3, 0, VolumePercentage_Music);

    // Option 3: Music Vol --
    Print(Menu.x[3]+10, Menu.y[3]+10, Colour, 0, "Music --");
    PrintInt(Menu.x[3]+10, Menu.y[3]+22, 3, 0, VolumePercentage_Music);

    // Option 4: Sound Vol ++
    Print(Menu.x[4]+10, Menu.y[4]+10, Colour, 0, "Sound ++");
    PrintInt(Menu.x[4]+10, Menu.y[4]+22, 3, 0, VolumePercentage_Sound);

    // Option 5: Sound Vol --
    Print(Menu.x[5]+10, Menu.y[5]+10, Colour, 0, "Sound --");
    PrintInt(Menu.x[5]+10, Menu.y[5]+22, 3, 0, VolumePercentage_Sound);

    // Option 6: Random levels
    Print(Menu.x[6]+10, Menu.y[6]+10, Colour, 0, "Random");
    Print(Menu.x[6]+10, Menu.y[6]+22, 3, 0, GV.RandomLevels ? "ON" : "OFF");

    // Option 7: Keymap & Controls Help
    Print(Menu.x[7]+10, Menu.y[7]+10, 3, 0, "Keymap /");
    Print(Menu.x[7]+10, Menu.y[7]+22, 3, 0, "Controls");
  }
}
