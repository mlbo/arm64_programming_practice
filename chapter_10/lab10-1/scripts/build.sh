#!/bin/bash
# Build script for lab10-1

set -e
ARMGNU=${ARMGNU:-aarch64-linux-gnu}
BOARD=${board:-rpi4}

COPS="-g -Wall -nostdlib -Iinclude -mgeneral-regs-only -ffreestanding -std=c++17"

if [ "$BOARD" = "rpi3" ]; then
    COPS="$COPS -DCONFIG_BOARD_PI3B"
else
    COPS="$COPS -DCONFIG_BOARD_PI4B"
fi

mkdir -p build

for src in src/*.cpp; do
    obj="build/$(basename ${src%.cpp}.o)"
    ${ARMGNU}-g++ $COPS -c $src -o $obj
done

for src in src/*.S; do
    obj="build/$(basename ${src%.S}.o)"
    ${ARMGNU}-gcc -g -c $src -o $obj
done

${ARMGNU}-ld -T src/linker.ld -o build/benos.elf build/*.o
${ARMGNU}-objcopy build/benos.elf -O binary benos.bin
echo "Build complete"