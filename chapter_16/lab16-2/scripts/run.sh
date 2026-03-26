#!/bin/bash
# chapter_16/lab16-2/scripts/run.sh
# 运行脚本

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LAB_DIR="$(dirname "$SCRIPT_DIR")"

cd "$LAB_DIR"

if [ ! -f perf_demo ]; then
    echo "Error: perf_demo not found. Run build.sh first."
    exit 1
fi

echo "Running perf_demo..."
./perf_demo

echo ""
echo "To analyze with perf c2c, run:"
echo "  sudo perf c2c record ./perf_demo"
echo "  sudo perf c2c report"