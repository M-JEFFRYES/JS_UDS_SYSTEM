#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.."; pwd)"
BUILD_DIR="$ROOT_DIR/build"
RUN_PATH="$BUILD_DIR/tests/out/uds_core_tests.app/Contents/MacOS/uds_core_tests"

"$RUN_PATH"