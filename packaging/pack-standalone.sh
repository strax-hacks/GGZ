#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"

VERSION="${1:-${PACKAGE_VERSION:-1.0.0}}"
VERSION="${VERSION#v}"
VERSION="${VERSION%-*}" # e.g. 1.0.0-1 -> 1.0.0

PLATFORM="${2:-${TARGET_PLATFORM:-cardputerzero-arm64}}"
BUILD_DIR="${3:-${BUILD_DIR:-${ROOT_DIR}/build/desktop}}"

OUTPUT_DIR="${ROOT_DIR}/dist"
PKG_DIR="${ROOT_DIR}/build/standalone_${PLATFORM}"
ARCHIVE_NAME="cardputerzero-openggs-${VERSION}-${PLATFORM}"

echo "=== Creating Standalone Archive: ${ARCHIVE_NAME}.tar.gz ==="

rm -rf "${PKG_DIR}"
mkdir -p "${PKG_DIR}/${ARCHIVE_NAME}"
mkdir -p "${OUTPUT_DIR}"

# Check binary
if [[ -f "${BUILD_DIR}/openggs-cz" ]]; then
  cp -f "${BUILD_DIR}/openggs-cz" "${PKG_DIR}/${ARCHIVE_NAME}/openggs-cz"
  chmod 755 "${PKG_DIR}/${ARCHIVE_NAME}/openggs-cz"
else
  echo "Error: Binary not found at ${BUILD_DIR}/openggs-cz" >&2
  exit 1
fi

# Copy assets
if [[ -d "${ROOT_DIR}/base" ]]; then
  cp -rf "${ROOT_DIR}/base" "${PKG_DIR}/${ARCHIVE_NAME}/base"
fi

# Copy packaging metadata / desktop / icons
mkdir -p "${PKG_DIR}/${ARCHIVE_NAME}/packaging"
cp -f "${ROOT_DIR}/packaging/openggs.desktop" "${PKG_DIR}/${ARCHIVE_NAME}/packaging/" 2>/dev/null || true
cp -f "${ROOT_DIR}/packaging/openggs.png" "${PKG_DIR}/${ARCHIVE_NAME}/packaging/" 2>/dev/null || true
if [[ -f "${ROOT_DIR}/README.md" ]]; then
  cp -f "${ROOT_DIR}/README.md" "${PKG_DIR}/${ARCHIVE_NAME}/"
fi

# Create portable runner script
cat << 'EOF' > "${PKG_DIR}/${ARCHIVE_NAME}/run.sh"
#!/usr/bin/env bash
set -e
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$DIR"
exec ./openggs-cz "$@"
EOF
chmod 755 "${PKG_DIR}/${ARCHIVE_NAME}/run.sh"

# Create .tar.gz archive
tar -czf "${OUTPUT_DIR}/${ARCHIVE_NAME}.tar.gz" -C "${PKG_DIR}" "${ARCHIVE_NAME}"

echo "✓ Successfully created: ${OUTPUT_DIR}/${ARCHIVE_NAME}.tar.gz"
