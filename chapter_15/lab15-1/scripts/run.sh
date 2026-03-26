#!/bin/bash
# run.sh - 运行脚本

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
LAB_DIR="$(dirname "$SCRIPT_DIR")"

cd "$LAB_DIR"

if [ ! -f benos.bin ]; then
    echo "Error: benos.bin not found. Run build.sh first."
    exit 1
fi

echo "Running lab15-1 in QEMU..."
echo "Press Ctrl+A then X to exit QEMU"
echo ""

qemu-system-aarch64 \
    -machine raspi4 \
    -cpu cortex-a72 \
    -nographic \
    -kernel benos.bin \
    -serial mon:stdio