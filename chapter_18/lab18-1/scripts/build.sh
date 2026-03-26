#!/bin/bash
# chapter_18/lab18-1/scripts/build.sh
# 编译脚本

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LAB_DIR="$(dirname "$SCRIPT_DIR")"

cd "$LAB_DIR"

echo "Building litmus_test..."

g++ -std=c++17 -O2 -pthread \
    -I./include \
    -o litmus_test \
    src/main.cpp src/litmus_tests.cpp

echo "Build successful: litmus_test"