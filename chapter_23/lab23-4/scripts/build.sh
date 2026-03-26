#!/bin/bash
# Build script for lab23-4

set -e

echo "Building SVE strcmp test..."

g++ -std=c++17 -Wall -O2 -march=armv8-a+sve \
    -I./include \
    -o sve_strcmp_test \
    src/strcmp_sve.S src/main.cpp

echo "Build complete: sve_strcmp_test"