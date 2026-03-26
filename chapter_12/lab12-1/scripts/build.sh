#!/bin/bash
# build.sh - 编译脚本

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
LAB_DIR="$(dirname "$SCRIPT_DIR")"

cd "$LAB_DIR"

echo "Building lab12-1..."

# 清理旧文件
rm -f *.bin *.elf *.o

# 编译汇编文件
echo "  Compiling assembly files..."
aarch64-linux-gnu-gcc -c src/boot.S -o boot.o
aarch64-linux-gnu-gcc -c src/early_uart.S -o early_uart.o
aarch64-linux-gnu-gcc -c src/entry.S -o entry.o
aarch64-linux-gnu-gcc -c src/asm_test.S -o asm_test.o
aarch64-linux-gnu-gcc -c src/atomic.S -o atomic.o
aarch64-linux-gnu-gcc -c src/mm.S -o mm.o
aarch64-linux-gnu-gcc -c src/memset_asm.S -o memset_asm.o

# 编译 C 文件
echo "  Compiling C files..."
aarch64-linux-gnu-gcc -c src/kernel.c -o kernel.o -I include -Wall -nostdlib
aarch64-linux-gnu-gcc -c src/irq.c -o irq.o -I include -Wall -nostdlib
aarch64-linux-gnu-gcc -c src/timer.c -o timer.o -I include -Wall -nostdlib
aarch64-linux-gnu-gcc -c src/pl_uart.c -o pl_uart.o -I include -Wall -nostdlib
aarch64-linux-gnu-gcc -c src/printk.c -o printk.o -I include -Wall -nostdlib
aarch64-linux-gnu-gcc -c src/string.c -o string.o -I include -Wall -nostdlib
aarch64-linux-gnu-gcc -c src/memset.c -o memset.o -I include -Wall -nostdlib

# 链接
echo "  Linking..."
aarch64-linux-gnu-ld -nostdlib -T src/linker.ld \
    boot.o early_uart.o entry.o asm_test.o atomic.o mm.o memset_asm.o \
    kernel.o irq.o timer.o pl_uart.o printk.o string.o memset.o \
    -o benos.elf

# 生成二进制
echo "  Generating binary..."
aarch64-linux-gnu-objcopy -O binary benos.elf benos.bin

echo "Build complete: benos.elf, benos.bin"
echo ""
ls -la benos.elf benos.bin