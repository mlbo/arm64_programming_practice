#!/bin/bash
# Build script for lab22-2

set -e

echo "Building NEON matrix multiplication test..."

g++ -std=c++17 -Wall -O2 -march=armv8-a+simd \
    -I./include \
    -o neon_matrix_test \
    src/neon_matrix.cpp src/main.cpp

echo "Build complete: neon_matrix_test"