#ifndef SYSTEM_POWER_H
#define SYSTEM_POWER_H

#include <cstdint>

// Initialize power monitoring subsystem
void Power_Init();

// Shutdown power monitoring subsystem
void Power_Shutdown();

// Polls battery status (non-blocking, updates internal state)
void Power_Update();

// Returns true if battery level is below low battery threshold (<15%)
bool Power_IsBatteryLow();

// Returns battery capacity percentage (0-100) or -1 if unknown / plugged into AC
int Power_GetBatteryPercentage();

// Returns true if the device is running on battery power
bool Power_IsOnBattery();

// Handles window focus loss / background suspension
void Power_OnFocusLost();

// Handles window focus regained / resume
void Power_OnFocusGained();

// Emergency save on SIGINT/SIGTERM or low power cutoff
void Power_EmergencySave();

// Frame pacing limiter: enforces target frame rate (default 60 FPS) to minimize CPU usage
void Power_FramePace(uint32_t targetFPS);

#endif // SYSTEM_POWER_H
