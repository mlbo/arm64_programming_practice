#!/bin/bash
# Build script for lab23-1

set -e

echo "Building SVE basic test..."

g++ -std=c++17 -Wall -O2 -march=armv8-a+sve \
    -I./include \
    -o sve_basic_test \
    src/sve_basic.cpp src/main.cpp

echo "Build complete: sve_basic_test"