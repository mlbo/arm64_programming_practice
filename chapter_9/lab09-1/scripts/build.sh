#!/bin/bash
# Build script for lab09-1

set -e

# Default toolchain
ARMGNU=${ARMGNU:-aarch64-linux-gnu}

# Board selection
BOARD=${board:-rpi4}

# Compiler flags
COPS="-g -Wall -nostdlib -Iinclude -mgeneral-regs-only -ffreestanding -std=c++17"
ASMOPS="-g -Iinclude"

if [ "$BOARD" = "rpi3" ]; then
    COPS="$COPS -DCONFIG_BOARD_PI3B"
else
    COPS="$COPS -DCONFIG_BOARD_PI4B"
fi

# Create build directory
mkdir -p build

# Compile C++ files
for src in src/*.cpp; do
    obj="build/$(basename ${src%.cpp}.o)"
    echo "Compiling $src..."
    ${ARMGNU}-g++ $COPS -c $src -o $obj
done

# Compile assembly files
for src in src/*.S; do
    obj="build/$(basename ${src%.S}.o)"
    echo "Assembling $src..."
    ${ARMGNU}-gcc $ASMOPS -c $src -o $obj
done

# Link
echo "Linking..."
${ARMGNU}-ld -T linker.ld -Map build/benos.map -o build/benos.elf build/*.o

# Generate binary
${ARMGNU}-objcopy build/benos.elf -O binary benos.bin
cp build/benos.elf benos.elf

echo "Build complete: benos.bin"