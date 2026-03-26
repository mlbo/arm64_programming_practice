#!/bin/bash
# Run script for lab09-1

BOARD=${board:-rpi4}

if [ "$BOARD" = "rpi3" ]; then
    QEMU_FLAGS="-machine raspi3b"
else
    QEMU_FLAGS="-machine raspi4b"
fi

QEMU_FLAGS="$QEMU_FLAGS -nographic"

echo "Running BenOS on QEMU ($BOARD)..."
echo "Press Ctrl+A, X to exit"
echo ""

qemu-system-aarch64 $QEMU_FLAGS -kernel benos.bin