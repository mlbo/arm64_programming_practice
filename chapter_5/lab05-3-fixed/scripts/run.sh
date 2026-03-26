#!/bin/bash
BOARD=${board:-rpi4}
[ "$BOARD" = "rpi3" ] && QEMU_MACHINE="raspi3b" || QEMU_MACHINE="raspi4b"
qemu-system-aarch64 -machine "$QEMU_MACHINE" -nographic -kernel benos.bin
