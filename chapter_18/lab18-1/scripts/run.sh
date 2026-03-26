#!/bin/bash
# chapter_18/lab18-1/scripts/run.sh
# 运行脚本

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LAB_DIR="$(dirname "$SCRIPT_DIR")"

cd "$LAB_DIR"

if [ ! -f litmus_test ]; then
    echo "Error: litmus_test not found. Run build.sh first."
    exit 1
fi

echo "Running litmus_test..."
./litmus_test