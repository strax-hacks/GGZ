#include "globals.h"

Text_Offset_Def Credits;

// ##############################################
// ##############################################
// ##############################################

void LOOP_Credits()
{
  int GameModeTemp = GV.Mode;
  GV.Mode = MODE_CREDITS;

  Credits.x_off = 16;
  Credits.y_off = 28;

  while(!QuitProgram && !QuitToMenu)
  {
    getInput();
    INPUT_General();

    if(Key_ESCAPE_pressed || Joy_ESCAPE_pressed || Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed){QuitToMenu = true;}

    if(Key_DOWN_pressed || Joy_DOWN_pressed)
    {
      if (Credits.y_off > -150) Credits.y_off -= 12;
    }
    if(Key_UP_pressed || Joy_UP_pressed)
    {
      if (Credits.y_off < 28) Credits.y_off += 12;
    }

    Update_Screen();  // draw the scene
  }
  GV.Mode = GameModeTemp;
  getInput();  // make sure no "pressed key" is carried over to the menu...
}

// ##############################################
// ##############################################
// ##############################################

void LOOP_Credits_Draw()
{
  Print((320 - 7 * 8) / 2, 8, 3, 0, "CREDITS");

  int yBase = (Credits.y_off == 0) ? 26 : Credits.y_off;
  int xBase = (Credits.x_off == 0) ? 16 : Credits.x_off;

  SDL_Rect clip = { 0, 22, 320, 122 };
  SDL_RenderSetClipRect(gRenderer, &clip);

  Print(xBase, 0 + yBase, 2, 0, "The Great Giana Sisters (C64)");
  Print(xBase, 12 + yBase, 0, 0, "1987 Softgold / Rainbow Arts");
  Print(xBase, 24 + yBase, 0, 0, "Written:      Armin Gessert");
  Print(xBase, 36 + yBase, 0, 0, "Artwork:      Manfred Trenz");
  Print(xBase, 48 + yBase, 0, 0, "Music:        Chris Huelsbeck");

  Print(xBase, 68 + yBase, 2, 0, "OpenGGS 2.0 (OpenGGS Maker)");
  Print(xBase, 80 + yBase, 0, 0, "Written:      RomanH");
  Print(xBase, 92 + yBase, 0, 0, "Artwork:      RomanH");
  Print(xBase, 104 + yBase, 0, 0, "Sound FX:     RomanH, Kenney");
  Print(xBase, 116 + yBase, 0, 0, "Music:        Alejandro Altimari");

  Print(xBase, 136 + yBase, 2, 0, "Cardputer ZERO Port");
  Print(xBase, 148 + yBase, 0, 0, "Port & Dev:   strax-hacks");
  Print(xBase, 160 + yBase, 0, 0, "Display:      ST7789v3 320x170 Native");
  Print(xBase, 172 + yBase, 0, 0, "GitHub:       strax-hacks/GGZ");

  Print(xBase, 192 + yBase, 2, 0, "Thanks to all retro gaming fans!");

  SDL_RenderSetClipRect(gRenderer, NULL);

  Print((320 - 30 * 8) / 2, 150, 2, 0, "UP/DOWN:SCROLL  ESC/ENTER:BACK");
}
