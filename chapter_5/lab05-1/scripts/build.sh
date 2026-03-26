#!/bin/bash
set -e
ARMGNU=${ARMGNU:-aarch64-linux-gnu}
BOARD=${board:-rpi4}
COPS="-g -Wall -nostdlib -nostdinc -Iinclude"
CPPOPS="-g -Wall -nostdlib -nostdinc -Iinclude -fno-exceptions -fno-rtti -std=c++17"
ASMOPS="-g -Iinclude"
[ "$BOARD" = "rpi3" ] && COPS="$COPS -DCONFIG_BOARD_PI3B" || COPS="$COPS -DCONFIG_BOARD_PI4B"
BUILD_DIR="build"
SRC_DIR="src"
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
for f in "$SRC_DIR"/*.c; do [ -f "$f" ] && "$ARMGNU-gcc" $COPS -c "$f" -o "$BUILD_DIR/$(basename $f .c)_c.o"; done
for f in "$SRC_DIR"/*.cpp; do [ -f "$f" ] && "$ARMGNU-g++" $CPPOPS -c "$f" -o "$BUILD_DIR/$(basename $f .cpp)_cpp.o"; done
for f in "$SRC_DIR"/*.S; do [ -f "$f" ] && "$ARMGNU-gcc" $ASMOPS -c "$f" -o "$BUILD_DIR/$(basename $f .S)_s.o"; done
OBJ_FILES=$(find "$BUILD_DIR" -name "*.o" | tr '\n' ' ')
"$ARMGNU-ld" -T "$SRC_DIR/linker.ld" -o "$BUILD_DIR/benos.elf" $OBJ_FILES
"$ARMGNU-objcopy" "$BUILD_DIR/benos.elf" -O binary benos.bin
echo "Build successful: benos.bin"
