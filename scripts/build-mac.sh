#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.."; pwd)"
BUILD_DIR="$ROOT_DIR/build-mac"

mkdir -p "$BUILD_DIR/tmp/obj" "$BUILD_DIR/tmp/moc" "$BUILD_DIR/tmp/rcc" "$BUILD_DIR/tmp/ui" "$BUILD_DIR/out"
cd "$BUILD_DIR"


/opt/homebrew/opt/qt@5/bin/qmake \
  CONFIG+=sdk_no_version_check \
  "OBJECTS_DIR=$PWD/tmp/obj" \
  "MOC_DIR=$PWD/tmp/moc" \
  "RCC_DIR=$PWD/tmp/rcc" \
  "UI_DIR=$PWD/tmp/ui" \
  "DESTDIR=$PWD/out" \
  "$ROOT_DIR/src/JS_UDS_UI.pro" 

make -j"$(sysctl -n hw.ncpu)"

echo "Built app at: $BUILD_DIR/out/JS_UDS_UI.app"
echo "Run with: open \"$BUILD_DIR/out/JS_UDS_UI.app\""