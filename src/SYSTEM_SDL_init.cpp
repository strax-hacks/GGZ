#include "globals.h"

#if defined(TARGET_CARDPUTER_ZERO) && defined(__linux__)
#include <SDL_syswm.h>
#include <xf86drm.h>
#endif

int CurrentScreenMode;
SDL_Window* gWindow = NULL;     //The window we'll be rendering to
SDL_Renderer* gRenderer = NULL; //The window renderer
SDL_Joystick* GameController = NULL;

// ##############################################
// ##############################################
// ##############################################

bool init()
{
  CurrentScreenMode = 0;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "SDL video initialization failed: %s\n", SDL_GetError());
    return false;
  }

  const bool handheldSize = GV.Resolution == RESOLUTION_320x170;
  const char* title = handheldSize ? "Giana Sisters Zero" : "OpenGGS 2.0 (aka OpenGGS Maker) - BETA 3";
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
  gWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             handheldSize ? 320 : GV.Screen_Width,
                             handheldSize ? 170 : GV.Screen_Height, SDL_WINDOW_SHOWN);
  if (!gWindow) {
    fprintf(stderr, "SDL window creation failed: %s\n", SDL_GetError());
    return false;
  }

#if defined(TARGET_CARDPUTER_ZERO) && defined(__linux__) && defined(SDL_VIDEO_DRIVER_KMSDRM)
  if (SDL_strcasecmp(SDL_GetCurrentVideoDriver(), "kmsdrm") == 0) {
    SDL_SysWMinfo info{};
    SDL_VERSION(&info.version);
    if (!SDL_GetWindowWMInfo(gWindow, &info) || info.subsystem != SDL_SYSWM_KMSDRM) {
      fprintf(stderr, "Could not get the LCD device from SDL: %s\n", SDL_GetError());
      return false;
    }
    // SDL can create a KMS window even when the DRM master ioctl fails.
    const int fd = info.info.kmsdrm.drm_fd;
    if (!drmIsMaster(fd) && drmSetMaster(fd) != 0) {
      fprintf(stderr, "Could not take control of the Cardputer LCD.\n");
      return false;
    }
  }
#endif

  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
  if (!gRenderer) {
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE);
  }
  if (!gRenderer) {
    gRenderer = SDL_CreateRenderer(gWindow, -1, 0);
  }
  if (!gRenderer) {
    fprintf(stderr, "SDL renderer creation failed: %s\n", SDL_GetError());
    return false;
  }
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    fprintf(stderr, "SDL PNG initialization failed: %s\n", IMG_GetError());
    return false;
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
  SDL_RenderSetLogicalSize(gRenderer, GV.Screen_Width, GV.Screen_Height);

  if (gameplayTargetTexture) {
    SDL_DestroyTexture(gameplayTargetTexture);
  }
  gameplayTargetTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888,
                                            SDL_TEXTUREACCESS_TARGET, 640, 340);
  if (gameplayTargetTexture) {
    SDL_SetTextureBlendMode(gameplayTargetTexture, SDL_BLENDMODE_NONE);
  } else {
    fprintf(stderr, "Could not create the gameplay texture: %s\n", SDL_GetError());
  }

  int i;
  SDL_Init( SDL_INIT_GAMECONTROLLER );
  SDL_JoystickEventState(SDL_ENABLE);
  GameController = SDL_JoystickOpen( 0 );

  for( i=0; i < SDL_NumJoysticks(); i++ )
  {
    fprintf(stdout,"Joystick Found! - %i - \n\n", i); //SDL_JoystickName(i)
  }

  SYSTEM_SetResolution(GV.Resolution);
  return true;
}

// ##############################################
// ##############################################
// ##############################################

void toggleScreenmode()
{
/*
  // NEEDED FOR CUSTOM FULLSCREEN
  SDL_DisplayMode mode;
  mode.refresh_rate = 0;
  mode.driverdata = 0;
  mode.format = SDL_PIXELFORMAT_ARGB8888;
*/

  CurrentScreenMode=CurrentScreenMode+1;
  if(CurrentScreenMode>1){CurrentScreenMode=0;}
  SDL_ShowCursor(1);
  if(GV.Mode == MODE_GAMELOOP && CurrentScreenMode == 1){SDL_ShowCursor(0);}

  if(CurrentScreenMode==0)  // WINDOWED
  {
    SDL_RenderSetScale(gRenderer, 1, 1);
  }

  // DELETE THIS "IF" TO HAVE NORMAL SCALING
  if(CurrentScreenMode==1)  // FULLSCREEN
  {
/*
    SDL_GetCurrentDisplayMode(0, &mode);
    if(mode.h > 959){SDL_RenderSetScale(gRenderer, 2, 2);}
    SDL_SetWindowDisplayMode(gWindow, &mode);
*/
  }

  SDL_SetWindowFullscreen(gWindow, CurrentScreenMode);
}

// ##############################################
// ##############################################
// ##############################################

void SYSTEM_SetResolution(int Resolution)
{
  // IF FULLSCREEN GOTO WINDOWED MODE
  if(CurrentScreenMode==1){toggleScreenmode();}

  if(Resolution == RESOLUTION_320x170)
  {
    GV.Screen_Width = 320;
    GV.Screen_Height = 170;
    GV.Resolution = RESOLUTION_320x170;
    LOOP_Menu_Define_Positions(2);

    // Compact single-line HUD header for 320x170 display
    HUD.PlayerX = 2;
    HUD.PlayerY = 2;
    HUD.SmallCoinX = 120;
    HUD.SmallCoinY = 2;
    HUD.BonusX = 138;
    HUD.BonusY = 2;
    HUD.LivesX = 174;
    HUD.LivesY = 2;
    HUD.StageX = 222;
    HUD.StageY = 2;
    HUD.TimeX = 276;
    HUD.TimeY = 2;
  }
  else
  {
    HUD.PlayerX = 16;
    HUD.PlayerY = 0;
    HUD.BonusX = 160;
    HUD.BonusY = 0;
    HUD.LivesX = 288;
    HUD.LivesY = 0;
    HUD.StageX = 416;
    HUD.StageY = 0;
    HUD.TimeX = 544;
    HUD.TimeY = 0;
    HUD.SmallCoinX = 160;
    HUD.SmallCoinY = 16;
  }

  if(Resolution == RESOLUTION_640x480)
  {
    GV.Screen_Width = 640;
    GV.Screen_Height = 480;
    GV.Resolution = RESOLUTION_640x480;
    LOOP_Menu_Define_Positions(4);
  }

  if(Resolution == RESOLUTION_800x600)
  {
    GV.Screen_Width = 800;
    GV.Screen_Height = 600;
    GV.Resolution = RESOLUTION_800x600;
    LOOP_Menu_Define_Positions(5);
    HUD.PlayerX += 80;
    HUD.BonusX += 80;
    HUD.LivesX += 80;
    HUD.StageX += 80;
    HUD.TimeX += 80;
    HUD.SmallCoinX += 80;
  }

  if(Resolution == RESOLUTION_1280x720)
  {
    GV.Screen_Width = 1280;
    GV.Screen_Height = 720;
    GV.Resolution = RESOLUTION_1280x720;
    LOOP_Menu_Define_Positions(5);
    HUD.PlayerX += 320;
    HUD.BonusX += 320;
    HUD.LivesX += 320;
    HUD.StageX += 320;
    HUD.TimeX += 320;
    HUD.SmallCoinX += 320;
  }

  Sprite_SmallCoin[0].PosX = HUD.SmallCoinX;
  Sprite_SmallCoin[0].PosY = HUD.SmallCoinY;

  if (gWindow != NULL) {
    SDL_SetWindowSize(gWindow, GV.Screen_Width, GV.Screen_Height);
    SDL_SetWindowPosition(gWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  }
  if (gRenderer != NULL) {
    SDL_RenderSetLogicalSize(gRenderer, GV.Screen_Width, GV.Screen_Height);
  }
  GV.ScreenWidthTiles = (int)(GV.Screen_Width/TS.Tile_Width);
  GV.ScreenHeightTiles = (int)(GV.Screen_Height/TS.Tile_Height);
}

// ##############################################
// ##############################################
// ##############################################
