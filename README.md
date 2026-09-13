# Cardputer ZERO OpenGGS (`cardputerzero-openggs`)

[![Build & Test](https://github.com/strax-hacks/GGZ/actions/workflows/build-and-package.yml/badge.svg)](https://github.com/strax-hacks/GGZ/actions/workflows/build-and-package.yml)
[![GitHub Release](https://img.shields.io/github/v/release/strax-hacks/GGZ?include_prereleases&label=Latest%20Release)](https://github.com/strax-hacks/GGZ/releases)

A native port and enhanced fork of **OpenGGS** (the open-source C64 *The Great Giana Sisters* remake) engineered specifically for the **M5Stack Cardputer ZERO** (Raspberry Pi Compute Module 0 / BCM2837 ARM64, 320×170 IPS LCD, 46-key mechanical tactile keyboard) and modern desktop systems.

> **Note on Hardware Testing:** This port has been engineered and validated using the desktop simulator harness and headless automated test suites, but is **yet to be tested on a real physical Cardputer ZERO device**. Reports, testing feedback, and pull requests from real hardware testing are warmly welcomed!

Full technical details and architecture specifications are documented in [docs/SPECIFICATION.md](docs/SPECIFICATION.md).

---

## ⬇️ Download Pre-Built Cardputer ZERO App (No Compilation Required)

You don't need to build from source! The ready-to-install Cardputer ZERO package is automatically built and published under GitHub Releases:

👉 **[Download the Latest Cardputer ZERO `.deb` on GitHub Releases](https://github.com/strax-hacks/GGZ/releases)**

### 🎮 How to Copy & Install on M5Stack Cardputer ZERO

The exact file you need is: **`cardputerzero-openggs_1.0.0-1_arm64.deb`**

#### Method A: Direct Download on Device
```bash
# 1. Download the .deb package
wget https://github.com/strax-hacks/GGZ/releases/latest/download/cardputerzero-openggs_1.0.0-1_arm64.deb

# 2. Install the package
sudo dpkg -i cardputerzero-openggs_1.0.0-1_arm64.deb || sudo apt-get install -f -y

# 3. Launch from APPLaunch Games menu or terminal
openggs-cz
```

#### Method B: Copy from PC / Mac via SCP or SD Card
```bash
# Copy over Wi-Fi
scp cardputerzero-openggs_1.0.0-1_arm64.deb root@cardputer.local:/tmp/

# SSH and install
ssh root@cardputer.local "dpkg -i /tmp/cardputerzero-openggs_1.0.0-1_arm64.deb"
```

---

## Key Changes & Enhancements vs Upstream OpenGGS

Compared to the upstream OpenGGS implementation, this version includes several major gameplay, visual, and architectural changes:

1. **Scrolling Intro Screen with Original Music:**
   - Implemented the iconic, previously missing Commodore 64 scrolling intro sequence (Stage 0).
   - Features retro "GIANA SISTERS" block typography crafted from diamond and brick tiles, green raster banner text, smooth horizontal autoscrolling, and authentic title music playback.
   - Quick navigation shortcuts: press `SPACE` / `ENTER` / `1` to start the game or `M` / `2` / `ESC` to open the Main Menu.

2. **Authentic C64 SID Music & Chip Sounds:**
   - Replaced generic audio samples with synthesized Commodore 64 MOS 6581 SID waveforms and chiptune sound effects (`Jump`, `Ding`, `Crack`, `Disk`, `Stomp`, `Shoot`, `Morphing`, `Died`, `Click`, `Portal`).
   - Integrated full chiptune module soundtracks (`Title`, `Outdoors / Stage 1`, `Indoors / Caves`, `Highscore`, `Ready`).
   - Low-latency `SDL2_mixer` audio pipeline tuned specifically for the ES8389 DAC / headphone amplifier and desktop audio devices.

3. **Scaled Graphics & Viewport for Cardputer ZERO (320×170):**
   - Custom rendering pipeline tailored for the 320×170 IPS display with two selectable viewport modes:
     - **C64 Scaled (Default / Authentic):** Full 640×340 stage viewport scaled to 320×170, preserving the exact visible height and layout of the original C64 release on CRT monitors.
     - **1:1 Zoomed Mode:** Unscaled pixel-for-pixel rendering with smooth dynamic camera tracking centered on Giana.
   - Toggle view modes instantly at any time during gameplay by pressing `V` or via the Options menu.

4. **Dedicated Menu Background & UI Redesign:**
   - Added custom retro-styled menu background artwork (`base/menu_background.png`).
   - Redesigned main menu, options menu, story screen, and in-game HUD coordinates to fit the 320×170 handheld geometry with clean alignment and zero clipping.

5. **Level Editor Removed:**
   - Removed the mouse-driven desktop level editor and its UI components to streamline the handheld experience, reduce binary size, conserve RAM/ROM, and focus the engine purely on authentic arcade platforming.

---

## Additional Features

- **Cardputer ZERO 46-Key Controls & Gamepad Support:**
  - Tuned keyboard matrix handling (`Arrows` / `WASD` for navigation, `SPACE` / `UP` / `W` / `K` for jump, `ENTER` / `J` / `RSHIFT` for fire/action).
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
