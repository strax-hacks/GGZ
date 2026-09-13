#include "globals.h"

Uint32 menu_timecounter;
bool QuitFromMenu;
bool QuitToMenu;
Menu_Def Menu;

// ##############################################
// ##############################################
// ##############################################

void LOOP_Controls_Help()
{
  bool inHelp = true;
  AUDIO_Sound_Play(AUDIO_CLICK);
  while(inHelp && !QuitProgram && !QuitToMenu) {
    getInput();
    INPUT_General();
    if(Key_ESCAPE_pressed || Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed || Joy_ESCAPE_pressed) {
      inHelp = false;
    }
    drawGround(0);
    Print((320 - 23 * 8) / 2, 10, 3, 0, "CARDPUTER ZERO CONTROLS");
    Print(16, 28, 0, 0, "Move:    Arrows or WASD");
    Print(16, 42, 0, 0, "Jump:    SPACE, W, UP, or K");
    Print(16, 56, 0, 0, "Fire:    ENTER, J, or R-SHIFT");
    Print(16, 70, 0, 0, "View:    V (C64 0.5X / 1:1 Zoom)");
    Print(16, 84, 0, 0, "Pause:   P    Back/Menu: ESC");
    Print(16, 98, 0, 0, "Save:    1 (Slot 1)  Load: 2");
    Print(16, 112, 0, 0, "Gamepad: D-Pad / [A]Jump [B]Fire");
    Print((320 - 28 * 8) / 2, 146, 2, 0, "Press ESC or ENTER to return");
    SDL_RenderPresent(gRenderer);
    SDL_Delay(30);
  }
  AUDIO_Sound_Play(AUDIO_CLICK);
}

// ##############################################
// ##############################################
// ##############################################

void LOOP_Menu()
{
  QuitFromMenu = false;
  QuitToMenu = true;
  menu_timecounter = SDL_GetTicks();
  GV.Mode = MODE_MENU;
  
  if (GV.Resolution == RESOLUTION_320x170) {
    Menu.MenuEntriesMainMenu = 6;
  } else {
    Menu.MenuEntriesMainMenu = 10;
  }
  Menu.ActiveMainMenu = 0;

  SYSTEM_SetResolution(GV.Resolution);

  while(!QuitProgram)
  {
    getInput();
    INPUT_General();

    Menu.Active = Menu.ActiveMainMenu;
    Menu.MenuEntries = Menu.MenuEntriesMainMenu;

    if(Key_RIGHT_pressed || Joy_RIGHT_pressed)
    {
      if (Menu.ActiveMainMenu + 1 < Menu.MenuEntriesMainMenu) {
        Menu.ActiveMainMenu += 1;
        AUDIO_Sound_Play(AUDIO_CLICK);
      }
    }

    if(Key_LEFT_pressed || Joy_LEFT_pressed)
    {
      if (Menu.ActiveMainMenu - 1 >= 0) {
        Menu.ActiveMainMenu -= 1;
        AUDIO_Sound_Play(AUDIO_CLICK);
      }
    }

    if(Key_DOWN_pressed || Joy_DOWN_pressed)
    {
      if(Menu.ActiveMainMenu + Menu.Cols < Menu.MenuEntriesMainMenu) {
        Menu.ActiveMainMenu += Menu.Cols;
        AUDIO_Sound_Play(AUDIO_CLICK);
      }
    }

    if(Key_UP_pressed || Joy_UP_pressed)
    {
      if(Menu.ActiveMainMenu - Menu.Cols >= 0) {
        Menu.ActiveMainMenu -= Menu.Cols;
        AUDIO_Sound_Play(AUDIO_CLICK);
      }
    }

    if(Key_ESCAPE_pressed || Joy_ESCAPE_pressed) {
      QuitProgram = true;
    }

    if(!Key_LALT)
    {
      if(GV.Resolution == RESOLUTION_320x170)
      {
        if((Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveMainMenu == 0) { LOOP_Gameloop_Standard(); }
        if((Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveMainMenu == 1) { LOOP_Options(); }
        if((Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveMainMenu == 2) { LOOP_Controls_Help(); }
        if((Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveMainMenu == 3) { LOOP_Story(); }
        if((Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveMainMenu == 4) { LOOP_Credits(); }
        if((Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveMainMenu == 5) { QuitProgram = true; }
      }
      else
      {
        if((Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveMainMenu == 0) { LOOP_Gameloop_Standard(); }
        if((Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveMainMenu == 1) { GV.GameType = TYPE_C64; GAMETYPE_Load(); }
        if((Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveMainMenu == 2) { GV.GameType = TYPE_AMIGA; GAMETYPE_Load(); }
        if((Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveMainMenu == 3) { LOOP_Editor_Stages(); }
        if((Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveMainMenu == 4) { LOOP_Options(); }
        if((Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveMainMenu == 5) { LOOP_Controls_Help(); }
        if((Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveMainMenu == 6) { LOOP_Story(); }
        if((Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveMainMenu == 7) { LOOP_Stagefile_Load(); }
        if((Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveMainMenu == 8) { LOOP_Credits(); }
        if((Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed) && Menu.ActiveMainMenu == 9) { QuitProgram = true; }

        if(Key_F1_pressed){SYSTEM_SetResolution(RESOLUTION_640x480);}
        if(Key_F2_pressed){SYSTEM_SetResolution(RESOLUTION_800x600);}
        if(Key_F3_pressed){SYSTEM_SetResolution(RESOLUTION_1280x720);}
      }
    }
    if(QuitToMenu)
    {
      QuitToMenu = false;
      AUDIO_Music_Play(MUSIC_MENU);
    }
    Update_Screen();  // draw the scene
  }
  Options_Save(); // SAVE OPTIONS TO FILE
}

// ##############################################
// ##############################################
// ##############################################

void LOOP_Menu_Draw()
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
    // Render Cardputer ZERO 320x170 Native Menu
    Print((320 - 23 * 8) / 2, 14, 3, 0, "THE GREAT GIANA SISTERS");

    const char* label[6] = {
      (PC.Stage > 1 || PC.Points > 0) ? "RESUME" : "START",
      "OPTIONS",
      "CONTROLS",
      "STORY",
      "CREDITS",
      "QUIT"
    };

    for(x = 0; x < 6; x++)
    {
      // Giana brick button background (128x32 texture slice scaled to 120x22)
      SrcRect.x = (x == Menu.Active) ? 128 : 0;
      SrcRect.y = 208;
      SrcRect.w = 128;
      SrcRect.h = 32;
      DstRect.x = Menu.x[x];
      DstRect.y = Menu.y[x];
      DstRect.w = 120;
      DstRect.h = 22;
      SDL_RenderCopyEx(gRenderer, InterfaceTexture, &SrcRect, &DstRect, 0, NULL, SDL_FLIP_NONE);

      // Centered text inside the button
      int textLen = (int)strlen(label[x]);
      int textX = Menu.x[x] + (120 - textLen * 8) / 2;
      int textY = Menu.y[x] + (22 - 8) / 2;

      if (x == Menu.Active)
      {
        Print(textX, textY, 0, 0, label[x]);
        Print(Menu.x[x] + 8, textY, 2, 0, ">");
        Print(Menu.x[x] + 120 - 16, textY, 2, 0, "<");
      }
      else
      {
        Print(textX, textY, 0, 0, label[x]);
      }
    }

    Print((320 - 32 * 8) / 2, 146, 2, 0, "ARROWS:SELECT  ENTER:OK ESC:QUIT");
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

    for(x=0;x<Menu.MenuEntries;x++)
    {
      DstRect.x = Menu.x[x];
      DstRect.y = Menu.y[x];
      SDL_RenderCopyEx(gRenderer, InterfaceTexture, &SrcRect, &DstRect, 0, NULL, SDL_FLIP_NONE );
    }

    SrcRect.x = 128;
    DstRect.x = Menu.x[Menu.Active];
    DstRect.y = Menu.y[Menu.Active];
    SDL_RenderCopyEx(gRenderer, InterfaceTexture, &SrcRect, &DstRect, 0, NULL, SDL_FLIP_NONE );

    Colour = 0;
    SrcRect.x = 256;
    SrcRect.y = 240;
    SrcRect.w = 32;
    SrcRect.h = 32;
    DstRect.w = SrcRect.w;
    DstRect.h = SrcRect.h;
    DstRect.x = Menu.x[0]+86;
    DstRect.y = Menu.y[0]+56;
    SDL_RenderCopyEx(gRenderer, InterfaceTexture, &SrcRect, &DstRect, 0, NULL, SDL_FLIP_NONE );
    Print(Menu.x[0]+10, Menu.y[0]+10, Colour, 0, "Start /");
    Print(Menu.x[0]+10, Menu.y[0]+22, Colour, 0, "Resume");

    Colour = 2;
    if(GV.DirAmigaExists || GV.DirDefaultExists)
    {
      if(GV.GameType == TYPE_C64){Colour = 3;}
      Print(Menu.x[1]+10, Menu.y[1]+10, Colour, 0, "Load");
      Print(Menu.x[1]+10, Menu.y[1]+22, Colour, 0, "C64-Version");
    }

    Colour = 2;
    if(GV.DirAmigaExists)
    {
      if(GV.GameType == TYPE_AMIGA){Colour = 3;}
      Print(Menu.x[2]+10, Menu.y[2]+10, Colour, 0, "Load");
      Print(Menu.x[2]+10, Menu.y[2]+22, Colour, 0, "Amiga-");
      Print(Menu.x[2]+10, Menu.y[2]+34, Colour, 0, "Version");
    }

    Colour = 2;
    if(GV.DirDefaultExists)
    {
      if(GV.GameType == TYPE_OPENGGS){Colour = 3;}
      Print(Menu.x[3]+10, Menu.y[3]+10, Colour, 0, "Load");
      Print(Menu.x[3]+10, Menu.y[3]+22, Colour, 0, "OpenGGS-");
      Print(Menu.x[3]+10, Menu.y[3]+34, Colour, 0, "Version");
    }

    Colour = 0;
    SrcRect.x = 288;
    SrcRect.y = 272;
    SrcRect.w = 32;
    SrcRect.h = 32;
    DstRect.w = SrcRect.w;
    DstRect.h = SrcRect.h;
    DstRect.x = Menu.x[4]+86;
    DstRect.y = Menu.y[4]+56;
    SDL_RenderCopyEx(gRenderer, InterfaceTexture, &SrcRect, &DstRect, 0, NULL, SDL_FLIP_NONE );
    Print(Menu.x[4]+10, Menu.y[4]+10, Colour, 0, "Stage");
    Print(Menu.x[4]+10, Menu.y[4]+22, Colour, 0, "Editor");

    SrcRect.x = 288;
    SrcRect.y = 208;
    SrcRect.w = 32;
    SrcRect.h = 32;
    DstRect.w = SrcRect.w;
    DstRect.h = SrcRect.h;
    DstRect.x = Menu.x[5]+86;
    DstRect.y = Menu.y[5]+56;
    SDL_RenderCopyEx(gRenderer, InterfaceTexture, &SrcRect, &DstRect, 0, NULL, SDL_FLIP_NONE );
    Print(Menu.x[5]+10, Menu.y[5]+10, Colour, 0, "Options");

    SrcRect.x = 256;
    SrcRect.y = 272;
    SrcRect.w = 32;
    SrcRect.h = 32;
    DstRect.w = SrcRect.w;
    DstRect.h = SrcRect.h;
    DstRect.x = Menu.x[6]+86;
    DstRect.y = Menu.y[6]+56;
    SDL_RenderCopyEx(gRenderer, InterfaceTexture, &SrcRect, &DstRect, 0, NULL, SDL_FLIP_NONE );
    Print(Menu.x[6]+10, Menu.y[6]+10, Colour, 0, "Story");

    Colour = 0;
    SrcRect.x = 256;
    SrcRect.y = 208;
    SrcRect.w = 32;
    SrcRect.h = 32;
    DstRect.w = SrcRect.w;
    DstRect.h = SrcRect.h;
    DstRect.x = Menu.x[7]+86;
    DstRect.y = Menu.y[7]+56;
    SDL_RenderCopyEx(gRenderer, InterfaceTexture, &SrcRect, &DstRect, 0, NULL, SDL_FLIP_NONE );
    Print(Menu.x[7]+10, Menu.y[7]+10, Colour, 0, "Load");
    Print(Menu.x[7]+10, Menu.y[7]+22, Colour, 0, "Stagefile");

    Colour = 0;
    SrcRect.x = 320;
    SrcRect.y = 208;
    SrcRect.w = 32;
    SrcRect.h = 32;
    DstRect.w = SrcRect.w;
    DstRect.h = SrcRect.h;
    DstRect.x = Menu.x[8]+86;
    DstRect.y = Menu.y[8]+56;
    SDL_RenderCopyEx(gRenderer, InterfaceTexture, &SrcRect, &DstRect, 0, NULL, SDL_FLIP_NONE );
    Print(Menu.x[8]+10, Menu.y[8]+10, Colour, 0, "Credits");

    Colour = 0;
    SrcRect.x = 288;
    SrcRect.y = 240;
    SrcRect.w = 32;
    SrcRect.h = 32;
    DstRect.w = SrcRect.w;
    DstRect.h = SrcRect.h;
    DstRect.x = Menu.x[9]+86;
    DstRect.y = Menu.y[9]+56;
    SDL_RenderCopyEx(gRenderer, InterfaceTexture, &SrcRect, &DstRect, 0, NULL, SDL_FLIP_NONE );
    Print(Menu.x[9]+10, Menu.y[9]+10, Colour, 0, "Quit");

    Print(20, GV.Screen_Height-12, 0, 0, "PAGEUP/DOWN - Volume");
    Print(20, 10, 0, 0, "OpenGGS 2.0 (OpenGGS Maker)");
  }
}

// ##############################################
// ##############################################
// ##############################################

void LOOP_Menu_Define_Positions(int Cols)
{
  Menu.Cols = Cols;

  if (GV.Resolution == RESOLUTION_320x170)
  {
    Menu.Cols = 2;
    // Main Menu 2 cols x 3 rows (Giana brick buttons 120x22)
    Menu.x[0] = 26;  Menu.y[0] = 38;
    Menu.x[1] = 174; Menu.y[1] = 38;
    Menu.x[2] = 26;  Menu.y[2] = 68;
    Menu.x[3] = 174; Menu.y[3] = 68;
    Menu.x[4] = 26;  Menu.y[4] = 98;
    Menu.x[5] = 174; Menu.y[5] = 98;
    return;
  }

  int MenuStartPosX = 20;
  int MenuStartPosY = 40;

  if(Menu.Cols == 2)
  {
    Menu.x[0] = 16;
    Menu.y[0] = 30;
    Menu.x[1] = 168;
    Menu.y[1] = 30;
    Menu.x[2] = 16;
    Menu.y[2] = 95;
    Menu.x[3] = 168;
    Menu.y[3] = 95;
    Menu.x[4] = 16;
    Menu.y[4] = 160;
    Menu.x[5] = 168;
    Menu.y[5] = 160;
    Menu.x[6] = 16;
    Menu.y[6] = 225;
    Menu.x[7] = 168;
    Menu.y[7] = 225;
    Menu.x[8] = 16;
    Menu.y[8] = 290;
    Menu.x[9] = 168;
    Menu.y[9] = 290;
  }

  if(Menu.Cols == 5)
  {
    Menu.x[0] = MenuStartPosX;
    Menu.y[0] = MenuStartPosY;
    Menu.x[1] = Menu.x[0]+138;
    Menu.y[1] = Menu.y[0];
    Menu.x[2] = Menu.x[1]+138;
    Menu.y[2] = Menu.y[0];
    Menu.x[3] = Menu.x[2]+138;
    Menu.y[3] = Menu.y[0];
    Menu.x[4] = Menu.x[3]+138;
    Menu.y[4] = Menu.y[0];

    Menu.x[5] = MenuStartPosX;
    Menu.y[5] = Menu.y[0]+106;
    Menu.x[6] = Menu.x[5]+138;
    Menu.y[6] = Menu.y[5];
    Menu.x[7] = Menu.x[6]+138;
    Menu.y[7] = Menu.y[5];
    Menu.x[8] = Menu.x[7]+138;
    Menu.y[8] = Menu.y[5];
    Menu.x[9] = Menu.x[8]+138;
    Menu.y[9] = Menu.y[5];

    Menu.x[10] = MenuStartPosX;
    Menu.y[10] = Menu.y[5]+106;
    Menu.x[11] = Menu.x[10]+138;
    Menu.y[11] = Menu.y[10];
    Menu.x[12] = Menu.x[11]+138;
    Menu.y[12] = Menu.y[10];
    Menu.x[13] = Menu.x[12]+138;
    Menu.y[13] = Menu.y[10];
    Menu.x[14] = Menu.x[13]+138;
    Menu.y[14] = Menu.y[10];

    Menu.x[15] = MenuStartPosX;
    Menu.y[15] = Menu.y[10]+106;
  }

  if(Menu.Cols == 4)
  {
    Menu.x[0] = MenuStartPosX;
    Menu.y[0] = MenuStartPosY;
    Menu.x[1] = Menu.x[0]+138;
    Menu.y[1] = Menu.y[0];
    Menu.x[2] = Menu.x[1]+138;
    Menu.y[2] = Menu.y[0];
    Menu.x[3] = Menu.x[2]+138;
    Menu.y[3] = Menu.y[0];
    Menu.x[4] = MenuStartPosX;
    Menu.y[4] = Menu.y[0]+106;
    Menu.x[5] = Menu.x[4]+138;
    Menu.y[5] = Menu.y[4];
    Menu.x[6] = Menu.x[5]+138;
    Menu.y[6] = Menu.y[4];
    Menu.x[7] = Menu.x[6]+138;
    Menu.y[7] = Menu.y[4];
    Menu.x[8] = MenuStartPosX;
    Menu.y[8] = Menu.y[4]+106;
    Menu.x[9] = Menu.x[8]+138;
    Menu.y[9] = Menu.y[8];
    Menu.x[10] = Menu.x[9]+138;
    Menu.y[10] = Menu.y[8];
    Menu.x[11] = Menu.x[10]+138;
    Menu.y[11] = Menu.y[8];

    Menu.x[12] = MenuStartPosX;
    Menu.y[12] = Menu.y[8]+106;
    Menu.x[13] = Menu.x[12]+138;
    Menu.y[13] = Menu.y[12];
    Menu.x[14] = Menu.x[13]+138;
    Menu.y[14] = Menu.y[12];
    Menu.x[15] = Menu.x[14]+138;
    Menu.y[15] = Menu.y[12];
  }

  if(Menu.Cols == 3)
  {
    Menu.x[0] = MenuStartPosX;
    Menu.y[0] = MenuStartPosY;
    Menu.x[1] = Menu.x[0]+138;
    Menu.y[1] = Menu.y[0];
    Menu.x[2] = Menu.x[1]+138;
    Menu.y[2] = Menu.y[0];
    Menu.x[3] = MenuStartPosX;
    Menu.y[3] = Menu.y[0]+106;
    Menu.x[4] = Menu.x[3]+138;
    Menu.y[4] = Menu.y[3];
    Menu.x[5] = Menu.x[4]+138;
    Menu.y[5] = Menu.y[3];
    Menu.x[6] = MenuStartPosX;
    Menu.y[6] = Menu.y[3]+106;
    Menu.x[7] = Menu.x[6]+138;
    Menu.y[7] = Menu.y[6];
    Menu.x[8] = Menu.x[7]+138;
    Menu.y[8] = Menu.y[6];
    Menu.x[9] = MenuStartPosX;
    Menu.y[9] = Menu.y[6]+106;
    Menu.x[10] = Menu.x[9]+138;
    Menu.y[10] = Menu.y[9];
    Menu.x[11] = Menu.x[10]+138;
    Menu.y[11] = Menu.y[9];
    Menu.x[12] = MenuStartPosX;
    Menu.y[12] = Menu.y[9]+106;
    Menu.x[13] = Menu.x[12]+138;
    Menu.y[13] = Menu.y[12];
    Menu.x[14] = Menu.x[13]+138;
    Menu.y[14] = Menu.y[12];
    Menu.x[15] = MenuStartPosX;
    Menu.y[15] = Menu.y[12]+106;
  }
}

// ##############################################
// ##############################################
// ##############################################
