# Cardputer ZERO OpenGGS (`cardputerzero-openggs`)
## Production System & Engineering Specification

**Document Version:** 1.0.0  
**Target Platform:** M5Stack Cardputer ZERO (Raspberry Pi Compute Module 0 / Broadcom RP3A0 Cortex-A53 @ 1.0 GHz, 512MB RAM, ST7789v3 320×170 IPS LCD, 46-key mechanical tactile matrix keyboard)  
**Operating System:** `cardputer-zero-os` (Debian 12 Bookworm ARM64, Linux 6.12.y+, DRM/KMS, labwc Wayland compositor, APPLaunch launcher)  
**Upstream Engine:** [bugix/OpenGGS](https://github.com/bugix/OpenGGS) (C/C++ SDL2 remake of Commodore 64 *The Great Giana Sisters*)  

---

## Table of Contents
1. [System Architecture & Hardware Target](#1-system-architecture--hardware-target)
2. [Display & Viewport Pipeline (320×170 Adaptation)](#2-display--viewport-pipeline-320170-adaptation)
3. [HUD, Menus & Visual Filters](#3-hud-menus--visual-filters)
4. [Input Subsystem & 46-Key Mapping](#4-input-subsystem--46-key-mapping)
5. [Storage, XDG Compliance & Save State Architecture](#5-storage-xdg-compliance--save-state-architecture)
6. [Power, Battery & Handheld Lifecycle](#6-power-battery--handheld-lifecycle)
7. [Audio Subsystem & Optimization](#7-audio-subsystem--optimization)
8. [OpenGGS Maker / Level Editor Adaptation](#8-openggs-maker--level-editor-adaptation)
9. [Cardputer ZERO OS & AppStore Packaging](#9-cardputer-zero-os--appstore-packaging)
10. [Development, Emulation & CI/CD Pipeline](#10-development-emulation--cicd-pipeline)
11. [Project Milestones](#11-project-milestones)

---

## 1. System Architecture & Hardware Target

```text
┌────────────────────────────────────────────────────────────────────────┐
│                   M5Stack Cardputer ZERO Hardware                      │
│  RP3A0 (BCM2837) Quad-core A53 @ 1.0GHz | 512MB RAM | ES8389 Audio   │
│  1.9" IPS LCD (ST7789v3 320x170) | 46-Key Matrix Keyboard | LiPo PMIC  │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │
┌───────────────────────────────────▼────────────────────────────────────┐
│                    cardputer-zero-os (Base System)                     │
│  Kernel 6.12.y (rpi-cm0 DT) | DRM/KMS Framebuffer | ALSA/PulseAudio    │
│  labwc (Wayland Compositor) / Xwayland | Systemd Session Manager       │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │
┌───────────────────────────────────▼────────────────────────────────────┐
│                  cardputer-zero-shell / APPLaunch                      │
│  Scans /usr/share/APPLaunch/applications/*.desktop                     │
│  Launches native Debian packages with hardware acceleration            │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │
┌───────────────────────────────────▼────────────────────────────────────┐
│                  cardputerzero-openggs Engine Port                     │
│  • SDL2 Logical Viewport (320x170 @ 60 FPS integer nearest-neighbor)   │
│  • Direct 46-Key Scancode Engine + GameController Hot-Plug Subsystem   │
│  • XDG Data/Config Isolation (~/.config/openggs, ~/.local/share/)      │
│  • Sysfs Battery Monitor & Auto-Pause on Window Focus Loss            │
│  • Low-Latency 22.05kHz 16-bit SDL2_mixer Audio Pipeline               │
│  • 33 Full C64 Stages + Sub-levels + Boss Encounters + Level Editor    │
└────────────────────────────────────────────────────────────────────────┘
```

---

## 2. Display & Viewport Pipeline (320×170 Adaptation)

### Hardware Constraints
- **Panel:** 1.9-inch ST7789v3 IPS LCD.
- **Resolution:** 320 pixels horizontal × 170 pixels vertical.
- **Refresh Rate:** 60 Hz with VSync.

### Logical Viewport Configuration
Upstream OpenGGS hardcodes standard PC 4:3 resolutions (640×480, 800×600, 1280×720). In `cardputerzero-openggs`, the engine defaults to native `RESOLUTION_320x170`:

```cpp
// SYSTEM_SDL_init.cpp
gWindow = SDL_CreateWindow("OpenGGS C64",
                           SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                           320, 170,
                           SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);

gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
SDL_RenderSetLogicalSize(gRenderer, 320, 170);
SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest"); // Preserve sharp pixel-art
```

### Tile Grid & Camera Centering
- **Tile Dimensions:** 16 × 16 pixels.
- **Horizontal Tiles Rendered:** \( 320 / 16 = 20 \) tiles.
- **Vertical Tiles Rendered:** 10 playable rows (160 px) + 10 px HUD header.
- **Camera Tracking:** Horizontal camera smooth-scroll follows player position `Player.PosX`, maintaining Giana at \( X \approx 120 \) to provide advance sightline of upcoming obstacles and enemies.

---

## 3. HUD, Menus & Visual Filters

### HUD Layout (320 × 170)
The in-game HUD is compacted into a single 10–14px top bar across 320px width:

| Element | X Coordinate | Y Coordinate | Content Format |
| :--- | :--- | :--- | :--- |
| **Score / Player** | `X = 4` | `Y = 2` | `GIANA 012450` |
| **Coins / Diamonds** | `X = 92` | `Y = 2` | `[Diamond Icon] x18` |
| **Lives** | `X = 160` | `Y = 2` | `[Giana Icon] x03` |
| **Stage** | `X = 215` | `Y = 2` | `STAGE 01-1` |
| **Time** | `X = 276` | `Y = 2` | `TIME 142` |

### Visual Settings & Modes
1. **Pixel-Perfect 1:1:** Exact native 320×170 rendering.
2. **Retro Scanlines:** Optional 50% opacity scanline overlay simulation for classic CRT aesthetic.
3. **FPS & Performance Counter:** Optional corner overlay showing current FPS and frame render time (\(ms\)).

---

## 4. Input Subsystem & 46-Key Mapping

### Physical Keyboard Layout on Cardputer ZERO

```text
┌────────────────────────────────────────────────────────────────────────┐
│ [ESC] [ 1 ] [ 2 ] [ 3 ] [ 4 ] [ 5 ] [ 6 ] [ 7 ] [ 8 ] [ 9 ] [ 0 ] [BS] │
│ [TAB] [ Q ] [ W ] [ E ] [ R ] [ T ] [ Y ] [ U ] [ I ] [ O ] [ P ] [RET]│
│ [FN ] [ A ] [ S ] [ D ] [ F ] [ G ] [ H ] [ J ] [ K ] [ L ] [ ; ] [ ' ] │
│ [SHF] [ Z ] [ X ] [ C ] [ V ] [ B ] [ N ] [ M ] [ , ] [ . ] [ / ] [SPC] │
│ [CTL] [ALT] [SYM]          [LEFT] [DOWN] [ UP ] [RIGHT]                │
└────────────────────────────────────────────────────────────────────────┘
```

### Game Control Bindings

| Action | Primary Key | Secondary / Ergonomic Key | External Gamepad |
| :--- | :--- | :--- | :--- |
| **Move Left** | `Left Arrow` | `A` | D-Pad Left / Left Stick Left |
| **Move Right** | `Right Arrow` | `D` | D-Pad Right / Left Stick Right |
| **Duck / Down** | `Down Arrow` | `S` | D-Pad Down / Left Stick Down |
| **Jump** | `SPACE` / `Up Arrow` | `W` or `K` | `Button A` (South) |
| **Shoot / Fireball** | `ENTER` | `J` or `Right Shift` | `Button B` (East) / `Button X` |
| **Pause Game** | `P` | `Fn + P` | `Start` Button |
| **Quick Save** | `1` / `Fn + 1` | Slot 1 Save | `L1 / LB` |
| **Quick Load** | `2` / `Fn + 2` | Slot 1 Load | `R1 / RB` |
| **Help Overlay** | `H` / `Fn + H` | Displays Keymap | `Select` Button |
| **Exit / Menu** | `ESC` (Home Key) | — | Guide / Home Button |

---

## 5. Storage, XDG Compliance & Save State Architecture

### Directory Structure & Sandboxing
To ensure security, multi-user support, and compatibility with read-only root filesystems, all mutable state is stored in user directories:

| Component | Path | Permissions | Purpose |
| :--- | :--- | :--- | :--- |
| **Read-Only Data** | `/usr/share/openggs/data/` | `0755 root:root` | Sprites, tilesets, BGM, SFX, 33 original stages |
| **User Configuration** | `$XDG_CONFIG_HOME/openggs/` (`~/.config/openggs/`) | `0700 user:user` | `options.cfg`, `keybindings.cfg` |
| **User Progress & Saves** | `$XDG_DATA_HOME/openggs/` (`~/.local/share/openggs/`) | `0700 user:user` | `hiscores.dat`, `saves/slot_{1..5}.sav`, `custom_stages/` |

### Save State Format (`slot_X.sav`)
Binary serialization containing:
- Magic Header: `OGGS_SAV\x01`
- Game Type (C64 / Amiga / Enhanced)
- Stage ID & Sub-level index
- Score, Lives, Diamonds collected, Power-up weapon level
- Player position \((X, Y)\), velocity \((v_x, v_y)\), facing direction
- Tile destruction bitmask for current stage
- Active enemies state array

---

## 6. Power, Battery & Handheld Lifecycle

### 1. Battery Monitoring
- Engine spawns a non-blocking background thread reading `/sys/class/power_supply/axp2101-battery/capacity` or PMIC status every 10 seconds.
- When battery level drops below 15%, a subtle flashing battery icon appears on the HUD.

### 2. Auto-Pause & Background Suspend
- Listens to Wayland / Xwayland focus loss events (`SDL_WINDOWEVENT_FOCUS_LOST`, `SDL_WINDOWEVENT_MINIMIZED`).
- On focus loss or when APPLaunch switches applications, the engine immediately pauses gameplay, mutes audio channels, and yields CPU cycles (`SDL_Delay(50)`).
- On `SIGTERM` / `SIGINT` (e.g. system shutdown), the engine writes an emergency quick-save (`auto_resume.sav`) before terminating cleanly.

### 3. Frame Rate Limiting & CPU Optimization
- Enforces strict 60.0 FPS frame pacing with microsecond sleep precision (`SDL_GetPerformanceCounter()`).
- Keeps CPU load on the quad-core Cortex-A53 below **12%**, ensuring minimal thermal generation and 4+ hours of battery life.

---

## 7. Audio Subsystem & Optimization (Core P1 Requirement)

- **Driver:** `SDL2_mixer` configured for low-latency ALSA / PulseAudio pipeline.
- **Audio Spec:** `22,050 Hz` (or `44,100 Hz`), `AUDIO_S16SYS` (16-bit signed), Stereo (2 channels), Chunk size: `1024 bytes` (~23–46ms latency).
- **Soundtracks:** Chiptune OGG streams for Title, Outdoor, Underground, and Boss themes (Original C64 / OpenGGS soundtrack by puch666 / Chris Hülsbeck compositions) with zero stutter and seamless loop points.
- **Sound Effects:** Complete collection of 16 pre-cached WAV sound chunks (jump, diamond pickup, brick break, fire shoot, enemy hit, power-up transformation, extra life, game over, stage clear).
- **Hardware Integration:** Calibrated output for the onboard ES8389 codec, AW8737A speaker amp (1W @ 8Ω), and 3.5mm TRRS headphone jack with auto-detection.
- **Volume & Control:** Dedicated software volume sliders for Music and SFX in settings, with instantaneous mute/unmute on pause and lifecycle suspend.

---

## 8. OpenGGS Maker / Level Editor (Deferred)

*Status: **Deferred** from on-device handheld deployment to minimize package size, eliminate mouse dependencies, and keep the handheld UI focused strictly on arcade gameplay.*
- The level editor source code remains modularized and can be built for desktop targets if needed, but is omitted from the Cardputer ZERO device launcher and binary build.

---

## 9. Cardputer ZERO OS & AppStore Packaging

### Debian Package Layout (`cardputerzero-openggs_1.0.0-1_arm64.deb`)

```text
cardputerzero-openggs_1.0.0-1_arm64.deb
├── DEBIAN/
│   ├── control
│   ├── postinst
│   └── prerm
└── usr/
    └── share/
        ├── APPLaunch/
        │   ├── applications/
        │   │   └── openggs.desktop
        │   └── share/
        │       └── images/
        │           └── openggs.png
        └── openggs/
            ├── bin/
            │   └── openggs-cz
            └── data/
                ├── base/
                │   ├── audio/
                │   ├── gfx/
                │   ├── stages/
                │   └── fonts/
                └── licenses/
                    └── GPL-2.0.txt
```

### CardputerZero AppStore Registry Entry (`registry.json`)
```json
{
  "id": "cardputerzero-openggs",
  "name": "OpenGGS C64",
  "version": "1.0.0-1",
  "category": "Games",
  "author": "OpenGGS Team / CardputerZero Community",
  "license": "GPL-2.0",
  "description": "The Great Giana Sisters C64 remake natively ported for M5Stack Cardputer ZERO.",
  "icon": "https://cardputerzero.github.io/packages/icons/openggs.png",
  "download": {
    "type": "deb",
    "url": "https://github.com/CardputerZero/packages/raw/main/pool/main/cardputerzero-openggs_1.0.0-1_arm64.deb",
    "md5": "<computed_md5_hash>"
  }
}
```

---

## 10. Development, Emulation & CI/CD Pipeline

### Multi-Tier Emulation Matrix
1. **Desktop SDL2 Simulator:** Native host build (macOS / Linux) rendering in a 320×170 emulated window with simulated 46-key matrix (`./scripts/run-desktop.sh`).
2. **Official `czdev` / `AppBuilder` Flow:** Builds using `app-builder.json` manifest and runs under `CardputerZero/Emulator`.
3. **QEMU ARM64 System Image Runner:** Full OS emulation booting `cardputer-zero-os` Bookworm kernel with virtual DRM display and SSH port forwarding (`./scripts/run-qemu.sh`).

### Headless Smoke Test & CI Matrix
- Automated test script `tests/smoke_test.sh`: boots engine with `SDL_VIDEODRIVER=dummy` and `SDL_AUDIODRIVER=dummy`, runs 100 frames through Stage 1, tests save-state serialization, and exits cleanly with return code `0`.
- GitHub Actions CI workflow (`.github/workflows/build-and-package.yml`):
  - Step 1: Desktop build & headless smoke test.
  - Step 2: ARM64 cross-compilation with toolchain.
  - Step 3: Debian package generation & verification.
  - Step 4: Automated release asset publishing.

---

## 11. Project Milestones

| Milestone ID | Area | Title | Target Device | Status |
| :--- | :--- | :--- | :--- | :--- |
| **`M1`** | Harness | Setup Cardputer Zero App Template Runner and build harness | Desktop / CP0 | Complete |
| **`M2`** | Engine | Ingest upstream bugix/OpenGGS C64 game codebase and assets | Core Engine | Complete |
| **`M3`** | Graphics | Implement native 320x170 display resolution and viewport scaling | ST7789v3 | Complete |
| **`M4`** | UI/HUD | Redesign HUD layout and menus for 320x170 screen geometry | ST7789v3 | Complete |
| **`M5`** | Controls | Adapt input system for Cardputer ZERO 46-key matrix keyboard | Matrix Keyboard | Complete |
| **`M6`** | Audio | Optimize audio pipeline for BCM2837 / ES8389 on Cardputer ZERO | ES8389 DAC | Complete |
| **`M7`** | Packaging | Create APPLaunch .desktop integration, icon, and Debian packaging (.deb) | Debian ARM64 | Complete |
| **`M8`** | CI / QA | Build validation suite: headless smoke tests and CI build matrix | GitHub Actions | Complete |
| **`M9`** | Tooling | Setup CardputerZero official czdev emulator runtime integration | Emulator | Complete |
| **`M10`** | Tooling | Setup QEMU ARM64 cardputer-zero-os system image runner | QEMU | Complete |
| **`M11`** | Storage | Implement XDG-compliant persistent storage and portable save-states | Linux Filesystem | Complete |
| **`M12`** | Power/OS | Implement battery monitoring, power management, and suspend/resume | PMIC / Sysfs | Complete |
| **`M13`** | Input | Implement SDL2 GameController hot-plugging & external gamepads | GameController | Complete |
| **`M14`** | UI/Settings | Implement on-device settings menu, visual filters, and help overlay | ST7789v3 | Complete |
| **`M15`** | Level Editor| Adapt OpenGGS Maker / Level Editor for 320x170 keyboard navigation | Handheld Editor | Deferred |
| **`M16`** | Store/Pub | Generate CardputerZero Store registry entry and distribution assets | App Store | Complete |
