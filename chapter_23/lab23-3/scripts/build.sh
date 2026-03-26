#!/bin/bash
# Build script for lab23-3

set -e

echo "Building SVE memcpy test..."

g++ -std=c++17 -Wall -O2 -march=armv8-a+sve \
    -I./include \
    -o sve_memcpy_test \
    src/sve_memcpy.cpp src/sve_memcpy.S src/main.cpp

echo "Build complete: sve_memcpy_test"