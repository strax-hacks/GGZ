#include "globals.h"

void BACKGROUND_ELEMENTS_Outdoors_Draw();
void BACKGROUND_ELEMENTS_Indoors_Draw();
void Background_DrawATube(int x, int y, bool Fire);

// ##############################################
// ##############################################
// ##############################################

void STAGE_Draw()
{
  int x,y;
  int virtualWidth = GV.Screen_Width;
  int virtualHeight = GV.Screen_Height;

  if (GV.Resolution == RESOLUTION_320x170 && GV.ViewMode == VIEW_MODE_C64_SCALED)
  {
    virtualWidth = 640;
    virtualHeight = 340;
  }

  if(PC.PosX < virtualWidth/2)   {PC.StagePosX = 0;}
  if(PC.PosX+1 > virtualWidth/2) {PC.StagePosX = PC.PosX-virtualWidth/2;}
  if(PC.PosX+1-(virtualWidth/2) > StageC64.StageWidthPixels-virtualWidth) {PC.StagePosX = StageC64.StageWidthPixels-virtualWidth;}

  if (virtualHeight >= StageC64.StageHeightPixels) {
    PC.StagePosY = 0;
  } else {
    if (GV.Resolution == RESOLUTION_320x170 && GV.ViewMode == VIEW_MODE_C64_SCALED) {
      // In 640x340 C64 mode, stage is 480px high (30 tiles).
      // Bottom ground is at 480px, so base camera offset is 480 - 340 = 140px.
      // If Giana jumps into the upper clouds/platforms (PC.PosY < 180), track upwards smoothly.
      PC.StagePosY = 140;
      if (PC.PosY < 220) {
        PC.StagePosY = PC.PosY - 80;
      }
    } else {
      PC.StagePosY = PC.PosY - (virtualHeight / 2) - 10;
    }
    if (PC.StagePosY < 0) { PC.StagePosY = 0; }
    if (PC.StagePosY > StageC64.StageHeightPixels - virtualHeight) {
      PC.StagePosY = StageC64.StageHeightPixels - virtualHeight;
    }
  }

  // Tiles 0 and 40 are solid opaque colors in the C64 tile sheet. Clearing
  // once is pixel-equivalent and avoids one render submission per cell.
  if (StageC64.BackgroundColour == 0) {
    SDL_SetRenderDrawColor(gRenderer, 124, 112, 218, 255);
  } else {
    SDL_SetRenderDrawColor(gRenderer, 18, 18, 18, 255);
  }
  SDL_RenderClear(gRenderer);

  const int screenWTiles = virtualWidth / 16;

  if(StageC64.BackgroundColour == 0){BACKGROUND_ELEMENTS_Outdoors_Draw();}
  if(StageC64.BackgroundColour == 1){BACKGROUND_ELEMENTS_Indoors_Draw();}

  TS.NextFrame = false;

  for(y=0; y<StageC64.StageHeight; y++)
  {
    int drawY = (y*TS.Tile_Height) - PC.StagePosY;
    if (drawY < -TS.Tile_Height || drawY > virtualHeight) continue;
    for(x=(int)(PC.StagePosX/16); x<(int)(PC.StagePosX/16)+screenWTiles+2; x++)
    {
      if (x >= 0 && x < StageC64.StageWidth && StageC64.TileNumber[x][y] !=0)
      {
        TILE_Draw((x*TS.Tile_Width)-PC.StagePosX, drawY, StageC64.TileNumber[x][y]);
        if(GV.ShowDebugInfos_Tiles)
        {
          if(TileType[StageC64.TileNumber[x][y]].Exit)   {INTERFACE_Tile_Draw((x*TS.Tile_Width)-PC.StagePosX, drawY, INTERFACE_MARK_EXIT);}
          if(TileType[StageC64.TileNumber[x][y]].Lethal) {INTERFACE_Tile_Draw((x*TS.Tile_Width)-PC.StagePosX, drawY, INTERFACE_MARK_LETHAL);}
          if(TileType[StageC64.TileNumber[x][y]].Coin)   {INTERFACE_Tile_Draw((x*TS.Tile_Width)-PC.StagePosX, drawY, INTERFACE_MARK_COIN);}
        }
      }
    }
  }
}

// ##############################################
// ##############################################
// ##############################################

void BACKGROUND_ELEMENTS_Outdoors_Draw()
{
    int TileNumber = 104;
    int x;
    int CPosX;
    int CPosY;

    for(x=0; x<12; x++)
    {
      CPosX = BackGroundElements.CloudPosX[x];
      if (GV.Resolution == RESOLUTION_320x170 && GV.ViewMode == VIEW_MODE_C64_SCALED) {
        CPosY = BackGroundElements.CloudPosY[x] - int(PC.StagePosY / 4);
      } else {
        CPosY = BackGroundElements.CloudPosY[x] - int(PC.StagePosY / 2);
      }
      TILE_Draw(CPosX-int(PC.StagePosX/2)+(0*TS.Tile_Width), CPosY, TileNumber+0);
      TILE_Draw(CPosX-int(PC.StagePosX/2)+(1*TS.Tile_Width), CPosY, TileNumber+1);
      TILE_Draw(CPosX-int(PC.StagePosX/2)+(2*TS.Tile_Width), CPosY, TileNumber+2);
      TILE_Draw(CPosX-int(PC.StagePosX/2)+(3*TS.Tile_Width), CPosY, TileNumber+3);
      TILE_Draw(CPosX-int(PC.StagePosX/2)+(4*TS.Tile_Width), CPosY, TileNumber+4);
      TILE_Draw(CPosX-int(PC.StagePosX/2)+(0*TS.Tile_Width), CPosY+TS.Tile_Height, TileNumber+0+TS.Width);
      TILE_Draw(CPosX-int(PC.StagePosX/2)+(1*TS.Tile_Width), CPosY+TS.Tile_Height, TileNumber+1+TS.Width);
      TILE_Draw(CPosX-int(PC.StagePosX/2)+(2*TS.Tile_Width), CPosY+TS.Tile_Height, TileNumber+2+TS.Width);
      TILE_Draw(CPosX-int(PC.StagePosX/2)+(3*TS.Tile_Width), CPosY+TS.Tile_Height, TileNumber+3+TS.Width);
      TILE_Draw(CPosX-int(PC.StagePosX/2)+(4*TS.Tile_Width), CPosY+TS.Tile_Height, TileNumber+4+TS.Width);
    }

  SDL_SetTextureAlphaMod(TilesTexture,100);
  CPosY = 440;
  CPosX = BackGroundElements.TubePosX[0]; Background_DrawATube(CPosX, CPosY, BackGroundElements.TubeFire[0]);
  CPosX = BackGroundElements.TubePosX[1]; Background_DrawATube(CPosX, CPosY, BackGroundElements.TubeFire[1]);
  CPosX = BackGroundElements.TubePosX[2]; Background_DrawATube(CPosX, CPosY, BackGroundElements.TubeFire[2]);
  CPosX = BackGroundElements.TubePosX[3]; Background_DrawATube(CPosX, CPosY, BackGroundElements.TubeFire[3]);
  SDL_SetTextureAlphaMod(TilesTexture,255);
}

void Background_DrawATube(int x, int y, bool Fire)
{
  int TileNumber = 514; // BIG TUBE
  int TubePosY;
  int z;

  for(z=0; z<2; z++)
  {
    TubePosY = y-(z*48) - PC.StagePosY;
    TILE_Draw(x-int(PC.StagePosX/2)+(0*TS.Tile_Width), TubePosY, TileNumber+0);
    TILE_Draw(x-int(PC.StagePosX/2)+(1*TS.Tile_Width), TubePosY, TileNumber+1);
    TILE_Draw(x-int(PC.StagePosX/2)+(2*TS.Tile_Width), TubePosY, TileNumber+2);
    TILE_Draw(x-int(PC.StagePosX/2)+(3*TS.Tile_Width), TubePosY, TileNumber+3);
    TILE_Draw(x-int(PC.StagePosX/2)+(4*TS.Tile_Width), TubePosY, TileNumber+4);
    TILE_Draw(x-int(PC.StagePosX/2)+(5*TS.Tile_Width), TubePosY, TileNumber+5);
    TILE_Draw(x-int(PC.StagePosX/2)+(0*TS.Tile_Width), TubePosY+TS.Tile_Height, TileNumber+0+TS.Width);
    TILE_Draw(x-int(PC.StagePosX/2)+(1*TS.Tile_Width), TubePosY+TS.Tile_Height, TileNumber+1+TS.Width);
    TILE_Draw(x-int(PC.StagePosX/2)+(2*TS.Tile_Width), TubePosY+TS.Tile_Height, TileNumber+2+TS.Width);
    TILE_Draw(x-int(PC.StagePosX/2)+(3*TS.Tile_Width), TubePosY+TS.Tile_Height, TileNumber+3+TS.Width);
    TILE_Draw(x-int(PC.StagePosX/2)+(4*TS.Tile_Width), TubePosY+TS.Tile_Height, TileNumber+4+TS.Width);
    TILE_Draw(x-int(PC.StagePosX/2)+(5*TS.Tile_Width), TubePosY+TS.Tile_Height, TileNumber+5+TS.Width);
    TILE_Draw(x-int(PC.StagePosX/2)+(0*TS.Tile_Width), TubePosY+(2*TS.Tile_Height), TileNumber+0+(2*TS.Width));
    TILE_Draw(x-int(PC.StagePosX/2)+(1*TS.Tile_Width), TubePosY+(2*TS.Tile_Height), TileNumber+1+(2*TS.Width));
    TILE_Draw(x-int(PC.StagePosX/2)+(2*TS.Tile_Width), TubePosY+(2*TS.Tile_Height), TileNumber+2+(2*TS.Width));
    TILE_Draw(x-int(PC.StagePosX/2)+(3*TS.Tile_Width), TubePosY+(2*TS.Tile_Height), TileNumber+3+(2*TS.Width));
    TILE_Draw(x-int(PC.StagePosX/2)+(4*TS.Tile_Width), TubePosY+(2*TS.Tile_Height), TileNumber+4+(2*TS.Width));
    TILE_Draw(x-int(PC.StagePosX/2)+(5*TS.Tile_Width), TubePosY+(2*TS.Tile_Height), TileNumber+5+(2*TS.Width));
  }

  if(Fire)
  {
    TileNumber = 6; // FIRE
    TILE_Draw(x-int(PC.StagePosX/2)+(1*TS.Tile_Width), TubePosY-(2*TS.Tile_Height), TileNumber+0);
    TILE_Draw(x-int(PC.StagePosX/2)+(2*TS.Tile_Width), TubePosY-(2*TS.Tile_Height), TileNumber+1);
    TILE_Draw(x-int(PC.StagePosX/2)+(3*TS.Tile_Width), TubePosY-(2*TS.Tile_Height), TileNumber+2);
    TILE_Draw(x-int(PC.StagePosX/2)+(4*TS.Tile_Width), TubePosY-(2*TS.Tile_Height), TileNumber+3);
    TILE_Draw(x-int(PC.StagePosX/2)+(1*TS.Tile_Width), TubePosY-(1*TS.Tile_Height), TileNumber+0+TS.Width);
    TILE_Draw(x-int(PC.StagePosX/2)+(2*TS.Tile_Width), TubePosY-(1*TS.Tile_Height), TileNumber+1+TS.Width);
    TILE_Draw(x-int(PC.StagePosX/2)+(3*TS.Tile_Width), TubePosY-(1*TS.Tile_Height), TileNumber+2+TS.Width);
    TILE_Draw(x-int(PC.StagePosX/2)+(4*TS.Tile_Width), TubePosY-(1*TS.Tile_Height), TileNumber+3+TS.Width);
  }
}

// ##############################################
// ##############################################
// ##############################################


void BACKGROUND_ELEMENTS_Indoors_Draw()
{
    int TileNumber = 672;
    int x, y;
    int CPosX = 0;
    int DrawPos;
    int virtualWidth = (GV.Resolution == RESOLUTION_320x170 && GV.ViewMode == VIEW_MODE_C64_SCALED) ? 640 : GV.Screen_Width;
    SDL_SetTextureAlphaMod(TilesTexture,100);

    for(x=0; x<84; x++)
    {
      DrawPos = CPosX-int(PC.StagePosX/2)+(0*TS.Tile_Width)+(x*2*TS.Tile_Width);
      if(DrawPos > -(2*TS.Tile_Width) && DrawPos < virtualWidth) //ONLY DRAW WHAT'S VISIBLE
      {
        for(y=4; y<15; y++)
        {
          int drawY1 = y*2*TS.Tile_Height - PC.StagePosY;
          int drawY2 = (y*2*TS.Tile_Height)+TS.Tile_Height - PC.StagePosY;
          TILE_Draw(CPosX-int(PC.StagePosX/2)+(0*TS.Tile_Width)+(x*2*TS.Tile_Width), drawY1, TileNumber+0);
          TILE_Draw(CPosX-int(PC.StagePosX/2)+(1*TS.Tile_Width)+(x*2*TS.Tile_Width), drawY1, TileNumber+1);
          TILE_Draw(CPosX-int(PC.StagePosX/2)+(0*TS.Tile_Width)+(x*2*TS.Tile_Width), drawY2, TileNumber+0+TS.Width);
          TILE_Draw(CPosX-int(PC.StagePosX/2)+(1*TS.Tile_Width)+(x*2*TS.Tile_Width), drawY2, TileNumber+1+TS.Width);
        }
      }
    }
    SDL_SetTextureAlphaMod(TilesTexture,255);
}

// ##############################################
// ##############################################
// ##############################################
