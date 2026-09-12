#include "globals.h"

void Game_Close()
{
  if (gRenderer) {
    SDL_DestroyRenderer( gRenderer );
    gRenderer = NULL;
  }
  if (gWindow) {
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
  }
  IMG_Quit();
  SDL_Quit();
}
