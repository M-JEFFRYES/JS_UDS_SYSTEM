#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.."; pwd)"
BUILD_DIR="$ROOT_DIR/build-mac"

cd "$BUILD_DIR"

open "$BUILD_DIR/out/JS_UDS_UI.app"