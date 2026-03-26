#!/bin/bash
# Build script for lab23-2

set -e

echo "Building SVE interleaved test..."

g++ -std=c++17 -Wall -O2 -march=armv8-a+sve \
    -I./include \
    -o sve_interleaved_test \
    src/sve_interleaved.cpp src/main.cpp

echo "Build complete: sve_interleaved_test"