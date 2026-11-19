#include "globals.h"

bool QuitProgram;
Config_Type Config;

void iniSetup();

// ##############################################
// ##############################################
// ##############################################

int main( int argc, char* args[] )
{
  GAME_ENVIRONMENT_Define();
  Options_Load();

  const char* driver = SDL_getenv("SDL_VIDEODRIVER");
  const int attempts = driver && SDL_strcasecmp(driver, "kmsdrm") == 0 ? 5 : 1;
  for (int attempt = 1; !init(); ++attempt)
  {
    Game_Close();
    if (attempt == attempts) {
      return 1;
    }
    SDL_Delay(300);
  }
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
