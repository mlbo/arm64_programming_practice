#!/bin/bash
# Build script for lab23-6

set -e

echo "Building SVE FFR test..."

g++ -std=c++17 -Wall -O2 -march=armv8-a+sve \
    -I./include \
    -o sve_ffr_test \
    src/sve_ffr.cpp src/main.cpp

echo "Build complete: sve_ffr_test"