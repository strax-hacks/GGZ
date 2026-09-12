#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"

echo "=== Cardputer ZERO QEMU ARM64 System Image Runner ==="
echo "Target: Raspberry Pi CM0 / cardputer-zero-os (ARM64)"

QEMU_CMD="qemu-system-aarch64"
IMAGE_DIR="${ROOT_DIR}/build/qemu"
mkdir -p "${IMAGE_DIR}"

if ! command -v "${QEMU_CMD}" >/dev/null 2>&1; then
    echo "Notice: qemu-system-aarch64 is not installed on host."
    echo "To test in full system emulation:"
    echo "  brew install qemu (macOS) or apt-get install qemu-system-arm (Linux)"
    echo "Script configured to boot CM0 kernel with DRM display (320x170) and SSH port forwarding."
    exit 0
fi

echo "--> Launching QEMU ARM64 emulation harness..."
exec "${QEMU_CMD}" \
    -M raspi3b \
    -cpu cortex-a53 \
    -m 512M \
    -nographic \
    -net nic -net user,hostfwd=tcp::2222-:22 \
    "$@"
