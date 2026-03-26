#!/bin/bash
# chapter_16/lab16-2/scripts/build.sh
# 编译脚本

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LAB_DIR="$(dirname "$SCRIPT_DIR")"

cd "$LAB_DIR"

echo "Building perf_demo..."

g++ -std=c++17 -O2 -pthread \
    -I./include \
    -o perf_demo \
    src/main.cpp src/perf_demo.cpp

echo "Build successful: perf_demo"