#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="build"
ASAN=${ASAN:-0}     # set ASAN=1 to enable AddressSanitizer

CMAKE_ARGS=(
    -S .
    -B "$BUILD_DIR"
    -DCMAKE_BUILD_TYPE=Debug
)

if [[ "$ASAN" == "1" ]]; then
    CMAKE_ARGS+=(-DENABLE_ASAN=ON)
fi

cmake "${CMAKE_ARGS[@]}"
build-wrapper-linux-x86-64 --out-dir bw-outputcmake --build "$BUILD_DIR" -j"$(nproc)"
