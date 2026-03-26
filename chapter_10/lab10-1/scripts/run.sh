#!/bin/bash
# Run script for lab10-1

BOARD=${board:-rpi4}

if [ "$BOARD" = "rpi3" ]; then
    QEMU_FLAGS="-machine raspi3b"
else
    QEMU_FLAGS="-machine raspi4b"
fi

qemu-system-aarch64 $QEMU_FLAGS -nographic -kernel benos.bin