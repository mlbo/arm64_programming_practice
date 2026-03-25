# Concurrency Track Wave 1 Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Establish the hosted C++ migration pattern by converting `chapter_16/lab16-1` to C++17, cleaning up its existing build issues, and extracting a small shared concurrency helper that later false-sharing and atomic examples can reuse.

**Architecture:** Keep the chapter-local `Makefile` as the entry point, add a shared hosted make fragment, then migrate the two false-sharing programs to C++17 using typed thread workers, `alignas(64)`, and shared timing/report helpers. This chapter becomes the baseline template for the concurrency/memory-model track.

**Tech Stack:** GNU Make, hosted `g++`/clang++, POSIX threads, C++17, repository-local hosted make fragment, repository-local concurrency helper header.

---

## File Map

- Create: `make/hosted.mk`
- Create: `shared/concurrency/false_sharing.hpp`
- Create: `chapter_16/lab16-1/false_sharing_array.cpp`
- Create: `chapter_16/lab16-1/false_sharing_struct.cpp`
- Modify: `chapter_16/lab16-1/Makefile`

## Baseline Facts To Respect

- `make -C chapter_16/lab16-1 clean all` currently fails because `clean` uses `rm` without `-f`.
- `make -C chapter_16/lab16-1 all` succeeds, but both worker functions warn because they do not return a value.

### Task 1: Fix the Build Entry Point and Introduce Hosted C++ Rules

**Files:**
- Create: `make/hosted.mk`
- Modify: `chapter_16/lab16-1/Makefile`
- Test: `chapter_16/lab16-1/Makefile`

- [ ] **Step 1: Reproduce the current failing `clean` behavior**

Run: `make -C chapter_16/lab16-1 clean all`
Expected: FAIL on `rm` because the binaries do not exist yet

- [ ] **Step 2: Add a hosted make fragment and switch the chapter Makefile to `rm -f`, `.PHONY`, and explicit `CXX`/`CXXFLAGS`**

```make
CXX ?= g++
CXXFLAGS += -std=gnu++17 -Wall -Wextra -pedantic -pthread
```

- [ ] **Step 3: Re-run the chapter build and confirm `clean all` now succeeds with the existing C baseline**

Run: `make -C chapter_16/lab16-1 clean all`
Expected: PASS, but warnings from missing worker returns still appear

- [ ] **Step 4: Preserve the output binary names so existing chapter usage does not change**

Run: `ls chapter_16/lab16-1/false_sharing_struct chapter_16/lab16-1/false_sharing_array`
Expected: Both binaries exist with their original names

- [ ] **Step 5: Commit**

```bash
git add make/hosted.mk chapter_16/lab16-1/Makefile
git commit -m "build: add hosted c++ rules for chapter16"
```

### Task 2: Convert `false_sharing_array` to C++17

**Files:**
- Create: `chapter_16/lab16-1/false_sharing_array.cpp`
- Modify: `chapter_16/lab16-1/Makefile`
- Test: `chapter_16/lab16-1/false_sharing_array.cpp`

- [ ] **Step 1: Point the chapter build at `false_sharing_array.cpp` before the file exists**

Run: `make -C chapter_16/lab16-1 clean all`
Expected: FAIL because the new C++ source file has not been created yet

- [ ] **Step 2: Write the minimal C++ version with a typed worker that returns `nullptr`**

```cpp
static void* access_data_array(void* param) {
  auto index = *static_cast<int*>(param);
  for (std::uint64_t i = 0; i < MAX_LOOP; ++i) {
    data[index] += 1;
  }
  return nullptr;
}
```

- [ ] **Step 3: Build again and confirm the new binary compiles without the old non-void warning**

Run: `make -C chapter_16/lab16-1 clean all`
Expected: PASS, with `false_sharing_array` warning-free

- [ ] **Step 4: Execute the binary and confirm both timing lines still print**

Run: `./chapter_16/lab16-1/false_sharing_array`
Expected: Output still includes both `cache false sharing` and `cache without false sharing`

- [ ] **Step 5: Commit**

```bash
git add chapter_16/lab16-1/false_sharing_array.cpp chapter_16/lab16-1/Makefile
git commit -m "chapter16: migrate false_sharing_array to c++"
```

### Task 3: Convert `false_sharing_struct` to C++17 With Typed Padding

**Files:**
- Create: `chapter_16/lab16-1/false_sharing_struct.cpp`
- Test: `chapter_16/lab16-1/false_sharing_struct.cpp`

- [ ] **Step 1: Point the chapter build at `false_sharing_struct.cpp` before creating it**

Run: `make -C chapter_16/lab16-1 clean all`
Expected: FAIL because the struct variant has not been ported yet

- [ ] **Step 2: Rebuild the data layout with `alignas(64)` and an explicit padding type**

```cpp
struct alignas(64) DataWithoutFalseSharing {
  std::uint64_t x;
  std::byte pad[64];
  std::uint64_t y;
};
```

- [ ] **Step 3: Build again and confirm the worker warnings are gone for both binaries**

Run: `make -C chapter_16/lab16-1 clean all`
Expected: PASS, no `-Wreturn-type` warnings

- [ ] **Step 4: Execute the struct binary and confirm both timing lines still print**

Run: `./chapter_16/lab16-1/false_sharing_struct`
Expected: Output still includes both false-sharing and no-false-sharing timings

- [ ] **Step 5: Commit**

```bash
git add chapter_16/lab16-1/false_sharing_struct.cpp chapter_16/lab16-1/Makefile
git commit -m "chapter16: migrate false_sharing_struct to c++"
```

### Task 4: Extract the Shared False-Sharing Helper

**Files:**
- Create: `shared/concurrency/false_sharing.hpp`
- Modify: `chapter_16/lab16-1/false_sharing_array.cpp`
- Modify: `chapter_16/lab16-1/false_sharing_struct.cpp`
- Test: `shared/concurrency/false_sharing.hpp`

- [ ] **Step 1: Duplicate a small timing helper in both C++ programs on purpose and confirm the build still passes**

Run: `make -C chapter_16/lab16-1 clean all`
Expected: PASS, but helper duplication is visible

- [ ] **Step 2: Move the shared timing/report logic into `shared/concurrency/false_sharing.hpp`**

```cpp
namespace shared::concurrency {
using Clock = std::chrono::steady_clock;
inline auto elapsed_ms(Clock::time_point begin, Clock::time_point end) -> std::uint64_t;
}
```

- [ ] **Step 3: Rebuild and confirm both binaries now consume the shared helper**

Run: `make -C chapter_16/lab16-1 clean all`
Expected: PASS with no duplicated timing helper code in the chapter sources

- [ ] **Step 4: Re-run both binaries to confirm the refactor preserved output**

Run: `./chapter_16/lab16-1/false_sharing_array && ./chapter_16/lab16-1/false_sharing_struct`
Expected: PASS with the same pair of timing lines from each binary

- [ ] **Step 5: Commit**

```bash
git add shared/concurrency/false_sharing.hpp chapter_16/lab16-1/false_sharing_array.cpp chapter_16/lab16-1/false_sharing_struct.cpp
git commit -m "shared: add false-sharing helper for concurrency track"
```

### Task 5: Add Remote Verification for the Hosted Concurrency Slice

**Files:**
- Test: `chapter_16/lab16-1/Makefile`

- [ ] **Step 1: Run the full hosted build locally as the first verification gate**

Run: `make -C chapter_16/lab16-1 clean all`
Expected: PASS

- [ ] **Step 2: Run both binaries locally and capture their outputs**

Run: `./chapter_16/lab16-1/false_sharing_array && ./chapter_16/lab16-1/false_sharing_struct`
Expected: PASS with the expected timing output pairs

- [ ] **Step 3: Sync the repo to `local_pc:/Users/boml/claude` and rebuild there**

Run: `rsync -a --delete ./ local_pc:/Users/boml/claude/arm64_programming_practice/ && ssh local_pc 'cd /Users/boml/claude/arm64_programming_practice/chapter_16/lab16-1 && make clean all'`
Expected: PASS on the remote host

- [ ] **Step 4: Execute both binaries remotely and confirm the same output pattern**

Run: `ssh local_pc 'cd /Users/boml/claude/arm64_programming_practice/chapter_16/lab16-1 && ./false_sharing_array && ./false_sharing_struct'`
Expected: PASS with the same pair of timing lines per binary

- [ ] **Step 5: Commit**

```bash
git add chapter_16/lab16-1/Makefile chapter_16/lab16-1/false_sharing_array.cpp chapter_16/lab16-1/false_sharing_struct.cpp shared/concurrency/false_sharing.hpp
git commit -m "verify: validate concurrency wave 1 locally and on local_pc"
```
