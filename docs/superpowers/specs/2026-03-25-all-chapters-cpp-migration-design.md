# All Chapters C++ Migration Design

## Overview

This document defines the repository-wide design for migrating the ARM64 programming practice repository from primarily C implementations to a structured C++ implementation while preserving the original educational intent of each chapter.

The migration applies to every chapter in the repository. The work will happen in place on the existing directory layout, but it will not be treated as a single flat rewrite. Instead, the repository will be split into technical tracks so that each family of chapters can share the right abstractions, build rules, and validation strategy.

## Goals

1. Migrate every chapter to a C++-based implementation strategy.
2. Preserve the educational value and observable behavior of the original experiments.
3. Improve structure, type safety, and maintainability instead of only renaming `.c` files to `.cpp`.
4. Keep low-level ABI-sensitive assembly boundaries stable where required.
5. Make the migration reproducible on the local machine and on `local_pc` under `/Users/boml/claude`.

## Non-Goals

1. Replacing every assembly file with C++.
2. Introducing a single monolithic build system for the whole repository in the first phase.
3. Forcing identical internal design across bare-metal, host-side concurrency, and SIMD chapters.
4. Enabling full hosted C++ runtime features such as exceptions and RTTI in bare-metal chapters.

## Repository Decomposition

The repository will be migrated through three technical tracks.

### Track A: Bare-Metal Kernel Track

Primary scope:

- `chapter_2`
- `chapter_3`
- `chapter_4`
- `chapter_5`
- `chapter_6`
- `chapter_7`
- `chapter_8`
- `chapter_9`
- `chapter_10`
- `chapter_11`
- `chapter_12`
- `chapter_13`
- `chapter_14`
- `chapter_15`
- `chapter_21`

Characteristics:

- Freestanding environment.
- Mixed assembly, linker script, and C source layout.
- Strong dependency on UART, MMU, exception vectors, timer, interrupt controller, and platform register access.
- High ABI sensitivity.

Migration model:

- Keep boot and exception-critical assembly boundaries intact.
- Move core C modules toward namespaced C++ components.
- Use thin `extern "C"` bridge points where assembly still calls into higher-level code.
- Introduce a minimal freestanding C++ runtime for global constructors and `new/delete`.

### Track B: Concurrency and Memory Model Track

Primary scope:

- `chapter_16`
- `chapter_18`
- `chapter_20`
- selected atomic or concurrency-oriented experiments in other chapters

Characteristics:

- Host-side or analysis-oriented experiments.
- Less dependency on bare-metal runtime.
- Better fit for standard C++17 atomics, threading helpers, and structured benchmark/test harnesses.

Migration model:

- Use standard C++17 where appropriate.
- Preserve memory-ordering semantics and benchmark intent.
- Introduce cleaner test and reporting structure around experiments.

### Track C: SIMD / NEON / SVE Track

Primary scope:

- `chapter_22`
- `chapter_23`

Characteristics:

- Intrinsics and assembly remain central to the instructional value.
- Performance-sensitive microbenchmarks and data movement routines.
- Often best served by C++ wrappers and test harnesses, not by hiding the vector logic.

Migration model:

- Keep SIMD and assembly routines explicit.
- Use C++ for data modeling, validation, benchmark setup, and result comparison.
- Avoid over-abstracting instruction-level demonstrations.

## In-Place Migration Policy

The repository layout will remain chapter-oriented. Existing paths stay intact. Migration happens in place by gradually replacing or wrapping current C files with C++ files and shared headers.

This policy is required to preserve:

- the current chapter navigation,
- the original educational grouping,
- existing README and experiment references,
- predictable validation paths for each lab.

## Shared Infrastructure

To avoid repeating the same runtime and utilities in every chapter, the repository will gain shared infrastructure organized by track.

### `shared/baremetal/`

Purpose:

- freestanding C++ runtime support,
- low-level MMIO wrappers,
- common UART and console abstractions,
- page allocator and page-table helper abstractions,
- platform-neutral support utilities for bare-metal chapters.

Expected contents:

- `cxx_runtime` support for `init_array`, `__cxa_pure_virtual`, and `operator new/delete`,
- `mmio` typed register access helpers,
- `console` and logging abstractions,
- page and address utilities,
- bridge headers used by legacy assembly-facing code.

### `shared/concurrency/`

Purpose:

- reusable benchmark entry points,
- output and reporting helpers,
- common atomic and threading utilities,
- experiment validation helpers.

### `shared/simd/`

Purpose:

- common data generators,
- validation helpers,
- benchmark harness support,
- SIMD-friendly buffer and alignment utilities.

## Build System Strategy

The initial migration will preserve chapter-local build entry points. A repository-wide conversion to CMake or another top-level build system is explicitly deferred.

Instead, the build strategy is:

1. keep chapter-local `Makefile` entry points,
2. introduce shared make fragments for common C++ flags and rules,
3. apply track-specific compiler and linker settings through reusable includes,
4. keep each chapter buildable using commands that remain recognizable to the current repository users.

## C++ Language and Runtime Policy

### Common Policy

- Use `C++17` across the repository.
- Prefer strong typing, `enum class`, namespaces, RAII where appropriate, and focused component boundaries.
- Avoid “rename only” migrations that preserve the same global, macro-heavy structure internally.

### Bare-Metal Policy

For bare-metal chapters:

- compile as freestanding C++,
- disable exceptions,
- disable RTTI,
- allow a small amount of global object construction,
- allow `new/delete`,
- keep constructors simple and side-effect light,
- move hardware activation into explicit `init()` calls rather than implicit global initialization side effects.

The startup sequence remains assembly-led. Assembly transfers control into a stable C ABI entry point, and C++ runtime initialization occurs under explicit repository control.

### Host-Side Policy

For concurrency and SIMD tracks:

- use normal hosted C++17 facilities where they improve structure,
- preserve low-level behavior and performance characteristics,
- avoid abstractions that obscure the teaching value of the experiment.

## Bare-Metal Runtime Design

Bare-metal chapters require a small, explicit C++ runtime layer rather than a full hosted runtime.

That runtime will provide:

- `init_array` execution for global constructors,
- `__cxa_pure_virtual`,
- minimal `operator new/delete`,
- a controlled heap or page-backed allocation strategy,
- no exception unwinding support.

The runtime must be understandable, auditable, and small enough to debug at the boot boundary.

## Module Boundary Design for Bare-Metal Chapters

Bare-metal chapters will follow a layered structure:

1. assembly and linker boundary layer
2. runtime and platform utility layer
3. subsystem layer such as console, memory, timer, and interrupt controller
4. kernel orchestration layer

Assembly-facing symbols remain stable where needed. Internal subsystem logic moves toward clearer C++ interfaces. Any compatibility wrappers should stay thin and temporary.

## Definition of “Synchronous Migration” Across Chapters

“Every chapter must be migrated” is interpreted as follows:

1. all chapters participate in the same repository-wide migration branch,
2. all chapters adopt the same migration rules,
3. all chapters eventually reach the same migration stage,
4. work proceeds per technical track, but no chapter is permanently left behind.

This does not require every chapter to receive the same exact internal abstractions. It requires every chapter to comply with the same migration standard appropriate for its track.

## Milestones

### M1: Shared Infrastructure and Toolchain Enablement

Deliverables:

- migration branch created,
- shared infrastructure directories created,
- common make fragments introduced,
- bare-metal runtime skeleton introduced,
- at least one C++ build target established in every chapter family.

Success criteria:

- representative builds succeed locally,
- representative builds can be reproduced on `local_pc:/Users/boml/claude`.

### M2: Representative Experiment Migration Per Chapter

Deliverables:

- each chapter has at least one representative experiment migrated according to its track rules,
- structure upgrades are real, not superficial,
- representative runtime behavior remains aligned with the original implementation.

Success criteria:

- each chapter demonstrates at least one “fully migrated” experiment,
- behavior deltas are documented when intentionally changed.

### M3: Full Chapter Coverage

Deliverables:

- remaining experiments inside each chapter are migrated,
- duplicated legacy helpers are replaced with shared infrastructure where reasonable,
- transitional wrappers are reduced.

Success criteria:

- core experiments across all chapters build and validate under the new C++ structure,
- no chapter remains half-migrated by design.

### M4: Validation, Cleanup, and Delivery

Deliverables:

- migration notes and updated documentation,
- stable validation commands per chapter,
- remote verification flow documented and exercised,
- cleanup of temporary compatibility layers where safe.

Success criteria:

- repository validation matrix passes locally and on `local_pc`,
- the branch is in a reviewable state for integration planning.

## Validation Matrix

Validation must happen at both the build level and the behavior level.

### Bare-Metal Validation

- build outputs such as `benos.bin` and `benos.elf`,
- boot success in QEMU or the expected platform,
- key serial output checkpoints,
- paging, timer, interrupt, and diagnostic output where applicable.

### Concurrency Validation

- compilation success,
- correct output or invariant checks,
- benchmark/report output consistency,
- preserved memory-ordering intent.

### SIMD Validation

- compilation success,
- functional equivalence of transformed data,
- benchmark harness execution,
- no accidental loss of explicit vectorized teaching content.

### Remote Validation

For all tracks, validation must be reproducible on:

- local machine,
- `local_pc` in `/Users/boml/claude`.

The remote environment is a required verification target, not an optional convenience step.

## Branch and Environment Strategy

The migration will use a dedicated branch:

- `codex/cpp-migration-all-chapters`

Remote verification target:

- host: `local_pc`
- working root: `/Users/boml/claude`

The design assumes that the repository will be synchronized or checked out there for build and runtime verification as implementation proceeds.

## Risks and Mitigations

### Risk: ABI breakage in bare-metal boot paths

Mitigation:

- preserve assembly boundaries first,
- use explicit bridge points,
- validate boot behavior incrementally.

### Risk: repository-wide scope becomes unmanageable

Mitigation:

- split work by track,
- use milestone-based rollout,
- define representative experiments before full chapter coverage.

### Risk: C++ abstractions obscure educational intent

Mitigation:

- keep low-level logic visible,
- avoid over-abstracting SIMD and architecture demonstrations,
- preserve direct visibility into hardware-oriented behavior.

### Risk: local-only success

Mitigation:

- require reproduction on `local_pc:/Users/boml/claude`,
- keep validation commands explicit and chapter-local.

## Testing Strategy During Implementation

Implementation will follow a strict incremental validation approach:

1. introduce the smallest failing build or validation target,
2. confirm the failure is meaningful,
3. add the minimum implementation needed to pass,
4. re-run chapter-local validation,
5. repeat.

For bare-metal chapters, serial output and QEMU behavior act as primary observable test surfaces in addition to build success.

## Recommended First Execution Slice

The first implementation slice should establish the shared infrastructure and toolchain policies, then prove the approach with one representative experiment from each track:

1. a bare-metal kernel chapter,
2. a concurrency chapter,
3. a SIMD chapter.

This is the fastest way to validate that the repository-wide rules are sound before rolling them across all remaining chapters.

## Open Execution Constraint

The design is repository-wide, but execution should still happen through explicit plans and checkpointed migration steps. The plan phase must name exact files, commands, and validation targets for the first implementation wave before any code migration begins.
