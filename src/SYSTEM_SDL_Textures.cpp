#include "globals.h"
#include "SYSTEM_Paths.h"

//SDL_Texture* tilesTexture = NULL;
SDL_Texture* fontTexture = NULL;
SDL_Texture* PlayerTexture = NULL;
SDL_Texture* TilesTexture = NULL;
SDL_Texture* EnemiesTexture = NULL;
SDL_Texture* InterfaceTexture = NULL;
SDL_Texture* PowerUpTexture = NULL;
SDL_Surface* tmpSurface;

// ##############################################
// ##############################################
// ##############################################

void loadTextures()
{
  // KILL TEXTURES - OTHERWISE MEMORY USAGE EXPLODES
  // WHEN LOADING DIFFERENT TEXTURES
  if (fontTexture) { SDL_DestroyTexture(fontTexture); fontTexture = NULL; }
  if (PlayerTexture) { SDL_DestroyTexture(PlayerTexture); PlayerTexture = NULL; }
  if (TilesTexture) { SDL_DestroyTexture(TilesTexture); TilesTexture = NULL; }
  if (EnemiesTexture) { SDL_DestroyTexture(EnemiesTexture); EnemiesTexture = NULL; }
  if (InterfaceTexture) { SDL_DestroyTexture(InterfaceTexture); InterfaceTexture = NULL; }
  if (PowerUpTexture) { SDL_DestroyTexture(PowerUpTexture); PowerUpTexture = NULL; }

  std::string fontPath = GetAssetPath(FileName.Font);
  tmpSurface = IMG_Load(fontPath.c_str());
  if (tmpSurface) {
    SDL_SetColorKey( tmpSurface, SDL_TRUE, SDL_MapRGB( tmpSurface->format, 255, 0, 255 ) );
    fontTexture = SDL_CreateTextureFromSurface( gRenderer, tmpSurface );
    SDL_FreeSurface( tmpSurface );
  }

  std::string playerPath = GetAssetPath(FileName.Player);
  tmpSurface = IMG_Load(playerPath.c_str());
  if (tmpSurface) {
    SDL_SetColorKey( tmpSurface, SDL_TRUE, SDL_MapRGB( tmpSurface->format, 255, 0, 255 ) );
    PlayerTexture = SDL_CreateTextureFromSurface( gRenderer, tmpSurface );
    SDL_FreeSurface( tmpSurface );
  }

  std::string tilesPath = GetAssetPath(FileName.Tiles);
  tmpSurface = IMG_Load(tilesPath.c_str());
  if (tmpSurface) {
    SDL_SetColorKey( tmpSurface, SDL_TRUE, SDL_MapRGB( tmpSurface->format, 255, 0, 255 ) );
    TilesTexture = SDL_CreateTextureFromSurface( gRenderer, tmpSurface );
    SDL_FreeSurface( tmpSurface );
  }

  std::string enemiesPath = GetAssetPath(FileName.Enemies);
  tmpSurface = IMG_Load(enemiesPath.c_str());
  if (tmpSurface) {
    SDL_SetColorKey( tmpSurface, SDL_TRUE, SDL_MapRGB( tmpSurface->format, 255, 0, 255 ) );
    EnemiesTexture = SDL_CreateTextureFromSurface( gRenderer, tmpSurface );
    SDL_FreeSurface( tmpSurface );
  }

  std::string ifacePath = GetAssetPath(FileName.Interface);
  tmpSurface = IMG_Load(ifacePath.c_str());
  if (tmpSurface) {
    SDL_SetColorKey( tmpSurface, SDL_TRUE, SDL_MapRGB( tmpSurface->format, 255, 0, 255 ) );
    InterfaceTexture = SDL_CreateTextureFromSurface( gRenderer, tmpSurface );
    SDL_FreeSurface( tmpSurface );
  }

  std::string powerPath = GetAssetPath(FileName.PowerUp);
  tmpSurface = IMG_Load(powerPath.c_str());
  if (tmpSurface) {
    SDL_SetColorKey( tmpSurface, SDL_TRUE, SDL_MapRGB( tmpSurface->format, 255, 0, 255 ) );
    PowerUpTexture = SDL_CreateTextureFromSurface( gRenderer, tmpSurface );
    SDL_FreeSurface( tmpSurface );
  }
}

// ##############################################
// ##############################################
// ##############################################
