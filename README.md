# Cardputer ZERO OpenGGS (`cardputerzero-openggs`)

A native port and working fork of **OpenGGS** (the open-source C64 *The Great Giana Sisters* remake) tailored specifically for the **M5Stack Cardputer ZERO** (Raspberry Pi Compute Module 0 / BCM2837 ARM64, 320×170 IPS LCD, 46-key mechanical keyboard matrix).

Full technical details and architecture specifications are documented in [docs/SPECIFICATION.md](docs/SPECIFICATION.md).

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
- **Controls:** 46-key matrix mapped to directional movement (`Arrows` / `WASD`), Jump (`SPACE` / `UP` / `K`), Fire (`ENTER` / `J`), Pause (`P`), and Menu (`ESC`), plus USB/Bluetooth GameController hot-plugging
- **Audio (Core P1):** `SDL2_mixer` tuned for Broadcom BCM2837 / ES8389 DAC (22,050 Hz / 44,100 Hz, 16-bit, buffer size 1024) — full original C64/OpenGGS chiptune music tracks & 16 sound effects with zero stutter, clean looping, and dedicated volume sliders
- **Power & Lifecycle:** Battery monitoring, auto-pause on window focus loss, low-power 60 FPS pacing
- **Storage:** XDG Base Directory isolation (`~/.config/openggs`, `~/.local/share/openggs`) with 5-slot save states
- **Packaging:** Debian `.deb` package with `APPLaunch` desktop integration (`/usr/share/APPLaunch/applications/openggs.desktop`) and CardputerZero Store metadata
- **Level Editor:** Deferred (excluded from the handheld device target to preserve ROM size and clean UI focus)

---

## Cardputer ZERO App Template Runner & Development

The development harness provides multi-tier simulation and ARM64 cross-compilation:

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

## Complete Beads Task Matrix

Task management, lifecycle states, and dependency chains are managed using **Beads** (`bd`):

| Bead Task ID | Milestone | Title | Priority | Est. | Status |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **`cz-ggz-bf4`** | **Epic** | **Port and package OpenGGS for M5Stack Cardputer ZERO** | `P1` | — | Open |
| `cz-ggz-bf4.1` | Harness | Setup Cardputer Zero App Template Runner and build harness | `P1` | 30m | **Ready** |
| `cz-ggz-bf4.2` | Engine | Ingest upstream bugix/OpenGGS C64 game codebase and assets | `P1` | 30m | Open |
| `cz-ggz-bf4.3` | Graphics | Implement native 320x170 display resolution and viewport scaling | `P1` | 45m | Open |
| `cz-ggz-bf4.4` | UI/HUD | Redesign HUD layout and menus for 320x170 screen geometry | `P2` | 30m | Open |
| `cz-ggz-bf4.5` | Controls | Adapt input system for Cardputer ZERO 46-key matrix keyboard | `P1` | 30m | Open |
| `cz-ggz-bf4.6` | Audio | Optimize audio pipeline for BCM2837 / ES8389 on Cardputer ZERO | `P1` | 25m | Open |
| `cz-ggz-bf4.7` | Packaging | Create APPLaunch .desktop integration, icon, and Debian packaging (.deb) | `P1` | 35m | Open |
| `cz-ggz-bf4.8` | CI / QA | Build validation suite: headless smoke tests and CI build matrix | `P2` | 30m | Open |
| `cz-ggz-bf4.9` | Tooling | Setup CardputerZero official czdev emulator runtime integration | `P2` | 25m | Open |
| `cz-ggz-bf4.10` | Tooling | Setup QEMU ARM64 cardputer-zero-os system image runner | `P3` | 40m | Open |
| `cz-ggz-bf4.11` | Storage | Implement XDG-compliant persistent storage and portable save-states | `P1` | 35m | Open |
| `cz-ggz-bf4.12` | Power/OS | Implement battery monitoring, power management, and suspend/resume | `P2` | 30m | Open |
| `cz-ggz-bf4.13` | Input | Implement SDL2 GameController hot-plugging & external gamepads | `P2` | 25m | Open |
| `cz-ggz-bf4.14` | UI/Settings | Implement on-device settings menu, visual filters, and help overlay | `P2` | 30m | Open |
| `cz-ggz-bf4.15` | Level Editor| Adapt OpenGGS Maker / Level Editor for 320x170 keyboard navigation | `P3` | 35m | **Deferred** |
| `cz-ggz-bf4.16` | Store/Pub | Generate CardputerZero Store registry entry and distribution assets | `P2` | 20m | Open |

To query ready tasks in Beads:
```bash
bd ready
```
