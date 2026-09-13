#include "globals.h"

void Delete_Coin(int x, int y);

// ##############################################
// ##############################################
// ##############################################

void PC_Check_TilecollisionNonSolid()
{
  // CALCULATE PLAYER POSITION
  PC.x1T = (int)((PC.PosX-PC.ColWidthHalf)/TS.Tile_Width);
  PC.x2T = (int)(PC.PosX/TS.Tile_Width);
  PC.x3T = (int)((PC.PosX+PC.ColWidthHalf)/TS.Tile_Width);
  PC.y1T = (int)((PC.PosY-PC.ColHeight)/TS.Tile_Height);
  PC.y2T = (int)((PC.PosY-PC.ColHeightHalf)/TS.Tile_Height);
  PC.y3T = (int)((PC.PosY-1)/TS.Tile_Height);
  PC.x1 = PC.PosX-PC.ColWidthHalf;
  PC.x2 = PC.PosX;
  PC.x3 = PC.PosX+PC.ColWidthHalf;
  PC.y1 = PC.PosY-PC.ColHeight;
  PC.y2 = PC.PosY-PC.ColHeightHalf;
  PC.y3 = PC.PosY-1;

  // CHECK FOR COIN COLLISIONS
  if(GetTileTypeAt(PC.x1T, PC.y1T).Coin){Delete_Coin(PC.x1T, PC.y1T);}
  if(GetTileTypeAt(PC.x2T, PC.y1T).Coin){Delete_Coin(PC.x2T, PC.y1T);}
  if(GetTileTypeAt(PC.x3T, PC.y1T).Coin){Delete_Coin(PC.x3T, PC.y1T);}
  if(GetTileTypeAt(PC.x1T, PC.y2T).Coin){Delete_Coin(PC.x1T, PC.y2T);}
  if(GetTileTypeAt(PC.x2T, PC.y2T).Coin){Delete_Coin(PC.x2T, PC.y2T);}
  if(GetTileTypeAt(PC.x3T, PC.y2T).Coin){Delete_Coin(PC.x3T, PC.y2T);}
  if(GetTileTypeAt(PC.x1T, PC.y3T).Coin){Delete_Coin(PC.x1T, PC.y3T);}
  if(GetTileTypeAt(PC.x2T, PC.y3T).Coin){Delete_Coin(PC.x2T, PC.y3T);}
  if(GetTileTypeAt(PC.x3T, PC.y3T).Coin){Delete_Coin(PC.x3T, PC.y3T);}
  // CHECK FOR COIN COLLISIONS

  // CHECK FOR LETHAL COLLISIONS
  if(PC.PosY > PC.ColHeight && PC.PowerUp < 7)
  {
    if(GetTileTypeAt(PC.x1T, PC.y1T).Lethal){PC.gotKilled = true;}
    if(GetTileTypeAt(PC.x2T, PC.y1T).Lethal){PC.gotKilled = true;}
    if(GetTileTypeAt(PC.x3T, PC.y1T).Lethal){PC.gotKilled = true;}
    if(GetTileTypeAt(PC.x1T, PC.y2T).Lethal){PC.gotKilled = true;}
    if(GetTileTypeAt(PC.x2T, PC.y2T).Lethal){PC.gotKilled = true;}
    if(GetTileTypeAt(PC.x3T, PC.y2T).Lethal){PC.gotKilled = true;}
    if(GetTileTypeAt(PC.x1T, PC.y3T).Lethal){PC.gotKilled = true;}
    if(GetTileTypeAt(PC.x2T, PC.y3T).Lethal){PC.gotKilled = true;}
    if(GetTileTypeAt(PC.x3T, PC.y3T).Lethal){PC.gotKilled = true;}
  }

  if(PC.PosY > PC.ColHeight && PC.PowerUp > 6) // DON'T GET KILLED BY FIRE
  {
    if(GetTileTypeAt(PC.x1T, PC.y1T).Lethal && !GetTileTypeAt(PC.x1T, PC.y1T).Fire){PC.gotKilled = true;}
    if(GetTileTypeAt(PC.x2T, PC.y1T).Lethal && !GetTileTypeAt(PC.x2T, PC.y1T).Fire){PC.gotKilled = true;}
    if(GetTileTypeAt(PC.x3T, PC.y1T).Lethal && !GetTileTypeAt(PC.x3T, PC.y1T).Fire){PC.gotKilled = true;}
    if(GetTileTypeAt(PC.x1T, PC.y2T).Lethal && !GetTileTypeAt(PC.x1T, PC.y2T).Fire){PC.gotKilled = true;}
    if(GetTileTypeAt(PC.x2T, PC.y2T).Lethal && !GetTileTypeAt(PC.x2T, PC.y2T).Fire){PC.gotKilled = true;}
    if(GetTileTypeAt(PC.x3T, PC.y2T).Lethal && !GetTileTypeAt(PC.x3T, PC.y2T).Fire){PC.gotKilled = true;}
    if(GetTileTypeAt(PC.x1T, PC.y3T).Lethal && !GetTileTypeAt(PC.x1T, PC.y3T).Fire){PC.gotKilled = true;}
    if(GetTileTypeAt(PC.x2T, PC.y3T).Lethal && !GetTileTypeAt(PC.x2T, PC.y3T).Fire){PC.gotKilled = true;}
    if(GetTileTypeAt(PC.x3T, PC.y3T).Lethal && !GetTileTypeAt(PC.x3T, PC.y3T).Fire){PC.gotKilled = true;}
  }
  // CHECK FOR LETHAL COLLISIONS

  // CHECK FOR ENTRANCE TO SUB-STAGE
  PC.GotoSubStage = false;
  if(GetTileTypeAt(PC.x1T, PC.y1T).SubStageEntrance){PC.GotoSubStage = true;}
  if(GetTileTypeAt(PC.x2T, PC.y1T).SubStageEntrance){PC.GotoSubStage = true;}
  if(GetTileTypeAt(PC.x3T, PC.y1T).SubStageEntrance){PC.GotoSubStage = true;}
  if(GetTileTypeAt(PC.x1T, PC.y2T).SubStageEntrance){PC.GotoSubStage = true;}
  if(GetTileTypeAt(PC.x2T, PC.y2T).SubStageEntrance){PC.GotoSubStage = true;}
  if(GetTileTypeAt(PC.x3T, PC.y2T).SubStageEntrance){PC.GotoSubStage = true;}
  if(GetTileTypeAt(PC.x1T, PC.y3T).SubStageEntrance){PC.GotoSubStage = true;}
  if(GetTileTypeAt(PC.x2T, PC.y3T).SubStageEntrance){PC.GotoSubStage = true;}
  if(GetTileTypeAt(PC.x3T, PC.y3T).SubStageEntrance){PC.GotoSubStage = true;}
  // CHECK FOR ENTRANCE TO SUB-STAGE
}

// ##############################################
// ##############################################
// ##############################################

void Delete_Coin(int x, int y)
{
  if (x < 0 || x >= 256 || y < 0 || y >= 30) return;
  int currentTile = GetStageTile(x, y);
  // DELETE THE ENTIRE COIN
  if(GetStageTile(x-1, y) == currentTile-1){SetStageTile(x-1, y, 0);}
  if(GetStageTile(x+1, y) == currentTile+1){SetStageTile(x+1, y, 0);}
  if(GetStageTile(x, y-1) == currentTile-TS.Width){SetStageTile(x, y-1, 0);}
  if(GetStageTile(x, y+1) == currentTile+TS.Width){SetStageTile(x, y+1, 0);}
  if(GetStageTile(x-1, y-1) == currentTile-TS.Width-1){SetStageTile(x-1, y-1, 0);}
  if(GetStageTile(x+1, y-1) == currentTile-TS.Width+1){SetStageTile(x+1, y-1, 0);}
  if(GetStageTile(x-1, y+1) == currentTile+TS.Width-1){SetStageTile(x-1, y+1, 0);}
  if(GetStageTile(x+1, y+1) == currentTile+TS.Width+1){SetStageTile(x+1, y+1, 0);}
  SetStageTile(x, y, 0);
  // DELETE THE ENTIRE COIN
  AUDIO_Sound_Play(AUDIO_DING);
  PC.Coins += 1;
  if(PC.Coins > 99){PC.Coins = 0; PC.Lives++;}
}

// ##############################################
// ##############################################
// ##############################################

bool PC_Collision_Exit()
{
  bool ReturnValue = false;
  //PC.Exit_Reached = false;

  if(
     GetTileTypeAt((int)((PC.PosX-PC.ColWidthHalf)/TS.Tile_Width), (int)((PC.PosY-PC.ColHeight)/TS.Tile_Height)).Exit ||
     GetTileTypeAt((int)((PC.PosX+PC.ColWidthHalf)/TS.Tile_Width), (int)((PC.PosY-PC.ColHeight)/TS.Tile_Height)).Exit ||
     GetTileTypeAt((int)((PC.PosX+PC.ColWidthHalf)/TS.Tile_Width), (int)((PC.PosY-PC.ColHeightHalf)/TS.Tile_Height)).Exit ||
     GetTileTypeAt((int)((PC.PosX-PC.ColWidthHalf)/TS.Tile_Width), (int)((PC.PosY-PC.ColHeightHalf)/TS.Tile_Height)).Exit ||
     GetTileTypeAt((int)((PC.PosX-PC.ColWidthHalf)/TS.Tile_Width), (int)((PC.PosY-0)/TS.Tile_Height)).Exit ||
     GetTileTypeAt((int)((PC.PosX-PC.ColWidthHalf)/TS.Tile_Width), (int)((PC.PosY-0)/TS.Tile_Height)).Exit
    )
  {ReturnValue = true;}

  return ReturnValue;
}

// ##############################################
// ##############################################
// ##############################################
