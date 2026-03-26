#!/bin/bash
# Run script for BenOS in QEMU

set -e

BOARD=${board:-rpi4}

if [ "$BOARD" = "rpi3" ]; then
    QEMU_MACHINE="raspi3"
else
    QEMU_MACHINE="raspi4"
fi

echo "Running BenOS on ${QEMU_MACHINE}..."

qemu-system-aarch64 \
    -machine ${QEMU_MACHINE} \
    -nographic \
    -kernel benos.bin