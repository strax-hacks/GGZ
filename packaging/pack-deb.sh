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
mkdir -p "${STAGE_DIR}/usr/share/APPLaunch/applications"
mkdir -p "${STAGE_DIR}/usr/share/APPLaunch/share/images"
mkdir -p "${STAGE_DIR}/usr/share/cardputerzero-openggs/bin"
mkdir -p "${STAGE_DIR}/usr/share/cardputerzero-openggs/data"
mkdir -p "${OUTPUT_DIR}"

# Generate Debian control file with proper version and architecture
sed \
  -e "s/^Version:.*/Version: ${VERSION}/" \
  -e "s/^Architecture:.*/Architecture: ${ARCH}/" \
  "${ROOT_DIR}/packaging/DEBIAN/control" > "${STAGE_DIR}/DEBIAN/control"

# Copy binary
install -m 0755 "${ROOT_DIR}/packaging/openggs-cz-launch" "${STAGE_DIR}/usr/bin/cardputerzero-openggs"
if [[ -f "${BUILD_DIR}/openggs-cz" ]]; then
  cp -f "${BUILD_DIR}/openggs-cz" "${STAGE_DIR}/usr/share/cardputerzero-openggs/bin/openggs-cz"
  chmod 755 "${STAGE_DIR}/usr/share/cardputerzero-openggs/bin/openggs-cz"
else
  echo "Error: Binary not found at ${BUILD_DIR}/openggs-cz" >&2
  exit 1
fi

# Copy game data assets
if [[ -d "${ROOT_DIR}/base" ]]; then
  cp -rf "${ROOT_DIR}/base"/* "${STAGE_DIR}/usr/share/cardputerzero-openggs/data/"
fi

# Install the launcher and icon under the package name.
install -m 0644 "${ROOT_DIR}/packaging/openggs.desktop" "${STAGE_DIR}/usr/share/APPLaunch/applications/cardputerzero-openggs.desktop"
install -m 0644 "${ROOT_DIR}/packaging/openggs.png" "${STAGE_DIR}/usr/share/APPLaunch/share/images/cardputerzero-openggs.png"

# The amd64 desktop package also integrates with standard Linux menus.
if [[ "$ARCH" == amd64 || "$ARCH" == x86_64 ]]; then
  ln -s cardputerzero-openggs "${STAGE_DIR}/usr/bin/openggs-cz"
  install -d "${STAGE_DIR}/usr/share/applications" "${STAGE_DIR}/usr/share/pixmaps"
  install -m 0644 "${ROOT_DIR}/packaging/openggs.desktop" "${STAGE_DIR}/usr/share/applications/cardputerzero-openggs.desktop"
  install -m 0644 "${ROOT_DIR}/packaging/openggs.png" "${STAGE_DIR}/usr/share/pixmaps/cardputerzero-openggs.png"
fi

# Build Debian package if dpkg-deb is available
if command -v dpkg-deb >/dev/null 2>&1; then
  dpkg-deb --build --root-owner-group "${STAGE_DIR}" "${OUTPUT_DIR}/${PKG_NAME}.deb"
  echo "✓ Successfully built: ${OUTPUT_DIR}/${PKG_NAME}.deb"
else
  echo "dpkg-deb not found on host. Staged directory ready at: ${STAGE_DIR}"
fi
