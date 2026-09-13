# Cardputer ZERO OpenGGS (`cardputerzero-openggs`)

[![Build & Test](https://github.com/strax-hacks/GGZ/actions/workflows/build-and-package.yml/badge.svg)](https://github.com/strax-hacks/GGZ/actions)

A native port and enhanced fork of **OpenGGS** (the open-source C64 *The Great Giana Sisters* remake) engineered specifically for the **M5Stack Cardputer ZERO** (Raspberry Pi Compute Module 0 / BCM2837 ARM64, 320×170 IPS LCD, 46-key mechanical tactile keyboard) and modern desktop systems.

Full technical details and architecture specifications are documented in [docs/SPECIFICATION.md](docs/SPECIFICATION.md).

---

## Features

- **Dual Viewport Modes (320×170):**
  - **C64 Scaled (Default / Authentic):** Full 640×340 stage viewport scaled to 320×170, matching the original Commodore 64 visible stage height and layout on CRT displays.
  - **1:1 Zoomed Mode:** Crisp unscaled pixel-for-pixel rendering with dynamic smooth camera tracking centered on Giana.
  - Switch view modes on-the-fly anytime by pressing `V` or toggling in the Options menu.
- **Authentic C64 Intro Sequence & Stage 0:**
  - Classic autoscrolling intro panorama with retro GIANA SISTERS diamond-brick layout, color cycling, and title chiptune.
  - Direct quick-launch shortcuts from the intro screen (`1` to Start Game, `2`/`M` for Main Menu, `E` for Editor, `H` for Highscores, `V` to toggle Viewport mode).
- **MOS 6581 SID Audio Pipeline:**
  - Complete sound effects and chiptune module soundtracks (`Title`, `Outdoors / Stage 1`, `Indoors / Caves`, `Highscore`, `Ready`).
  - Low-latency `SDL2_mixer` audio engine configured for the Cardputer ZERO ES8389 DAC and desktop audio devices.
- **Cardputer ZERO 46-Key Controls & Gamepad Support:**
  - Tuned keyboard matrix handling (`Arrows`/`WASD` for navigation, `SPACE`/`UP`/`W`/`K` for jump, `ENTER`/`J`/`RSHIFT` for fire/action).
  - Full USB and Bluetooth `SDL_GameController` hot-plugging.
- **XDG-Compliant Save States & Storage:**
  - Multi-slot quick-save/load engine (Slots 1–5) and auto-save on stage transitions stored in `$XDG_DATA_HOME/openggs/` (`~/.local/share/openggs/`).
  - Persistent game settings and keymaps stored in `$XDG_CONFIG_HOME/openggs/` (`~/.config/openggs/`).
- **Power & Battery Management:**
  - Real-time battery status monitoring via Linux `/sys/class/power_supply` subsystem.
  - Auto-pause on focus loss and efficient 60 FPS pacing.
- **Automated QA & Test Suite:**
  - Headless smoke testing and multi-frame screenshot capture suite integrated with CTest.

---

## Controls

| Action | Cardputer ZERO Keyboard | External Gamepad |
| :--- | :--- | :--- |
| **Move Left / Right** | `Left` / `Right` or `A` / `D` | `D-Pad Left` / `Right` or `Left Stick` |
| **Look Down / Crouch** | `Down` or `S` | `D-Pad Down` |
| **Jump** | `Space`, `Up`, `W`, or `K` | `A` Button (South) |
| **Shoot / Fire** | `Enter`, `J`, or `Right Shift` | `B` Button (East) / `X` Button |
| **Toggle View Mode** | `V` | `Y` Button (North) |
| **Pause Game** | `P` | `Start` Button |
| **Menu / Back** | `Esc` | `Back` / `Select` Button |
| **Quick Save** | `F5` | — |
| **Quick Load** | `F6` | — |

---

## Building & Running

### Requirements
- C++17 compiler (GCC 9+, Clang 10+, or Apple Clang)
- CMake 3.16+
- SDL2, SDL2_image, SDL2_mixer
- Pkg-config

### Quick Start (Desktop Simulator)
```bash
# Clone the repository
git clone https://github.com/strax-hacks/GGZ.git
cd GGZ

# Run the desktop development simulator (builds and launches in 320x170 preview window)
./scripts/run-desktop.sh
```

### Building with CMake Presets
```bash
# Configure & build desktop simulator
cmake --preset desktop-dev
cmake --build --preset desktop-dev

# Run automated test suite
ctest --test-dir build/desktop --output-on-failure
```

### Cross-Compiling for Cardputer ZERO (ARM64)
```bash
# Configure and build ARM64 release binary
cmake --preset cp0-arm64-cross
cmake --build --preset cp0-arm64-cross

# Package Debian .deb package for Cardputer ZERO
./packaging/pack-deb.sh
# Artifact: dist/cardputerzero-openggs_1.0.0-1_arm64.deb
```

---

## Project Structure

```text
├── base/                   # Game assets (sprites, tiles, font, chiptunes, sound effects)
│   ├── amiga/              # MOD music modules
│   ├── audio/              # Synthesized SID sound effects (.wav)
│   ├── c64/                # C64 tilesets and palette assets
│   └── music/              # ImpulseTracker (.it) and MOD (.mod) music tracks
├── docs/                   # Architecture and technical specifications
├── packaging/              # APPLaunch desktop entry, icon, and Debian control files
├── scripts/                # Launch scripts and asset generation tools
├── src/                    # Core OpenGGS C++ game engine source code
└── tests/                  # Headless smoke test and frame capture test suite
```

---

## License

OpenGGS is distributed under the GNU General Public License v2 (GPL-2.0). See upstream project details at [bugix/OpenGGS](https://github.com/bugix/OpenGGS).
