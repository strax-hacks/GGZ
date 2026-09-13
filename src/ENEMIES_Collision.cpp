#include "globals.h"

bool ENEMY_Collision_Down(int EnemyNumber);
bool ENEMY_Collision_Left(int EnemyNumber);
bool ENEMY_Collision_Right(int EnemyNumber);

// ##############################################
// ##############################################
// ##############################################

void ENEMY_Check_Tilecollision(int EnemyNumber)
{
  if (EnemyNumber < 1 || EnemyNumber >= MAX_NUM_ENEMIES || !Enemy[EnemyNumber].inUse) {
    return;
  }

  Enemy[EnemyNumber].OnGround = false;

  // CHECK IF ENEMY STANDS ON GROUND
  if(ENEMY_Collision_Down(EnemyNumber))
  {
    Enemy[EnemyNumber].OnGround = true;
    Enemy[EnemyNumber].PosY -= (Enemy[EnemyNumber].PosY % TS.Tile_Height);
  }

  if(Enemy[EnemyNumber].Direction == NPC_LEFT)
  {
    if(ENEMY_Collision_Left(EnemyNumber))
    {
      ENEMY_Change_Direction(EnemyNumber);
    }
  }

  if(Enemy[EnemyNumber].Direction == NPC_RIGHT)
  {
    if(ENEMY_Collision_Right(EnemyNumber))
    {
      ENEMY_Change_Direction(EnemyNumber);
    }
  }

}

// ##############################################
// ##############################################
// ##############################################

bool ENEMY_Collision_Down(int EnemyNumber)
{
  bool ReturnValue = false;

  if (EnemyNumber < 1 || EnemyNumber >= MAX_NUM_ENEMIES || !Enemy[EnemyNumber].inUse) {
    return false;
  }

  int type = Enemy[EnemyNumber].Type;
  if (type < 0 || type >= 30) {
    return false;
  }

  if(Enemy[EnemyNumber].JumpVelocity < 0)
  {
    int tileLeft = (int)((Enemy[EnemyNumber].PosX - Enemy_Type[type].ColWidthHalf) / TS.Tile_Width);
    int tileCenter = (int)(Enemy[EnemyNumber].PosX / TS.Tile_Width);
    int tileRight = (int)((Enemy[EnemyNumber].PosX + Enemy_Type[type].ColWidthHalf) / TS.Tile_Width);
    int tileY = (int)(Enemy[EnemyNumber].PosY / TS.Tile_Height);

    if(GetTileTypeAt(tileLeft, tileY).Solid ||
       GetTileTypeAt(tileCenter, tileY).Solid ||
       GetTileTypeAt(tileRight, tileY).Solid)
    {
      ReturnValue = true;
    }
  }

  // DON'T STOP JUMPERS!
  if(Enemy_Type[type].JumpStrength > 0){ReturnValue = false;}

  return ReturnValue;
}

// ##############################################
// ##############################################
// ##############################################

bool ENEMY_Collision_Left(int EnemyNumber)
{
  bool ReturnValue = false;

  if (EnemyNumber < 1 || EnemyNumber >= MAX_NUM_ENEMIES || !Enemy[EnemyNumber].inUse) {
    return false;
  }

  int type = Enemy[EnemyNumber].Type;
  if (type < 0 || type >= 30) {
    return false;
  }

  int tileLeft = (int)((Enemy[EnemyNumber].PosX - Enemy_Type[type].ColWidthHalf) / TS.Tile_Width);
  int tileCenter = (int)(Enemy[EnemyNumber].PosX / TS.Tile_Width);
  int tileRight = (int)((Enemy[EnemyNumber].PosX + Enemy_Type[type].ColWidthHalf) / TS.Tile_Width);
  int tileY = (int)(Enemy[EnemyNumber].PosY / TS.Tile_Height);

  // CHECK FOR CLIFFS
  if(!GetTileTypeAt(tileLeft, tileY).Solid &&
      GetTileTypeAt(tileCenter, tileY).Solid &&
      GetTileTypeAt(tileRight, tileY).Solid &&
      Enemy_Type[type].Walker &&
     !Enemy_Type[type].FallOfCliffs)
  {
    ReturnValue = true;
  }

  // CHECK FOR WALLS
  int wallTileX = (int)((Enemy[EnemyNumber].PosX - Enemy_Type[type].ColWidthHalf - 1) / TS.Tile_Width);
  int wallBottomY = (int)(Enemy[EnemyNumber].PosY / TS.Tile_Height) - 1;
  int wallTopY = (int)((Enemy[EnemyNumber].PosY - Enemy_Type[type].ColHeight) / TS.Tile_Height);

  if(GetTileTypeAt(wallTileX, wallBottomY).Solid)   // CHECK BOTTOM-LEFT
  {
    ReturnValue = true;
  }

  if(GetTileTypeAt(wallTileX, wallTopY).Solid)   // CHECK TOP-LEFT
  {
    ReturnValue = true;
  }

  return ReturnValue;
}

// ##############################################
// ##############################################
// ##############################################

bool ENEMY_Collision_Right(int EnemyNumber)
{
  bool ReturnValue = false;

  if (EnemyNumber < 1 || EnemyNumber >= MAX_NUM_ENEMIES || !Enemy[EnemyNumber].inUse) {
    return false;
  }

  int type = Enemy[EnemyNumber].Type;
  if (type < 0 || type >= 30) {
    return false;
  }

  int tileLeft = (int)((Enemy[EnemyNumber].PosX - Enemy_Type[type].ColWidthHalf) / TS.Tile_Width);
  int tileCenter = (int)(Enemy[EnemyNumber].PosX / TS.Tile_Width);
  int tileRight = (int)((Enemy[EnemyNumber].PosX + Enemy_Type[type].ColWidthHalf) / TS.Tile_Width);
  int tileY = (int)(Enemy[EnemyNumber].PosY / TS.Tile_Height);

  // CHECK FOR CLIFFS
  if( GetTileTypeAt(tileLeft, tileY).Solid &&
      GetTileTypeAt(tileCenter, tileY).Solid &&
     !GetTileTypeAt(tileRight, tileY).Solid &&
      Enemy_Type[type].Walker &&
     !Enemy_Type[type].FallOfCliffs)
  {
    ReturnValue = true;
  }

  // CHECK FOR WALLS
  int wallTileX = (int)((Enemy[EnemyNumber].PosX + Enemy_Type[type].ColWidthHalf + 1) / TS.Tile_Width);
  int wallBottomY = (int)(Enemy[EnemyNumber].PosY / TS.Tile_Height) - 1;
  int wallTopY = (int)((Enemy[EnemyNumber].PosY - Enemy_Type[type].ColHeight) / TS.Tile_Height);

  if(GetTileTypeAt(wallTileX, wallBottomY).Solid)   // CHECK BOTTOM-LEFT
  {
    ReturnValue = true;
  }

  if(GetTileTypeAt(wallTileX, wallTopY).Solid)   // CHECK TOP-LEFT
  {
    ReturnValue = true;
  }

  return ReturnValue;
}

// ##############################################
// ##############################################
// ##############################################
