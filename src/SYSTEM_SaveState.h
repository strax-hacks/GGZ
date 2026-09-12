#ifndef SYSTEM_SAVESTATE_H
#define SYSTEM_SAVESTATE_H

#include <string>
#include <cstdint>

#define SAVE_MAGIC "OGGS_SAV\x01"
#define SAVE_MAGIC_LEN 8
#define MAX_SAVE_SLOTS 5

struct SaveStateMetadata {
    bool exists;
    int slot;
    int stage;
    int score;
    int lives;
    int coins;
    uint32_t timestamp;
};

// Saves current game state to given slot (1..MAX_SAVE_SLOTS)
bool SaveState_Write(int slot);

// Loads game state from given slot (1..MAX_SAVE_SLOTS)
bool SaveState_Read(int slot);

// Queries metadata for a save slot
SaveStateMetadata SaveState_GetInfo(int slot);

// Quick Save / Quick Load helpers
bool SaveState_QuickSave();
bool SaveState_QuickLoad();

#endif // SYSTEM_SAVESTATE_H
