#include "globals.h"

Text_Offset_Def Story;

// ##############################################
// ##############################################
// ##############################################

void LOOP_Story()
{
  int GameModeTemp = GV.Mode;
  GV.Mode = MODE_STORY;
  AUDIO_Music_Halt();

  Story.x_off = 10;
  Story.y_off = 26;

  while(!QuitProgram && !QuitToMenu)
  {
    getInput();
    INPUT_General();

    if(Key_ESCAPE_pressed || Joy_ESCAPE_pressed || Key_ENTER_pressed || Key_SPACE_pressed || Joy_OK_pressed){QuitToMenu = true;}

    if(Key_DOWN_pressed || Joy_DOWN_pressed)
    {
      if (Story.y_off > -100) Story.y_off -= 12;
    }
    if(Key_UP_pressed || Joy_UP_pressed)
    {
      if (Story.y_off < 26) Story.y_off += 12;
    }

    Update_Screen();  // draw the scene
  }
  GV.Mode = GameModeTemp;
  getInput();  // make sure no "pressed key" is carried over to the menu...
}

// ##############################################
// ##############################################
// ##############################################

void LOOP_Story_Draw()
{
  int Line = 12;

  Print((320 - 19 * 8) / 2, 8, 3, 0, "GIANA SISTERS STORY");

  int yBase = (Story.y_off == 0) ? 26 : Story.y_off;
  int xBase = (Story.x_off == 0) ? 10 : Story.x_off;

  SDL_Rect clip = { 0, 22, 320, 122 };
  SDL_RenderSetClipRect(gRenderer, &clip);

  Print(xBase, yBase + (0*Line), 2, 0, "What happened?");
  Print(xBase, yBase + (1*Line), 0, 0, "One night, when little Giana");
  Print(xBase, yBase + (2*Line), 0, 0, "was fast asleep, she had");
  Print(xBase, yBase + (3*Line), 0, 0, "a strange dream.");
  Print(xBase, yBase + (4*Line), 0, 0, "Everybody dreams weird things,");
  Print(xBase, yBase + (5*Line), 0, 0, "but little Giana is about to");
  Print(xBase, yBase + (6*Line), 0, 0, "experience a new world!");
  Print(xBase, yBase + (7*Line), 0, 0, "");
  Print(xBase, yBase + (8*Line), 0, 0, "Giana finds herself in a");
  Print(xBase, yBase + (9*Line), 0, 0, "mysterious realm where");
  Print(xBase, yBase + (10*Line), 0, 0, "strange creatures lurk.");
  Print(xBase, yBase + (11*Line), 0, 0, "She cannot escape until she");
  Print(xBase, yBase + (12*Line), 0, 0, "finds the magical diamond!");
  Print(xBase, yBase + (13*Line), 0, 0, "");
  Print(xBase, yBase + (14*Line), 2, 0, "Her sister Maria dreams too...");

  SDL_RenderSetClipRect(gRenderer, NULL);

  Print((320 - 30 * 8) / 2, 150, 2, 0, "UP/DOWN:SCROLL  ESC/ENTER:BACK");
}
