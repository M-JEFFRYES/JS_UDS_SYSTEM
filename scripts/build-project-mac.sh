#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.."; pwd)"
BUILD_DIR="$ROOT_DIR/build"

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"


/opt/homebrew/opt/qt@5/bin/qmake \
  CONFIG+=sdk_no_version_check \
  "$ROOT_DIR/project.pro" 

make -j"$(sysctl -n hw.ncpu)"

echo
echo "Built app at: $BUILD_DIR/src/out/JS_UDS_UI.app"
echo "Run with: open \"$BUILD_DIR/src/out/JS_UDS_UI.app\""
echo 
echo "Built app at: $BUILD_DIR/src/out/JS_UDS_UI.app"
echo "Run tests with: open \"$BUILD_DIR/tests/out/uds_core_tests.app\""
echo
