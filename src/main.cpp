#include "globals.h"

bool QuitProgram;
Config_Type Config;

void init();                    //Starts up SDL and creates window
void iniSetup();

// ##############################################
// ##############################################
// ##############################################

int main( int argc, char* args[] )
{
  GAME_ENVIRONMENT_Define();
  Options_Load();

  init();               // Start up SDL and create window
  srand (time(NULL));   // initialize random seed
  defineAngles();

  Option_GameType_Load();
  GAMETYPE_Load();


  QuitProgram = false;

  LOOP_Intro();
  if(!QuitProgram)
  {
    LOOP_Menu();
  }


  Game_Close();  //Free resources and close SDL
  return 0;
}

// ##############################################
// ##############################################
// ##############################################

// ##############################################
// ##############################################
// ##############################################
