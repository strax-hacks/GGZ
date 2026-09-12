#include "SYSTEM_Power.h"
#include "SYSTEM_SaveState.h"
#include "globals.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <thread>

static int s_batteryPercentage = 100;
static bool s_isOnBattery = false;
static bool s_isLowBattery = false;
static uint32_t s_lastBatteryCheckTime = 0;
static uint64_t s_lastFrameTime = 0;

void Power_Init() {
    s_batteryPercentage = 100;
    s_isOnBattery = false;
    s_isLowBattery = false;
    s_lastBatteryCheckTime = 0;
    s_lastFrameTime = SDL_GetPerformanceCounter();
    Power_Update();
}

void Power_Shutdown() {
    // Clean up power resources if needed
}

void Power_Update() {
    uint32_t now = SDL_GetTicks();
    // Check battery every 10 seconds to avoid filesystem / sysfs I/O thrashing
    if (s_lastBatteryCheckTime != 0 && (now - s_lastBatteryCheckTime) < 10000) {
        return;
    }
    s_lastBatteryCheckTime = now;

    // Check Linux sysfs battery supply (Cardputer ZERO AXP2101 PMIC or standard power_supply)
    const char* capacityPaths[] = {
        "/sys/class/power_supply/axp2101-battery/capacity",
        "/sys/class/power_supply/battery/capacity",
        "/sys/class/power_supply/BAT0/capacity",
        nullptr
    };

    bool found = false;
    for (int i = 0; capacityPaths[i] != nullptr; ++i) {
        FILE* fp = fopen(capacityPaths[i], "r");
        if (fp) {
            int cap = 100;
            if (fscanf(fp, "%d", &cap) == 1) {
                s_batteryPercentage = cap;
                s_isOnBattery = true;
                found = true;
            }
            fclose(fp);
            break;
        }
    }

    if (!found) {
        // Desktop or non-sysfs environment: report 100% and not on low battery
        s_batteryPercentage = 100;
        s_isOnBattery = false;
    }

    s_isLowBattery = (s_isOnBattery && s_batteryPercentage < 15);
}

bool Power_IsBatteryLow() {
    return s_isLowBattery;
}

int Power_GetBatteryPercentage() {
    return s_batteryPercentage;
}

bool Power_IsOnBattery() {
    return s_isOnBattery;
}

void Power_OnFocusLost() {
    // Mute or reduce volume, delay to yield CPU
    Mix_PauseMusic();
    Mix_Pause(-1);
}

void Power_OnFocusGained() {
    Mix_ResumeMusic();
    Mix_Resume(-1);
}

void Power_EmergencySave() {
    if (GV.Mode == MODE_GAMELOOP) {
        // Save to slot 5 as emergency quick save
        SaveState_Write(5);
    }
}

void Power_FramePace(uint32_t targetFPS) {
    if (targetFPS == 0) targetFPS = 60;
    uint64_t freq = SDL_GetPerformanceFrequency();
    uint64_t targetCountsPerFrame = freq / targetFPS;

    uint64_t now = SDL_GetPerformanceCounter();
    uint64_t elapsed = now - s_lastFrameTime;

    if (elapsed < targetCountsPerFrame) {
        uint64_t remainingCounts = targetCountsPerFrame - elapsed;
        uint32_t sleepMs = static_cast<uint32_t>((remainingCounts * 1000) / freq);
        if (sleepMs > 0) {
            SDL_Delay(sleepMs);
        }
        // Fine-grained busy-wait for remaining microseconds to ensure jitter-free 60fps
        while ((SDL_GetPerformanceCounter() - s_lastFrameTime) < targetCountsPerFrame) {
            // yield CPU thread
            std::this_thread::yield();
        }
    }
    s_lastFrameTime = SDL_GetPerformanceCounter();
}
