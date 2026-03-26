#!/bin/bash
# Build script for BenOS

set -e

ARMGNU=${ARMGNU:-aarch64-linux-gnu}
BOARD=${board:-rpi4}

echo "Building BenOS for ${BOARD}..."

make clean 2>/dev/null || true
make board=${BOARD}

echo "Build complete. Output: benos.bin"