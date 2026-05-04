#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="build"
ASAN=${ASAN:-0}

# 1. Start from a clean slate
echo "Cleaning build directory..."
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

CMAKE_ARGS=(
    -S .
    -B "$BUILD_DIR"
    -DCMAKE_BUILD_TYPE=Debug
)

if [[ "$ASAN" == "1" ]]; then
    LIBASAN=$(gcc -print-file-name=libasan.a 2>/dev/null || true)
    if [[ "$LIBASAN" == "libasan.a" ]]; then
        echo "ERROR: libasan.a not found – install it first:" >&2
        echo "  sudo dnf install libasan" >&2
        exit 1
    fi
    CMAKE_ARGS+=(-DENABLE_ASAN=ON)
fi

# 2. Configure the project
echo "Configuring with CMake..."
cmake "${CMAKE_ARGS[@]}"

# 3. Build with SonarQube build-wrapper
echo "Starting build-wrapper..."
build-wrapper-linux-x86-64 --out-dir bw-output cmake --build "$BUILD_DIR" -j"$(nproc)"
