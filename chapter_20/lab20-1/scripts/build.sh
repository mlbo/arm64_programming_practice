#!/bin/bash
# chapter_20/lab20-1/scripts/build.sh
# 编译脚本

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LAB_DIR="$(dirname "$SCRIPT_DIR")"

cd "$LAB_DIR"

echo "Building atomic_demo..."

g++ -std=c++17 -O2 -pthread \
    -I./include \
    -o atomic_demo \
    src/main.cpp src/atomic_test.cpp

echo "Build successful: atomic_demo"