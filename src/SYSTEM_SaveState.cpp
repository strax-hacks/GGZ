#include "SYSTEM_SaveState.h"
#include "SYSTEM_Paths.h"
#include "globals.h"
#include <cstdio>
#include <cstring>
#include <ctime>

#pragma pack(push, 1)
struct SaveStateHeader {
    char magic[SAVE_MAGIC_LEN];
    uint32_t version;
    uint32_t timestamp;
    int32_t gameType;
    int32_t stage;
    int32_t subStageNumber;
    int32_t points;
    int32_t lives;
    int32_t coins;
    int32_t powerUp;
    int32_t mode;
    int32_t posX;
    int32_t posY;
    float runVelocity;
    float jumpVelocity;
    int32_t direction;
    int32_t stagePosX;
    int32_t backgroundColour;
    int32_t tileType;
    int32_t stageWidth;
    int32_t stageHeight;
};
#pragma pack(pop)

static std::string GetSlotFilename(int slot) {
    char buf[64];
    snprintf(buf, sizeof(buf), "slot_%d.sav", slot);
    return GetSavePath(buf);
}

bool SaveState_Write(int slot) {
    if (slot < 1 || slot > MAX_SAVE_SLOTS) {
        return false;
    }

    std::string path = GetSlotFilename(slot);
    FILE* fp = fopen(path.c_str(), "wb");
    if (!fp) {
        return false;
    }

    SaveStateHeader header;
    memset(&header, 0, sizeof(header));
    memcpy(header.magic, SAVE_MAGIC, SAVE_MAGIC_LEN);
    header.version = 1;
    header.timestamp = static_cast<uint32_t>(time(nullptr));
    header.gameType = GV.GameType;
    header.stage = PC.Stage;
    header.subStageNumber = PC.SubStageNumber;
    header.points = PC.Points;
    header.lives = PC.Lives;
    header.coins = PC.Coins;
    header.powerUp = PC.PowerUp;
    header.mode = PC.Mode;
    header.posX = PC.PosX;
    header.posY = PC.PosY;
    header.runVelocity = PC.RunVelocity;
    header.jumpVelocity = PC.JumpVelocity;
    header.direction = PC.Direction;
    header.stagePosX = PC.StagePosX;
    header.backgroundColour = StageC64.BackgroundColour;
    header.tileType = StageC64.TileType;
    header.stageWidth = StageC64.StageWidth;
    header.stageHeight = StageC64.StageHeight;

    if (fwrite(&header, sizeof(header), 1, fp) != 1) {
        fclose(fp);
        return false;
    }

    // Write tile data: 256x30
    if (fwrite(StageC64.TileNumber, sizeof(StageC64.TileNumber), 1, fp) != 1) {
        fclose(fp);
        return false;
    }

    // Write enemies data
    if (fwrite(Enemy, sizeof(Enemy), 1, fp) != 1) {
        fclose(fp);
        return false;
    }

    // Write platform data
    if (fwrite(&Platform, sizeof(Platform), 1, fp) != 1) {
        fclose(fp);
        return false;
    }

    fclose(fp);
    AUDIO_Sound_Play(AUDIO_DISK);
    return true;
}

bool SaveState_Read(int slot) {
    if (slot < 1 || slot > MAX_SAVE_SLOTS) {
        return false;
    }

    std::string path = GetSlotFilename(slot);
    FILE* fp = fopen(path.c_str(), "rb");
    if (!fp) {
        return false;
    }

    SaveStateHeader header;
    if (fread(&header, sizeof(header), 1, fp) != 1) {
        fclose(fp);
        return false;
    }

    if (memcmp(header.magic, SAVE_MAGIC, SAVE_MAGIC_LEN) != 0) {
        fclose(fp);
        return false;
    }

    // Restore state
    GV.GameType = header.gameType;
    PC.Stage = header.stage;
    PC.SubStageNumber = header.subStageNumber;
    PC.Points = header.points;
    PC.Lives = header.lives;
    PC.Coins = header.coins;
    PC.PowerUp = header.powerUp;
    PC.Mode = header.mode;
    PC.PosX = header.posX;
    PC.PosY = header.posY;
    PC.RunVelocity = header.runVelocity;
    PC.JumpVelocity = header.jumpVelocity;
    PC.Direction = header.direction;
    PC.StagePosX = header.stagePosX;
    StageC64.BackgroundColour = header.backgroundColour;
    StageC64.TileType = header.tileType;
    StageC64.StageWidth = header.stageWidth;
    StageC64.StageHeight = header.stageHeight;

    if (fread(StageC64.TileNumber, sizeof(StageC64.TileNumber), 1, fp) != 1) {
        fclose(fp);
        return false;
    }

    if (fread(Enemy, sizeof(Enemy), 1, fp) != 1) {
        fclose(fp);
        return false;
    }

    if (fread(&Platform, sizeof(Platform), 1, fp) != 1) {
        fclose(fp);
        return false;
    }

    fclose(fp);
    AUDIO_Sound_Play(AUDIO_GOTPOWERUP);
    return true;
}

SaveStateMetadata SaveState_GetInfo(int slot) {
    SaveStateMetadata meta;
    memset(&meta, 0, sizeof(meta));
    meta.slot = slot;
    meta.exists = false;

    if (slot < 1 || slot > MAX_SAVE_SLOTS) {
        return meta;
    }

    std::string path = GetSlotFilename(slot);
    FILE* fp = fopen(path.c_str(), "rb");
    if (!fp) {
        return meta;
    }

    SaveStateHeader header;
    if (fread(&header, sizeof(header), 1, fp) == 1 &&
        memcmp(header.magic, SAVE_MAGIC, SAVE_MAGIC_LEN) == 0) {
        meta.exists = true;
        meta.stage = header.stage;
        meta.score = header.points;
        meta.lives = header.lives;
        meta.coins = header.coins;
        meta.timestamp = header.timestamp;
    }

    fclose(fp);
    return meta;
}

bool SaveState_QuickSave() {
    return SaveState_Write(1);
}

bool SaveState_QuickLoad() {
    return SaveState_Read(1);
}
