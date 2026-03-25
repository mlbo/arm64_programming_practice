# SIMD Track Wave 1 Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Establish the SIMD/SVE migration pattern by converting `chapter_23/example_rgb24_bgr24` to C++17, preserving the explicit SVE assembly boundary, and adding a reusable RGB/BGR validation helper for later SIMD chapters.

**Architecture:** Keep the chapter-local Makefile and the explicit `rgb24_bgr24_asm` assembly function, then move the host-side driver into C++17 with typed helpers, a shared validation layer, and explicit remote execution on `local_pc` for SVE-capable runtime verification.

**Tech Stack:** GNU Make, hosted `g++`/clang++, SVE assembly, C++17, repository-local SIMD helper header, remote ARM64 runtime verification on `local_pc`.

---

## File Map

- Create: `make/simd.mk`
- Create: `shared/simd/rgb24.hpp`
- Create: `chapter_23/example_rgb24_bgr24/rgb24_bgr24_sve.cpp`
- Modify: `chapter_23/example_rgb24_bgr24/Makefile`
- Modify: `chapter_23/example_rgb24_bgr24/rgb24_bgr24_asm.S`

## Baseline Facts To Respect

- `make -C chapter_23/example_rgb24_bgr24 clean all` currently fails because `clean` uses `rm` without `-f`.
- `make -C chapter_23/example_rgb24_bgr24 all` currently fails because there is no `all` target.
- `make -C chapter_23/example_rgb24_bgr24 rgb24_bgr24_sve` currently fails because `rgb24_bgr24_sve.c` calls `rgb24_bgr24_asm` without a prototype.

### Task 1: Repair the Chapter Build Entry Point and Introduce SIMD C++ Rules

**Files:**
- Create: `make/simd.mk`
- Modify: `chapter_23/example_rgb24_bgr24/Makefile`
- Test: `chapter_23/example_rgb24_bgr24/Makefile`

- [ ] **Step 1: Reproduce the missing-target and broken-clean baseline**

Run: `make -C chapter_23/example_rgb24_bgr24 clean all`
Expected: FAIL on `clean`, then still lack a default `all` target

- [ ] **Step 2: Add a shared SIMD make fragment and normalize the chapter Makefile**

```make
CXX ?= g++
CXXFLAGS += -std=gnu++17 -Wall -Wextra -pedantic -march=armv8-a+sve
all: rgb24_bgr24_sve
clean:
	rm -f *.o rgb24_bgr24_sve
```

- [ ] **Step 3: Re-run the chapter build and confirm the next failure is the missing assembly prototype in the source**

Run: `make -C chapter_23/example_rgb24_bgr24 clean all`
Expected: FAIL in the C/C++ source, not in `clean` or target selection

- [ ] **Step 4: Preserve the existing binary name and assembly object rule**

Run: `make -C chapter_23/example_rgb24_bgr24 -n all`
Expected: The plan still builds `rgb24_bgr24_sve` from one C++ unit and one assembly unit

- [ ] **Step 5: Commit**

```bash
git add make/simd.mk chapter_23/example_rgb24_bgr24/Makefile
git commit -m "build: add simd c++ rules for chapter23"
```

### Task 2: Convert the SVE Driver to C++ and Fix the Assembly Boundary

**Files:**
- Create: `chapter_23/example_rgb24_bgr24/rgb24_bgr24_sve.cpp`
- Modify: `chapter_23/example_rgb24_bgr24/rgb24_bgr24_asm.S`
- Test: `chapter_23/example_rgb24_bgr24/rgb24_bgr24_sve.cpp`

- [ ] **Step 1: Point the Makefile at `rgb24_bgr24_sve.cpp` before the file exists**

Run: `make -C chapter_23/example_rgb24_bgr24 clean all`
Expected: FAIL because the C++ driver file has not been created yet

- [ ] **Step 2: Create the minimal C++ driver with an explicit assembly prototype**

```cpp
extern "C" void rgb24_bgr24_asm(unsigned char* src, unsigned char* dst, unsigned long count);
```

- [ ] **Step 3: Rebuild and confirm the source now compiles cleanly**

Run: `make -C chapter_23/example_rgb24_bgr24 clean all`
Expected: PASS build, or fail only on any remaining SVE-specific warning that must be fixed explicitly

- [ ] **Step 4: Keep the SVE assembly symbol stable and add `extern` comments if needed**

Run: `nm chapter_23/example_rgb24_bgr24/rgb24_bgr24_sve | grep rgb24_bgr24_asm`
Expected: The binary still links against the explicit assembly symbol

- [ ] **Step 5: Commit**

```bash
git add chapter_23/example_rgb24_bgr24/rgb24_bgr24_sve.cpp chapter_23/example_rgb24_bgr24/rgb24_bgr24_asm.S chapter_23/example_rgb24_bgr24/Makefile
git commit -m "chapter23: migrate rgb24_bgr24 sve driver to c++"
```

### Task 3: Extract a Shared RGB/BGR Validation Helper

**Files:**
- Create: `shared/simd/rgb24.hpp`
- Modify: `chapter_23/example_rgb24_bgr24/rgb24_bgr24_sve.cpp`
- Test: `shared/simd/rgb24.hpp`

- [ ] **Step 1: Leave the buffer allocation and comparison logic inline first and confirm the build still passes**

Run: `make -C chapter_23/example_rgb24_bgr24 clean all`
Expected: PASS before the helper extraction

- [ ] **Step 2: Move the typed buffer and comparison logic into a shared helper**

```cpp
namespace shared::simd {
struct RgbBuffers {
  std::vector<unsigned char> src;
  std::vector<unsigned char> c_ref;
  std::vector<unsigned char> asm_out;
};
bool equal_rgb24(const std::vector<unsigned char>& lhs, const std::vector<unsigned char>& rhs);
}
```

- [ ] **Step 3: Rebuild and confirm the chapter now depends on the shared helper instead of open-coded comparisons**

Run: `make -C chapter_23/example_rgb24_bgr24 clean all`
Expected: PASS

- [ ] **Step 4: Run the binary remotely and confirm the equality messages still print**

Run: `ssh local_pc 'cd /Users/boml/claude/arm64_programming_practice/chapter_23/example_rgb24_bgr24 && ./rgb24_bgr24_sve'`
Expected: PASS with the “identical” result lines still present

- [ ] **Step 5: Commit**

```bash
git add shared/simd/rgb24.hpp chapter_23/example_rgb24_bgr24/rgb24_bgr24_sve.cpp
git commit -m "shared: add rgb24 validation helper for simd track"
```

### Task 4: Add the Remote Verification Path for the SIMD Slice

**Files:**
- Test: `chapter_23/example_rgb24_bgr24/Makefile`

- [ ] **Step 1: Rebuild the binary locally as the fast compile gate**

Run: `make -C chapter_23/example_rgb24_bgr24 clean all`
Expected: PASS locally

- [ ] **Step 2: Sync the repository to `local_pc:/Users/boml/claude`**

Run: `rsync -a --delete ./ local_pc:/Users/boml/claude/arm64_programming_practice/`
Expected: PASS

- [ ] **Step 3: Rebuild the chapter on `local_pc`**

Run: `ssh local_pc 'cd /Users/boml/claude/arm64_programming_practice/chapter_23/example_rgb24_bgr24 && make clean all'`
Expected: PASS on the remote ARM64 host

- [ ] **Step 4: Execute the binary remotely and confirm the equivalence checks still succeed**

Run: `ssh local_pc 'cd /Users/boml/claude/arm64_programming_practice/chapter_23/example_rgb24_bgr24 && ./rgb24_bgr24_sve'`
Expected: PASS with the equality messages and no runtime crash

- [ ] **Step 5: Commit**

```bash
git add make/simd.mk chapter_23/example_rgb24_bgr24/Makefile chapter_23/example_rgb24_bgr24/rgb24_bgr24_sve.cpp shared/simd/rgb24.hpp
git commit -m "verify: validate simd wave 1 locally and on local_pc"
```
