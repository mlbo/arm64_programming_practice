#!/bin/bash
BOARD=${board:-rpi4}
QEMU_FLAGS="-machine ${BOARD#rpi} -nographic"
qemu-system-aarch64 $QEMU_FLAGS -kernel benos.bin