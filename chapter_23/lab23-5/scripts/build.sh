#!/bin/bash
# Build script for lab23-5

set -e

echo "Building SVE matrix test..."

g++ -std=c++17 -Wall -O2 -march=armv8-a+sve \
    -I./include \
    -o sve_matrix_test \
    src/sve_matrix.cpp src/main.cpp

echo "Build complete: sve_matrix_test"