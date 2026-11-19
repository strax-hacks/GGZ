#!/usr/bin/env bash
set -euo pipefail

RELEASE_TAG="${1:-v1.0.0}"
RELEASE_VERSION="${2:-1.0.0}"
REPO="${3:-strax-hacks/GGZ}"
OUTPUT_FILE="${4:-release_body.md}"

RELEASE_HIGHLIGHTS=""
if [[ "$RELEASE_TAG" == v1.0.3 ]]; then
  RELEASE_HIGHLIGHTS=$(cat <<'NOTES'
## What’s new in v1.0.3

- Press any key to continue from Get Ready and warp screens. Use D/G to move and T to fire on the Zero.
- Mixes both soundtrack channels for the built-in speaker and lowers the effects mix.
- Keeps music playing through power-ups and retries the LCD handoff when the display is busy.
- Fixes an out-of-bounds substage copy without changing the level file format.
NOTES
)
elif [[ "$RELEASE_TAG" == v1.0.2 ]]; then
  RELEASE_HIGHLIGHTS=$(cat <<'NOTES'
## What’s new in v1.0.2

- Launches on the Cardputer Zero’s built-in LCD from its home screen.
- Adds the stone G icon, handheld controls, and any-key start from the title.
- Lowers the death sound and adds live effects-volume controls.
NOTES
)
fi

cat << EOF > "${OUTPUT_FILE}"
# Giana Sisters Zero (\`cardputerzero-openggs\`) ${RELEASE_TAG}

Pre-built Debian installation package for **M5Stack Cardputer ZERO** (Raspberry Pi CM0 / ARM64).

---

${RELEASE_HIGHLIGHTS:+$RELEASE_HIGHLIGHTS}

## 🎮 Cardputer ZERO Download

| File | Type | Architecture | Description |
| :--- | :--- | :--- | :--- |
| **\`cardputerzero-openggs_${RELEASE_VERSION}-1_arm64.deb\`** | Debian Package | \`arm64\` (Raspberry Pi CM0) | Full standalone app package, game assets, and APPLaunch menu entry. |
| **\`SHA256SUMS.txt\`** | Checksums | — | SHA-256 integrity verification hash. |

---

## 🚀 How to Install on Cardputer ZERO

Run these commands in the Zero's Terminal or in an SSH session as your regular user.

\`\`\`bash
cd "\$(mktemp -d)" && chmod 755 . &&
wget -O cardputerzero-openggs_${RELEASE_VERSION}-1_arm64.deb https://github.com/${REPO}/releases/download/${RELEASE_TAG}/cardputerzero-openggs_${RELEASE_VERSION}-1_arm64.deb &&
  sudo apt install ./cardputerzero-openggs_${RELEASE_VERSION}-1_arm64.deb
\`\`\`

Open **Giana Sisters Zero** from the Zero's home screen. For SSH setup or installation from a local package, see the [README](https://github.com/${REPO}#install-on-cardputer-zero).

---

## 🕹️ Controls

| Action | Cardputer ZERO 46-Key Keyboard | External Gamepad |
| :--- | :--- | :--- |
| **Move Left / Right** | \`D\` / \`G\` or \`Z\` / \`C\` | \`D-Pad Left\` / \`Right\` |
| **Down** | \`X\` | \`D-Pad Down\` |
| **Jump** | \`F\` | \`A\` Button (South) |
| **Shoot / Fire** | \`T\`, \`V\`, or \`Space\` after the fireball power-up | \`B\` Button (East) |
| **Toggle View Mode** | \`Fn+0\` (\`F10\`) | — |
| **Pause Game** | \`P\` | — |
| **Menu / Back** | \`Esc\` | \`Back\` / \`Select\` |
| **Quick Save / Load** | \`1\` / \`2\` | — |
| **Music Down / Up** | \`Fn+M\` / \`Fn+L\` | — |
| **Effects Down / Up** | \`U\` / \`I\` | — |

Press any key at the title to start or at the Get Ready screen to continue. Hold \`Esc\` to return to the Zero home screen. Use \`Fn+S\` and \`Fn+D\` for system volume.
EOF

echo "✓ Generated ${OUTPUT_FILE} for ${RELEASE_TAG} (version ${RELEASE_VERSION})"
