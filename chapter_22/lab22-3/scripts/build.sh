#!/bin/bash
# Build script for lab22-3

set -e

echo "Building auto-vectorization test..."

g++ -std=c++17 -Wall -O3 -march=armv8-a+simd -ftree-vectorize \
    -I./include \
    -o auto_vectorization_test \
    src/auto_vec.cpp src/main.cpp

echo "Build complete: auto_vectorization_test"
echo ""
echo "To see vectorization reports, compile with:"
echo "  g++ -O3 -fopt-info-vec src/auto_vec.cpp"