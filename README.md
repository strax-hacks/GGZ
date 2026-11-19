# The Great Giana Sisters native on Cardputer ZERO, a fork/port of OpenGGS

[![Build & Test](https://github.com/strax-hacks/GGZ/actions/workflows/build-and-package.yml/badge.svg)](https://github.com/strax-hacks/GGZ/actions/workflows/build-and-package.yml)
[![GitHub Release](https://img.shields.io/github/v/release/strax-hacks/GGZ?include_prereleases&label=Latest%20Release)](https://github.com/strax-hacks/GGZ/releases)

A native port and enhanced fork of **OpenGGS** (the open-source C64 *The Great Giana Sisters* remake) engineered specifically for the **M5Stack Cardputer ZERO** (Raspberry Pi Compute Module 0 / BCM2837 ARM64, 320×170 IPS LCD, 46-key mechanical tactile keyboard) and modern desktop systems.

Full technical details and architecture specifications are documented in [docs/SPECIFICATION.md](docs/SPECIFICATION.md).

---

## Install on Cardputer Zero

Open Terminal on your Zero. To install over Wi-Fi, enable SSH on the Zero, find its address in **IP Panel**, and connect from your computer using the account you created during setup. [M5Stack SSH setup](https://docs.m5stack.com/en/compute/cardputerzero/usage)

```bash
ssh YOUR_USER@ZERO_IP
```

Run this **on the Zero**, either in its Terminal or in that SSH session. It downloads the ARM64 package from the latest published GitHub release and installs it with APT.

```bash
(
  set -eu
  release=$(curl -fsSL -o /dev/null -w '%{url_effective}' https://github.com/strax-hacks/GGZ/releases/latest)
  tag=${release##*/}
  case "$release" in
    https://github.com/strax-hacks/GGZ/releases/tag/v*) ;;
    *) echo "Could not find the latest GGZ release tag" >&2; exit 1 ;;
  esac
  deb="cardputerzero-openggs_${tag#v}-1_arm64.deb"
  download_dir=$(mktemp -d)
  chmod 755 "$download_dir"
  trap 'rm -f "$download_dir/$deb"; rmdir "$download_dir"' EXIT
  curl -fsSL "https://github.com/strax-hacks/GGZ/releases/download/$tag/$deb" -o "$download_dir/$deb"
  chmod 644 "$download_dir/$deb"
  sudo apt install "$download_dir/$deb"
)
```

Open **Giana Sisters Zero** from the Zero's home screen.

### Install an ARM64 package you already have

From your computer, copy the package to the Zero and connect. Substitute your actual filename, username, and IP address.

```bash
scp ./cardputerzero-openggs_1.0.2~store-1_arm64.deb YOUR_USER@ZERO_IP:/tmp/
ssh YOUR_USER@ZERO_IP
```

On the Zero, run:

```bash
sudo apt install /tmp/cardputerzero-openggs_1.0.2~store-1_arm64.deb
```

The updated package also provides `cardputerzero-openggs` as a command. Run it on the Zero as your regular user. It opens on the Zero's built-in display.

---

## Key Changes & Enhancements vs Upstream OpenGGS

Compared to the upstream OpenGGS implementation, this version includes several major gameplay, visual, and architectural changes:

1. **Scrolling Intro Screen with Original Music:**
   - Implemented the iconic, previously missing Commodore 64 scrolling intro sequence (Stage 0).
   - Features retro "GIANA SISTERS" block typography crafted from diamond and brick tiles, green raster banner text, smooth horizontal autoscrolling, and authentic title music playback.
   - Quick navigation shortcuts: press any key to start the game or `Esc` to open the main menu.

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
  - Use the printed direction cluster (`F`, `X`, `Z`, `C`) directly, with `D` / `G` as movement alternatives and `T`, `V`, or `Space` for fire.
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
| **Move Left / Right** | `D` / `G` or `Z` / `C` | `D-Pad Left` / `Right` |
| **Down** | `X` | `D-Pad Down` |
| **Jump** | `F` | `A` Button (South) |
| **Shoot / Fire** | `T`, `V`, or `Space` after the fireball power-up | `B` Button (East) |
| **Toggle View Mode** | `Fn+0` (`F10`) | — |
| **Pause Game** | `P` | — |
| **Menu / Back** | `Esc` | `Back` / `Select` Button |
| **Quick Save / Load** | `1` / `2` | — |
| **Music Down / Up** | `Fn+M` / `Fn+L` (`Page Down` / `Page Up`) | — |
| **Effects Down / Up** | `U` / `I` | — |

Press any key at the title to start or at the Get Ready screen to continue. Tap `Esc` during gameplay to go back, or hold it to return to the Zero home screen. `Fn+S` and `Fn+D` are the Zero's system volume shortcuts. [M5Stack keyboard guide](https://docs.m5stack.com/en/compute/cardputerzero/usage)

The Cardputer Zero build combines both audio channels for the built-in speaker. Headphones on the Zero get the same mono mix.

On desktop, use the arrow keys or `WASD`, `Up` / `W` / `K` to jump, `Ctrl` or `J` to fire, and `V` to toggle the view.

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

### Build a Cardputer Zero package

From the repository root, use Docker to build in an ARM64 Debian environment. Docker needs ARM64 support, either native or through emulation.

```bash
docker run --rm --platform linux/arm64 -v "$PWD:/work" -w /work debian:bookworm-slim sh -ec '
  apt-get update
  apt-get install -y --no-install-recommends build-essential cmake pkg-config \
    libdrm-dev libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev
  cmake -S . -B build/cp0-arm64-linux -DCMAKE_BUILD_TYPE=Release \
    -DTARGET_DEVICE=CARDPUTER_ZERO -DBUILD_TESTS=OFF
  cmake --build build/cp0-arm64-linux --parallel 2
  bash packaging/pack-deb.sh 1.0.2~store-1 arm64 build/cp0-arm64-linux
'
```

The package is written to `dist/`. Follow the [local package install steps](#install-an-arm64-package-you-already-have).

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
