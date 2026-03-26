# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is the companion code repository for "ARM64体系结构：编程与实践" (ARM64 Architecture: Programming and Practice), a Chinese book about ARMv8/v9 architecture. Contains lab exercises organized by chapter (ch2-23), including bare-metal OS development (BenOS) and SIMD programming examples.

## Build Commands

### Standard Lab (bare-metal, e.g., chapters 2-16, 20-21)

```bash
cd chapter_X/labXX-Y
make                        # Build (generates benos.bin, benos.elf)
make board=rpi3            # Build for Raspberry Pi 3B
make clean                 # Clean build artifacts
make run                   # Run in QEMU (raspi4 by default)
make debug                 # Run in QEMU with GDB server on port 1234
```

### Chapter 21 BenOS (complex kernel build)

```bash
cd chapter_21/lab21-2/benos
make                       # Build benos kernel
make clean                 # Clean
make mrproper              # Deep clean (removes .config)
make defconfig             # Generate default config
```

### Chapter 23 SVE Examples (host-compiled)

```bash
cd chapter_23/example_hello_sve
make                       # Build with native gcc (requires ARM64 host with SVE)
./hello                    # Run directly on ARM64 host
```

### Chapter 18 Litmus Tests (memory model verification)

```bash
# Install herdtools7 first
herd7 -cat aarch64.cat example_18_12_asm.litmus
```

## Toolchain

- **Cross-compiler**: `aarch64-linux-gnu-gcc` (set via `ARMGNU ?= aarch64-linux-gnu`)
- **QEMU**: `qemu-system-aarch64 -machine raspi4` (or `raspi3`)
- **Required**: GCC 9.3+ for aarch64 target

## Architecture

### Project Structure

```
chapter_X/
├── labXX-Y/           # Individual lab
│   ├── src/           # C (.c) and assembly (.S) source files
│   ├── include/       # Headers
│   ├── Makefile       # Build configuration
│   └── linker.ld      # Linker script
└── example_*/         # Standalone examples
```

### Key Chapters

| Track | Chapters | Content |
|-------|----------|---------|
| Bare-metal | 2-16 | Assembly basics, exception handling, interrupts, GIC, cache |
| Memory Model | 18 | Litmus tests for memory consistency |
| Atomics | 20 | LDXR/STXR implementation |
| Kernel | 21 | Complete BenOS with process/memory management |
| NEON | 22 | NEON SIMD intrinsics |
| SVE | 23 | SVE predicate programming |

### BenOS Architecture (Chapter 21)

Linux-kernel-style build system with Kconfig. Key directories:
- `arch/arm64/` - Architecture-specific code (boot, kernel, mach-rpi)
- `kernel/` - Core kernel (scheduler, fork)
- `mm/` - Memory management
- `init/` - Kernel initialization
- `lib/` - Library functions

## Development Notes

### Build Flags (Bare-metal)

```makefile
COPS += -g -Wall -nostdlib -Iinclude -mgeneral-regs-only
```

- `-nostdlib`: No standard library
- `-mgeneral-regs-only`: No FP/SIMD registers in assembly (for boot code)
- `-ffreestanding`: Freestanding environment

### Run Targets

- `make run`: Launches QEMU with `-nographic` for UART console
- `make debug`: QEMU with `-S -s` flags (halts at entry, GDB server on :1234)

### Debugging

```bash
# Terminal 1
make debug

# Terminal 2
aarch64-linux-gnu-gdb benos.elf
(gdb) target remote :1234
(gdb) break _start
(gdb) continue
```

## Planned Refactoring

See `docs/cpp-refactor-design.md` for the C++ migration plan:
- Namespace: `benos` (bare-metal) or `arm64lab` (standalone)
- Code style: Reference ncnn project
- Each chapter will have standardized documentation and build scripts