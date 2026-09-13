# Project Instructions for AI Agents

This file provides instructions and context for AI coding agents working on this project.

## Build & Test

```bash
# Configure & build desktop simulator
cmake --preset desktop-dev
cmake --build --preset desktop-dev

# Run automated tests
ctest --test-dir build/desktop --output-on-failure
```

## Architecture Overview

Cardputer ZERO OpenGGS (`cardputerzero-openggs`) is a native C++17 port of OpenGGS for the M5Stack Cardputer ZERO (Raspberry Pi CM0 / BCM2837 ARM64, 320x170 ST7789v3 IPS display, 46-key matrix keyboard) and modern desktop systems.

- **Audio:** SDL2_mixer SID chiptune engine
- **Display:** 320x170 dual viewport (C64 scaled and 1:1 zoomed mode)
- **Input:** 46-key matrix and SDL_GameController
- **Storage:** XDG Base Directory isolation (`~/.config/openggs`, `~/.local/share/openggs`)

## Conventions & Patterns

- Preserve C++17 compatibility and SDL2 subsystem abstractions.
- Follow XDG Base Directory specification for writable file locations.
- Ensure headless test harness (`smoke_test`, `capture_screens`) passes across all changes.
