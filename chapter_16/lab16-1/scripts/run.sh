#!/bin/bash
# chapter_16/lab16-1/scripts/run.sh
# 运行脚本

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LAB_DIR="$(dirname "$SCRIPT_DIR")"

cd "$LAB_DIR"

if [ ! -f false_sharing_demo ]; then
    echo "Error: false_sharing_demo not found. Run build.sh first."
    exit 1
fi

echo "Running false_sharing_demo..."
./false_sharing_demo