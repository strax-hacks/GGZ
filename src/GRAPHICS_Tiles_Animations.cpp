#include "globals.h"

namespace {

constexpr int MAX_ACTIVE_ANIMATION_CELLS = 256 * 30;

struct ActiveAnimationCell {
  unsigned short x;
  unsigned short y;
};

struct ActiveAnimationRegistry {
  ActiveAnimationCell cells[MAX_ACTIVE_ANIMATION_CELLS];
  int indices[256][30];
  int count;

  ActiveAnimationRegistry() : count(0)
  {
    for (int x = 0; x < 256; ++x) {
      for (int y = 0; y < 30; ++y) {
        indices[x][y] = -1;
      }
    }
  }
};

ActiveAnimationRegistry ActiveAnimations;

void RemoveActiveAnimationCell(int x, int y)
{
  const int index = ActiveAnimations.indices[x][y];
  if (index < 0) {
    return;
  }

  const int lastIndex = ActiveAnimations.count - 1;
  if (index != lastIndex) {
    ActiveAnimations.cells[index] = ActiveAnimations.cells[lastIndex];
    ActiveAnimations.indices[ActiveAnimations.cells[index].x][ActiveAnimations.cells[index].y] = index;
  }
  --ActiveAnimations.count;
  ActiveAnimations.indices[x][y] = -1;
}

} // namespace

void Stage_AnimationCounter_Set(int x, int y, int counter)
{
  if (x < 0 || x >= 256 || y < 0 || y >= 30) {
    return;
  }

  StageC64.AnimationCounter[x][y] = counter > 0 ? counter : 0;
  if (StageC64.AnimationCounter[x][y] == 0) {
    RemoveActiveAnimationCell(x, y);
    return;
  }

  if (ActiveAnimations.indices[x][y] >= 0) {
    return;
  }

  if (ActiveAnimations.count >= MAX_ACTIVE_ANIMATION_CELLS) {
    return;
  }

  ActiveAnimations.cells[ActiveAnimations.count] = {
    static_cast<unsigned short>(x), static_cast<unsigned short>(y)};
  ActiveAnimations.indices[x][y] = ActiveAnimations.count;
  ++ActiveAnimations.count;
}

void Stage_AnimationCounters_Reset()
{
  ActiveAnimations.count = 0;
  for (int x = 0; x < 256; ++x) {
    for (int y = 0; y < 30; ++y) {
      ActiveAnimations.indices[x][y] = -1;
    }
  }
  memset(StageC64.AnimationCounter, 0, sizeof(StageC64.AnimationCounter));
}

void Stage_Animation_Tick()
{
  int index = 0;
  while (index < ActiveAnimations.count) {
    const int x = ActiveAnimations.cells[index].x;
    const int y = ActiveAnimations.cells[index].y;

    if (StageC64.AnimationCounter[x][y] > 0) {
      --StageC64.AnimationCounter[x][y];
    }
    if (StageC64.AnimationCounter[x][y] == 0 && StageC64.TileNumber[x][y] != 0) {
      SwitchTileToNextFrame(x, y);
    }

    if (StageC64.AnimationCounter[x][y] == 0) {
      RemoveActiveAnimationCell(x, y);
      continue;
    }
    ++index;
  }
}

int Stage_Animation_ActiveCount()
{
  return ActiveAnimations.count;
}

// ##############################################
// ##############################################
// ##############################################

void SwitchTileToNextFrame(int x, int y)
{
  // CRUMBLING GROUND
       if(StageC64.TileNumber[x][y] > 567 && StageC64.TileNumber[x][y] < 572 && StageC64.AnimationCounter[x][y] == 0)  {StageC64.TileNumber[x][y] += 2; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  else if(StageC64.TileNumber[x][y] > 571 && StageC64.TileNumber[x][y] < 574 && StageC64.AnimationCounter[x][y] == 0) {StageC64.TileNumber[x][y] = 0;}



  // BLINK OF COIN BLOCK
  if(StageC64.TileNumber[x][y] == 563 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 587; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 564 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 588; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 565 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 589; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 603 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 627; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 604 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 628; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 605 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 629; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}



  // BREAKABLE BLOCK
  if(StageC64.TileNumber[x][y] == 507 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 0;}
  if(StageC64.TileNumber[x][y] == 508 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 0;}
  if(StageC64.TileNumber[x][y] == 509 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 0;}
  if(StageC64.TileNumber[x][y] == 547 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 0;}
  if(StageC64.TileNumber[x][y] == 548 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 0;}
  if(StageC64.TileNumber[x][y] == 549 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 0;}

  if(StageC64.TileNumber[x][y] == 504 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 507; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 505 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 508; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 506 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 509; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 544 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 547; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 545 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 548; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 546 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 549; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}



  // BLINK OF 5-COIN BLOCK
  if(StageC64.TileNumber[x][y] == 643 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3;Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 644 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3;Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 645 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3;Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 683 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3;Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 684 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3;Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 685 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3;Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}

  if(StageC64.TileNumber[x][y] == 649 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 650 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 651 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 689 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 690 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 691 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}

  if(StageC64.TileNumber[x][y] == 655 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 656 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 657 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 695 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 696 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 697 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}

  if(StageC64.TileNumber[x][y] == 661 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 662 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 663 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 701 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 702 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 703 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] += 3; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}

  if(StageC64.TileNumber[x][y] == 667 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 587; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 668 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 588; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 669 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 589; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 707 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 627; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 708 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 628; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
  if(StageC64.TileNumber[x][y] == 709 && StageC64.AnimationCounter[x][y] == 0){StageC64.TileNumber[x][y] = 629; Stage_AnimationCounter_Set(x, y, World.TileSwitchSpeed);}
}

// ##############################################
// ##############################################
// ##############################################
