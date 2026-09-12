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

    if(!Key_LALT && (Key_ENTER_pressed || Joy_OK_pressed) && Menu.ActiveOptions == 0) {
      if (GV.Resolution != RESOLUTION_320x170) SYSTEM_SetResolution(RESOLUTION_320x170);
      else SYSTEM_SetResolution(RESOLUTION_640x480);
      AUDIO_Sound_Play(AUDIO_CLICK);
    }
    if(!Key_LALT && (Key_ENTER_pressed || Joy_OK_pressed) && Menu.ActiveOptions == 1) {
      toggleScreenmode();
      AUDIO_Sound_Play(AUDIO_CLICK);
    }
    if(!Key_LALT && (Key_ENTER_pressed || Joy_OK_pressed) && Menu.ActiveOptions == 2) {
      AUDIO_Volume_Change_Music(VolumePercentage_Music += 10, true);
      AUDIO_Sound_Play(AUDIO_CLICK);
    }
    if(!Key_LALT && (Key_ENTER_pressed || Joy_OK_pressed) && Menu.ActiveOptions == 3) {
      AUDIO_Volume_Change_Music(VolumePercentage_Music -= 10, true);
      AUDIO_Sound_Play(AUDIO_CLICK);
    }
    if(!Key_LALT && (Key_ENTER_pressed || Joy_OK_pressed) && Menu.ActiveOptions == 4) {
      AUDIO_Volume_Change_Sound(VolumePercentage_Sound += 10, true);
      AUDIO_Sound_Play(AUDIO_CLICK);
    }
    if(!Key_LALT && (Key_ENTER_pressed || Joy_OK_pressed) && Menu.ActiveOptions == 5) {
      AUDIO_Volume_Change_Sound(VolumePercentage_Sound -= 10, true);
      AUDIO_Sound_Play(AUDIO_CLICK);
    }
    if(!Key_LALT && (Key_ENTER_pressed || Joy_OK_pressed) && Menu.ActiveOptions == 6) {
      GV.RandomLevels = !GV.RandomLevels;
      STAGES_Import();
      AUDIO_Sound_Play(AUDIO_CLICK);
    }
    if(!Key_LALT && (Key_ENTER_pressed || Joy_OK_pressed) && Menu.ActiveOptions == 7) {
      // Keymap / Help overlay
      AUDIO_Sound_Play(AUDIO_CLICK);
      bool inHelp = true;
      while(inHelp && !QuitProgram && !QuitToMenu) {
        getInput();
        INPUT_General();
        if(Key_ESCAPE_pressed || Key_ENTER_pressed || Joy_OK_pressed || Joy_ESCAPE_pressed) {
          inHelp = false;
        }
        drawGround(0);
        Print(10, 8, 0, 0, "CARDPUTER ZERO CONTROLS");
        Print(10, 24, 2, 0, "Move: Arrows or WASD");
        Print(10, 38, 2, 0, "Jump: SPACE, W, UP, or K");
        Print(10, 52, 2, 0, "Fire: ENTER, J, or RSHIFT");
        Print(10, 66, 2, 0, "Pause: P  Menu/Back: ESC");
        Print(10, 80, 2, 0, "Save/Load: 1 / 2");
        Print(10, 100, 3, 0, "Press ESC or ENTER to return");
        SDL_RenderPresent(gRenderer);
        SDL_Delay(30);
      }
      AUDIO_Sound_Play(AUDIO_CLICK);
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

  // Option 0: Resolution
  Colour = (GV.Resolution == RESOLUTION_320x170) ? 3 : 0;
  Print(Menu.x[0]+10, Menu.y[0]+10, Colour, 0, "Resolution");
  if (GV.Resolution == RESOLUTION_320x170) {
    Print(Menu.x[0]+10, Menu.y[0]+22, Colour, 0, "320x170");
    Print(Menu.x[0]+10, Menu.y[0]+34, 2, 0, "Cardputer");
  } else {
    Print(Menu.x[0]+10, Menu.y[0]+22, Colour, 0, "640x480");
    Print(Menu.x[0]+10, Menu.y[0]+34, 2, 0, "Desktop");
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
