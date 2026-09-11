#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${ROOT_DIR}/build/cp0-arm64"
STAGE_DIR="${ROOT_DIR}/build/deb_stage"
OUTPUT_DIR="${ROOT_DIR}/dist"

VERSION="1.0.0-1"
PKG_NAME="cardputerzero-openggs_${VERSION}_arm64"

echo "=== Packaging Cardputer ZERO DEB Package: ${PKG_NAME}.deb ==="

rm -rf "${STAGE_DIR}"
mkdir -p "${STAGE_DIR}/DEBIAN"
mkdir -p "${STAGE_DIR}/usr/share/APPLaunch/applications"
mkdir -p "${STAGE_DIR}/usr/share/APPLaunch/share/images"
mkdir -p "${STAGE_DIR}/usr/share/openggs/bin"
mkdir -p "${STAGE_DIR}/usr/share/openggs/data"
mkdir -p "${OUTPUT_DIR}"

# Copy Debian control file
cp "${ROOT_DIR}/packaging/DEBIAN/control" "${STAGE_DIR}/DEBIAN/control"

# Copy APPLaunch .desktop and icon
cp "${ROOT_DIR}/packaging/openggs.desktop" "${STAGE_DIR}/usr/share/APPLaunch/applications/openggs.desktop"
if [[ -f "${ROOT_DIR}/packaging/openggs.png" ]]; then
  cp "${ROOT_DIR}/packaging/openggs.png" "${STAGE_DIR}/usr/share/APPLaunch/share/images/openggs.png"
fi

# Copy binary
if [[ -f "${BUILD_DIR}/openggs-cz" ]]; then
  cp "${BUILD_DIR}/openggs-cz" "${STAGE_DIR}/usr/share/openggs/bin/openggs-cz"
  chmod 755 "${STAGE_DIR}/usr/share/openggs/bin/openggs-cz"
else
  echo "Warning: ${BUILD_DIR}/openggs-cz not found. Staging template executable placeholder."
fi

# Copy game data assets
if [[ -d "${ROOT_DIR}/base" ]]; then
  cp -r "${ROOT_DIR}/base"/* "${STAGE_DIR}/usr/share/openggs/data/"
fi

# Build Debian package
dpkg-deb --build --root-owner-group "${STAGE_DIR}" "${OUTPUT_DIR}/${PKG_NAME}.deb"

echo "✓ Successfully built: ${OUTPUT_DIR}/${PKG_NAME}.deb"
