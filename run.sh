#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="build"
APP="$BUILD_DIR/bin/app"

if [[ ! -x "$APP" ]]; then
    echo "Binary not found: $APP" >&2
    echo "Run ./build.sh first." >&2
    exit 1
fi

"$APP"
