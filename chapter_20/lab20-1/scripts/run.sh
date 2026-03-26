#!/bin/bash
# chapter_20/lab20-1/scripts/run.sh
# 运行脚本

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LAB_DIR="$(dirname "$SCRIPT_DIR")"

cd "$LAB_DIR"

if [ ! -f atomic_demo ]; then
    echo "Error: atomic_demo not found. Run build.sh first."
    exit 1
fi

echo "Running atomic_demo..."
./atomic_demo