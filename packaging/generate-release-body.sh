#!/usr/bin/env bash
set -euo pipefail

RELEASE_TAG="${1:-v1.0.0}"
RELEASE_VERSION="${2:-1.0.0}"
REPO="${3:-strax-hacks/GGZ}"
OUTPUT_FILE="${4:-release_body.md}"

cat << EOF > "${OUTPUT_FILE}"
# Giana Sisters Zero (\`cardputerzero-openggs\`) ${RELEASE_TAG}

Pre-built Debian installation package for **M5Stack Cardputer ZERO** (Raspberry Pi CM0 / ARM64).

---

## 🎮 Cardputer ZERO Download

| File | Type | Architecture | Description |
| :--- | :--- | :--- | :--- |
| **\`cardputerzero-openggs_${RELEASE_VERSION}-1_arm64.deb\`** | Debian Package | \`arm64\` (Raspberry Pi CM0) | Full standalone app package, game assets, and APPLaunch menu entry. |
| **\`SHA256SUMS.txt\`** | Checksums | — | SHA-256 integrity verification hash. |

---

## 🚀 How to Install on Cardputer ZERO

### Method 1: Direct Download on Device
\`\`\`bash
# 1. Download the .deb package
wget https://github.com/${REPO}/releases/download/${RELEASE_TAG}/cardputerzero-openggs_${RELEASE_VERSION}-1_arm64.deb

# 2. Install using dpkg
sudo dpkg -i cardputerzero-openggs_${RELEASE_VERSION}-1_arm64.deb || sudo apt-get install -f -y

# 3. Launch from APPLaunch menu or terminal
openggs-cz
\`\`\`

### Method 2: Copy from PC / Mac via SCP
\`\`\`bash
# Copy from your computer to Cardputer ZERO
scp cardputerzero-openggs_${RELEASE_VERSION}-1_arm64.deb root@cardputer.local:/tmp/

# SSH in and install
ssh root@cardputer.local "dpkg -i /tmp/cardputerzero-openggs_${RELEASE_VERSION}-1_arm64.deb"
\`\`\`

---

## 🕹️ Controls

| Action | Cardputer ZERO 46-Key Keyboard | External Gamepad |
| :--- | :--- | :--- |
| **Move Left / Right** | \`Left\` / \`Right\` or \`A\` / \`D\` | \`D-Pad Left\` / \`Right\` |
| **Look Down / Crouch** | \`Down\` or \`S\` | \`D-Pad Down\` |
| **Jump** | \`Space\`, \`Up\`, \`W\`, or \`K\` | \`A\` Button (South) |
| **Shoot / Fire** | \`Enter\`, \`J\`, or \`Right Shift\` | \`B\` Button (East) / \`X\` |
| **Toggle View Mode** | \`V\` (C64 Scaled / 1:1 Zoom) | \`Y\` Button (North) |
| **Pause Game** | \`P\` | \`Start\` Button |
| **Menu / Back** | \`Esc\` | \`Back\` / \`Select\` |
| **Quick Save / Load** | \`F5\` / \`F6\` | — |
EOF

echo "✓ Generated ${OUTPUT_FILE} for ${RELEASE_TAG} (version ${RELEASE_VERSION})"
