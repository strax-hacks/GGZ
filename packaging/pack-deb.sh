#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"

VERSION="${1:-${PACKAGE_VERSION:-1.0.0-1}}"
# Strip leading 'v' if present (e.g. v1.0.0 -> 1.0.0-1)
VERSION="${VERSION#v}"
if [[ "$VERSION" != *-* ]]; then
  VERSION="${VERSION}-1"
fi

ARCH="${2:-${TARGET_ARCH:-arm64}}"
BUILD_DIR="${3:-${BUILD_DIR:-${ROOT_DIR}/build/cp0-${ARCH}}}"
if [[ ! -f "${BUILD_DIR}/openggs-cz" && -f "${ROOT_DIR}/build/desktop/openggs-cz" && "$ARCH" =~ ^(amd64|x86_64)$ ]]; then
  BUILD_DIR="${ROOT_DIR}/build/desktop"
fi

STAGE_DIR="${ROOT_DIR}/build/deb_stage_${ARCH}"
OUTPUT_DIR="${ROOT_DIR}/dist"
PKG_NAME="cardputerzero-openggs_${VERSION}_${ARCH}"

echo "=== Packaging DEB Package: ${PKG_NAME}.deb ==="

rm -rf "${STAGE_DIR}"
mkdir -p "${STAGE_DIR}/DEBIAN"
mkdir -p "${STAGE_DIR}/usr/bin"
mkdir -p "${STAGE_DIR}/usr/share/applications"
mkdir -p "${STAGE_DIR}/usr/share/pixmaps"
mkdir -p "${STAGE_DIR}/usr/share/APPLaunch/applications"
mkdir -p "${STAGE_DIR}/usr/share/APPLaunch/share/images"
mkdir -p "${STAGE_DIR}/usr/share/openggs/bin"
mkdir -p "${STAGE_DIR}/usr/share/openggs/data"
mkdir -p "${OUTPUT_DIR}"

# Generate Debian control file with proper version and architecture
sed \
  -e "s/^Version:.*/Version: ${VERSION}/" \
  -e "s/^Architecture:.*/Architecture: ${ARCH}/" \
  "${ROOT_DIR}/packaging/DEBIAN/control" > "${STAGE_DIR}/DEBIAN/control"

# Copy binary
if [[ -f "${BUILD_DIR}/openggs-cz" ]]; then
  cp -f "${BUILD_DIR}/openggs-cz" "${STAGE_DIR}/usr/share/openggs/bin/openggs-cz"
  chmod 755 "${STAGE_DIR}/usr/share/openggs/bin/openggs-cz"
  # Create symlink in /usr/bin for command line invocation
  ln -sf /usr/share/openggs/bin/openggs-cz "${STAGE_DIR}/usr/bin/openggs-cz"
else
  echo "Error: Binary not found at ${BUILD_DIR}/openggs-cz" >&2
  exit 1
fi

# Copy game data assets
if [[ -d "${ROOT_DIR}/base" ]]; then
  cp -rf "${ROOT_DIR}/base"/* "${STAGE_DIR}/usr/share/openggs/data/"
fi

# Copy desktop entries and icons (both APPLaunch and standard FreeDesktop)
cp -f "${ROOT_DIR}/packaging/openggs.desktop" "${STAGE_DIR}/usr/share/APPLaunch/applications/openggs.desktop"
cp -f "${ROOT_DIR}/packaging/openggs.desktop" "${STAGE_DIR}/usr/share/applications/openggs.desktop"
if [[ -f "${ROOT_DIR}/packaging/openggs.png" ]]; then
  cp -f "${ROOT_DIR}/packaging/openggs.png" "${STAGE_DIR}/usr/share/APPLaunch/share/images/openggs.png"
  cp -f "${ROOT_DIR}/packaging/openggs.png" "${STAGE_DIR}/usr/share/pixmaps/openggs.png"
fi

# Build Debian package if dpkg-deb is available
if command -v dpkg-deb >/dev/null 2>&1; then
  dpkg-deb --build --root-owner-group "${STAGE_DIR}" "${OUTPUT_DIR}/${PKG_NAME}.deb"
  echo "✓ Successfully built: ${OUTPUT_DIR}/${PKG_NAME}.deb"
else
  echo "dpkg-deb not found on host. Staged directory ready at: ${STAGE_DIR}"
fi
