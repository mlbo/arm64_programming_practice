#!/bin/bash
# Build script for lab22-1

set -e

echo "Building NEON RGB/BGR conversion test..."

g++ -std=c++17 -Wall -O2 -march=armv8-a+simd \
    -I./include \
    -o neon_rgb_bgr_test \
    src/neon_rgb_bgr.cpp src/main.cpp

echo "Build complete: neon_rgb_bgr_test"