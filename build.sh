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
    # gcc -print-file-name returns the bare name when the file is not found
    LIBASAN=$(gcc -print-file-name=libasan.a 2>/dev/null || true)
    if [[ "$LIBASAN" == "libasan.a" ]]; then
        echo "ERROR: libasan.a not found – install it first:" >&2
        echo "  sudo dnf install libasan   # RHEL 8 / 9" >&2
        exit 1
    fi
    CMAKE_ARGS+=(-DENABLE_ASAN=ON)
fi

cmake "${CMAKE_ARGS[@]}"
build-wrapper-linux-x86-64 --out-dir bw-output cmake --build "$BUILD_DIR" -j"$(nproc)"
