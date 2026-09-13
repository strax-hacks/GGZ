#include "globals.h"

void Game_Close()
{
  if (gameplayTargetTexture) {
    SDL_DestroyTexture( gameplayTargetTexture );
    gameplayTargetTexture = NULL;
  }
  if (gRenderer) {
    SDL_DestroyRenderer( gRenderer );
    gRenderer = NULL;
  }
  if (gWindow) {
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
  }
  Mix_FreeMusic(BGM_Title); BGM_Title = NULL;
  Mix_FreeMusic(BGM_Outdoors); BGM_Outdoors = NULL;
  Mix_FreeMusic(BGM_Indoors); BGM_Indoors = NULL;
  Mix_FreeMusic(BGM_Highscore); BGM_Highscore = NULL;
  Mix_FreeMusic(BGM_MultiSong); BGM_MultiSong = NULL;
  Mix_CloseAudio();
  IMG_Quit();
  SDL_Quit();
}
