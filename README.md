# Cardputer ZERO OpenGGS (`cardputerzero-openggs`)

A native port and working fork of **OpenGGS** (the open-source C64 *The Great Giana Sisters* remake) tailored specifically for the **M5Stack Cardputer ZERO** (Raspberry Pi Compute Module 0 / BCM2837 ARM64, 320×170 IPS LCD, 46-key mechanical keyboard matrix).

---

## Architecture Overview

```text
Raspberry Pi OS / Debian Bookworm ARM64
  └── cardputer-zero-os (DRM/KMS / labwc session plumbing)
        └── cardputer-zero-shell / APPLaunch
              └── cardputerzero-openggs (/usr/share/APPLaunch/applications/openggs.desktop)
                    └── SDL2 Logical Viewport (320x170 @ 60 FPS)
```

- **Resolution:** 320×170 native framebuffer / logical scaled viewport (`nearest` texture filtering for authentic retro pixel art)
- **Controls:** 46-key matrix mapped to directional movement (`Arrows` / `WASD`), Jump (`SPACE` / `UP` / `K`), Fire (`ENTER` / `J`), Pause (`P`), and Menu (`ESC`)
- **Audio:** `SDL2_mixer` tuned for Broadcom BCM2837 / ES8389 DAC (22,050 Hz, 16-bit, buffer size 1024)
- **Packaging:** Debian `.deb` package with `APPLaunch` desktop integration (`/usr/share/APPLaunch/applications/openggs.desktop`)

---

## Cardputer ZERO App Template Runner

The development harness provides both a local desktop simulator (macOS / Linux SDL2) and ARM64 cross-compilation:

### 1. Run Desktop Simulator (Local Development)
```bash
# Builds and launches the app in a 320x170 scaled preview window
./scripts/run-desktop.sh
```

### 2. CMake Preset Workflow
```bash
# Configure & build desktop development preview
cmake --preset desktop-dev
cmake --build --preset desktop-dev

# Cross-compile for Cardputer ZERO ARM64 target
cmake --preset cp0-arm64-cross
cmake --build --preset cp0-arm64-cross
```

### 3. Package for Cardputer ZERO (`.deb`)
```bash
./packaging/pack-deb.sh
# Generated artifact: dist/cardputerzero-openggs_1.0.0-1_arm64.deb
```

---

## Beads Task Breakdown & Issue Tracker

Task management and execution are tracked using **Beads** (`bd`):

| Bead ID | Task Title | Type | Estimate | Status |
| :--- | :--- | :--- | :--- | :--- |
| **`cz-ggz-bf4`** | **Port and package OpenGGS (C64 clone) for M5Stack Cardputer ZERO** | `epic` | — | Open |
| `cz-ggz-bf4.1` | Setup Cardputer Zero App Template Runner and build harness | `task` | 30m | Open (Scaffolded) |
| `cz-ggz-bf4.2` | Ingest upstream bugix/OpenGGS C64 game codebase and assets | `task` | 30m | Open |
| `cz-ggz-bf4.3` | Implement native 320x170 display resolution and viewport scaling in engine | `task` | 45m | Open |
| `cz-ggz-bf4.4` | Redesign HUD layout and menus for 320x170 screen geometry | `task` | 30m | Open |
| `cz-ggz-bf4.5` | Adapt input system for Cardputer ZERO 46-key matrix keyboard | `task` | 30m | Open |
| `cz-ggz-bf4.6` | Optimize audio pipeline for BCM2837 / ES8389 on Cardputer ZERO | `task` | 25m | Open |
| `cz-ggz-bf4.7` | Create APPLaunch .desktop integration, app icon, and Debian packaging (.deb) | `task` | 35m | Open |
| `cz-ggz-bf4.8` | Build validation suite: headless smoke tests and CI build matrix | `task` | 30m | Open |
| `cz-ggz-bf4.9` | Setup CardputerZero official czdev emulator runtime integration | `task` | 25m | Open |
| `cz-ggz-bf4.10` | Setup QEMU ARM64 cardputer-zero-os system image runner | `task` | 40m | Open |

To view ready work in Beads:
```bash
bd ready
bd show <issue-id>
```
