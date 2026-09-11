#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${ROOT_DIR}/build/desktop"

echo "=== Cardputer ZERO App Template Runner (OpenGGS Desktop Simulator) ==="
echo "Target Resolution: 320x170 (ST7789v3 native format)"

mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

if command -v cmake >/dev/null 2>&1; then
  echo "--> Configuring CMake build..."
  cmake "${ROOT_DIR}" -DCMAKE_BUILD_TYPE=Debug -DTARGET_DEVICE=DESKTOP
  echo "--> Building openggs-cz simulator binary..."
  cmake --build . --parallel "$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 2)"
else
  echo "Error: cmake is required to build the simulator." >&2
  exit 1
fi

if [[ -f "${BUILD_DIR}/openggs-cz" ]]; then
  echo "--> Launching OpenGGS in 320x170 Cardputer ZERO Simulator..."
  cd "${ROOT_DIR}"
  "${BUILD_DIR}/openggs-cz" "$@"
else
  echo "Error: openggs-cz binary was not created." >&2
  exit 1
fi
