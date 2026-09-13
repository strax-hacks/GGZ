#include "globals.h"

// ##############################################
// ##############################################
// ##############################################

void Check_Switch_Tile_Down(int x, int y)
{
  if (x < 0 || x >= 256 || y < 0 || y >= 30) return;
  if(StageC64.TileNumber[x][y] == 566){StageC64.TileNumber[x][y] = 568; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 567){StageC64.TileNumber[x][y] = 569; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
}

// ##############################################
// ##############################################
// ##############################################
