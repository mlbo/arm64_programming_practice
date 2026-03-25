# Bare-Metal Track Wave 1 Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Establish the repository-wide bare-metal C++ pattern by wiring shared freestanding C++ build support into `chapter_15/lab15-2`, then migrating its boot entry, console, memory, and IRQ/timer stack behind a controlled C++ runtime.

**Architecture:** Keep all assembly entry points and linker-controlled memory layout stable, then insert a minimal freestanding C++ runtime and a new `extern "C"` kernel entry that orchestrates C++ components. Use this chapter as the template that later chapters in the bare-metal track will copy with smaller chapter-specific adjustments.

**Tech Stack:** GNU Make, freestanding `aarch64-linux-gnu-g++`, ARM64 assembly, linker script sections, QEMU, repository-local shared make fragments, repository-local bare-metal runtime helpers.

---

## File Map

- Create: `make/baremetal.mk`
- Create: `shared/baremetal/runtime/cxx_runtime.hpp`
- Create: `shared/baremetal/runtime/cxx_runtime.cpp`
- Create: `shared/baremetal/runtime/new_delete.cpp`
- Create: `scripts/verify/baremetal_wave1_local_pc.sh`
- Create: `chapter_15/lab15-2/src/kernel_main.cpp`
- Modify: `chapter_15/lab15-2/Makefile`
- Modify: `chapter_15/lab15-2/src/linker.ld`
- Modify: `chapter_15/lab15-2/src/kernel.c`
- Modify: `chapter_15/lab15-2/src/pl_uart.c`
- Modify: `chapter_15/lab15-2/src/printk.c`
- Modify: `chapter_15/lab15-2/src/page_alloc.c`
- Modify: `chapter_15/lab15-2/src/mmu.c`
- Modify: `chapter_15/lab15-2/src/timer.c`
- Modify: `chapter_15/lab15-2/src/gic_v2.c`
- Modify: `chapter_15/lab15-2/src/irq.c`

## Baseline Facts To Respect

- `make -C chapter_15/lab15-2 clean all` currently fails locally because `aarch64-linux-gnu-gcc` is not installed.
- Local bare-metal build verification must therefore include an explicit toolchain probe.
- Runtime validation must be repeated on `local_pc:/Users/boml/claude`.

### Task 1: Add Shared Bare-Metal Build Rules and Toolchain Probe

**Files:**
- Create: `make/baremetal.mk`
- Create: `scripts/verify/baremetal_wave1_local_pc.sh`
- Modify: `chapter_15/lab15-2/Makefile`
- Test: `chapter_15/lab15-2/Makefile`

- [ ] **Step 1: Write the failing preflight check into the plan execution script**

```bash
command -v aarch64-linux-gnu-gcc
command -v aarch64-linux-gnu-g++
```

- [ ] **Step 2: Run the preflight locally and confirm it fails**

Run: `command -v aarch64-linux-gnu-gcc && command -v aarch64-linux-gnu-g++`
Expected: FAIL locally with missing command(s)

- [ ] **Step 3: Add shared bare-metal make rules and a `toolchain-check` target**

```make
ARMCROSS ?= aarch64-linux-gnu
CC := $(ARMCROSS)-gcc
CXX := $(ARMCROSS)-g++
CPPFLAGS += -Iinclude -I../../shared/baremetal/runtime
CXXFLAGS += -std=gnu++17 -ffreestanding -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit
```

- [ ] **Step 4: Wire `chapter_15/lab15-2/Makefile` to use `make/baremetal.mk` and preserve current assembly rules**

Run: `make -C chapter_15/lab15-2 toolchain-check`
Expected: PASS on `local_pc`, clear failure message locally if the toolchain is absent

- [ ] **Step 5: Commit**

```bash
git add make/baremetal.mk scripts/verify/baremetal_wave1_local_pc.sh chapter_15/lab15-2/Makefile
git commit -m "build: add bare-metal c++ make fragment"
```

### Task 2: Add Minimal Freestanding C++ Runtime and Linker Support

**Files:**
- Create: `shared/baremetal/runtime/cxx_runtime.hpp`
- Create: `shared/baremetal/runtime/cxx_runtime.cpp`
- Create: `shared/baremetal/runtime/new_delete.cpp`
- Modify: `chapter_15/lab15-2/src/linker.ld`
- Test: `chapter_15/lab15-2/src/linker.ld`

- [ ] **Step 1: Add the failing linker expectation by referencing `__init_array_start` and `__init_array_end` from a new runtime unit**

```cpp
extern "C" void (*__init_array_start[])();
extern "C" void (*__init_array_end[])();
```

- [ ] **Step 2: Run the chapter build remotely and confirm the linker fails because the symbols/sections are not defined yet**

Run: `ssh local_pc 'cd /Users/boml/claude/arm64_programming_practice && make -C chapter_15/lab15-2 clean all'`
Expected: FAIL with missing `init_array` section support or unresolved runtime symbols

- [ ] **Step 3: Add minimal runtime support and linker script sections**

```cpp
namespace shared::baremetal::runtime {
inline void run_global_constructors() {
  for (auto ctor = __init_array_start; ctor != __init_array_end; ++ctor) {
    (*ctor)();
  }
}
}
```

- [ ] **Step 4: Re-run the remote build and confirm runtime objects now link**

Run: `ssh local_pc 'cd /Users/boml/claude/arm64_programming_practice && make -C chapter_15/lab15-2 clean all'`
Expected: Build advances past runtime/linker integration

- [ ] **Step 5: Commit**

```bash
git add shared/baremetal/runtime/cxx_runtime.hpp shared/baremetal/runtime/cxx_runtime.cpp shared/baremetal/runtime/new_delete.cpp chapter_15/lab15-2/src/linker.ld
git commit -m "runtime: add bare-metal c++ startup support"
```

### Task 3: Insert a New C++ Kernel Entry Without Breaking Assembly ABI

**Files:**
- Create: `chapter_15/lab15-2/src/kernel_main.cpp`
- Modify: `chapter_15/lab15-2/src/kernel.c`
- Test: `chapter_15/lab15-2/src/kernel_main.cpp`

- [ ] **Step 1: Change the C build to expect `kernel_main.cpp` before the file exists**

Run: `ssh local_pc 'cd /Users/boml/claude/arm64_programming_practice && make -C chapter_15/lab15-2 clean all'`
Expected: FAIL with a missing C++ kernel entry object

- [ ] **Step 2: Create the minimal C++ entry and rename the legacy C entrypoint**

```cpp
extern "C" void kernel_legacy_main();

extern "C" void kernel_main() {
  shared::baremetal::runtime::run_global_constructors();
  kernel_legacy_main();
}
```

- [ ] **Step 3: Update `kernel.c` so the old implementation is exposed as `kernel_legacy_main`**

Run: `ssh local_pc 'cd /Users/boml/claude/arm64_programming_practice && make -C chapter_15/lab15-2 clean all'`
Expected: PASS and produce `benos.bin`

- [ ] **Step 4: Run QEMU remotely and verify the boot still reaches the existing serial output checkpoints**

Run: `ssh local_pc 'cd /Users/boml/claude/arm64_programming_practice/chapter_15/lab15-2 && timeout 5 make run'`
Expected: Output still includes early EL/memory diagnostics instead of crashing before `kernel_main`

- [ ] **Step 5: Commit**

```bash
git add chapter_15/lab15-2/src/kernel_main.cpp chapter_15/lab15-2/src/kernel.c chapter_15/lab15-2/Makefile
git commit -m "kernel: add c++ entrypoint for lab15-2"
```

### Task 4: Migrate Console and Logging to C++ Boundaries

**Files:**
- Modify: `chapter_15/lab15-2/src/pl_uart.c`
- Modify: `chapter_15/lab15-2/src/printk.c`
- Test: `chapter_15/lab15-2/src/printk.c`

- [ ] **Step 1: Add a failing build expectation by switching the chapter build to compile the console units as C++**

Run: `ssh local_pc 'cd /Users/boml/claude/arm64_programming_practice && make -C chapter_15/lab15-2 clean all'`
Expected: FAIL on C-to-C++ incompatibilities in UART/printk units

- [ ] **Step 2: Convert UART and printk units to C++-safe code while preserving exported symbol names**

```cpp
extern "C" void uart_init();
extern "C" void putchar(char c);
```

- [ ] **Step 3: Replace macro-heavy internals with typed helper functions where possible, but keep assembly-visible ABI stable**

Run: `ssh local_pc 'cd /Users/boml/claude/arm64_programming_practice && make -C chapter_15/lab15-2 clean all'`
Expected: PASS with no new unresolved console symbols

- [ ] **Step 4: Re-run the remote QEMU boot and compare console output to the pre-migration checkpoint**

Run: `ssh local_pc 'cd /Users/boml/claude/arm64_programming_practice/chapter_15/lab15-2 && timeout 5 make run'`
Expected: Same early serial path, same major boot messages

- [ ] **Step 5: Commit**

```bash
git add chapter_15/lab15-2/src/pl_uart.c chapter_15/lab15-2/src/printk.c
git commit -m "console: migrate lab15-2 uart and printk to c++"
```

### Task 5: Migrate Memory and Interrupt Subsystems

**Files:**
- Modify: `chapter_15/lab15-2/src/page_alloc.c`
- Modify: `chapter_15/lab15-2/src/mmu.c`
- Modify: `chapter_15/lab15-2/src/timer.c`
- Modify: `chapter_15/lab15-2/src/gic_v2.c`
- Modify: `chapter_15/lab15-2/src/irq.c`
- Test: `chapter_15/lab15-2/src/timer.c`

- [ ] **Step 1: Move the memory units to C++ compilation and confirm the build fails on the first incompatibility**

Run: `ssh local_pc 'cd /Users/boml/claude/arm64_programming_practice && make -C chapter_15/lab15-2 clean all'`
Expected: FAIL in page allocator or MMU units before behavior changes are made

- [ ] **Step 2: Convert page allocator and MMU units to typed C++ helpers while keeping symbol names stable**

```cpp
namespace kernel::memory {
unsigned long get_free_page();
void free_page(unsigned long page);
}
```

- [ ] **Step 3: Move timer/GIC/IRQ units to C++ compilation and preserve interrupt entry ABI**

Run: `ssh local_pc 'cd /Users/boml/claude/arm64_programming_practice && make -C chapter_15/lab15-2 clean all'`
Expected: PASS and produce bootable artifacts

- [ ] **Step 4: Re-run QEMU and verify timer/GIC behavior still progresses past initialization**

Run: `ssh local_pc 'cd /Users/boml/claude/arm64_programming_practice/chapter_15/lab15-2 && timeout 5 make run'`
Expected: Boot output still reaches timer/cache/interrupt diagnostics without lockup

- [ ] **Step 5: Commit**

```bash
git add chapter_15/lab15-2/src/page_alloc.c chapter_15/lab15-2/src/mmu.c chapter_15/lab15-2/src/timer.c chapter_15/lab15-2/src/gic_v2.c chapter_15/lab15-2/src/irq.c
git commit -m "kernel: migrate lab15-2 memory and irq stack to c++"
```

### Task 6: Finish the Bare-Metal Wave 1 Verification Path

**Files:**
- Modify: `scripts/verify/baremetal_wave1_local_pc.sh`
- Test: `scripts/verify/baremetal_wave1_local_pc.sh`

- [ ] **Step 1: Write the verification script to sync the repo to `local_pc:/Users/boml/claude` and run the chapter build plus QEMU smoke test**

```bash
rsync -a --delete ./ local_pc:/Users/boml/claude/arm64_programming_practice/
ssh local_pc 'cd /Users/boml/claude/arm64_programming_practice/chapter_15/lab15-2 && make clean all && timeout 5 make run'
```

- [ ] **Step 2: Run the script once and confirm it reaches the expected build and smoke-test checkpoints**

Run: `bash scripts/verify/baremetal_wave1_local_pc.sh`
Expected: PASS on remote build and boot smoke test

- [ ] **Step 3: Re-run `git status` and confirm only intended bare-metal files changed**

Run: `git status --short`
Expected: Only bare-metal track files are modified

- [ ] **Step 4: Add a short README comment or script usage header if the sync path is not obvious**

Run: `bash scripts/verify/baremetal_wave1_local_pc.sh`
Expected: PASS with self-explanatory output

- [ ] **Step 5: Commit**

```bash
git add scripts/verify/baremetal_wave1_local_pc.sh
git commit -m "chore: add bare-metal remote verification script"
```
