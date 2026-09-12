#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"

echo "=== Cardputer ZERO OpenGGS Headless Smoke Test Suite ==="

# Build desktop binary if not already built
mkdir -p "${ROOT_DIR}/build/desktop"
cd "${ROOT_DIR}/build/desktop"

if [[ ! -f "${ROOT_DIR}/build/desktop/tests/smoke_test" ]]; then
    echo "--> Compiling test suite..."
    cmake "${ROOT_DIR}" -DCMAKE_BUILD_TYPE=Debug -DTARGET_DEVICE=DESKTOP
    cmake --build . --target smoke_test --parallel 4
fi

echo "--> Running smoke_test..."
export SDL_VIDEODRIVER=dummy
export SDL_AUDIODRIVER=dummy
cd "${ROOT_DIR}"
"${ROOT_DIR}/build/desktop/tests/smoke_test"

echo "✓ Headless smoke test passed successfully."
