#!/bin/bash
# chapter_16/lab16-1/scripts/build.sh
# 编译脚本

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LAB_DIR="$(dirname "$SCRIPT_DIR")"

cd "$LAB_DIR"

echo "Building false_sharing_demo..."

g++ -std=c++17 -O2 -pthread \
    -I./include \
    -o false_sharing_demo \
    src/main.cpp src/false_sharing.cpp

echo "Build successful: false_sharing_demo"