#!/bin/bash
# build.sh - 编译脚本

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
LAB_DIR="$(dirname "$SCRIPT_DIR")"

cd "$LAB_DIR"

echo "Building lab15-2..."

make clean 2>/dev/null || true
make

echo "Build complete!"