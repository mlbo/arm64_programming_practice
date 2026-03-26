# BenOS 完整内核 C++ 重构实现计划

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 将 chapter_21 BenOS 完整内核重构为规范化的 C++ 项目，作为其他章节的参考模板。

**Architecture:** 采用分层架构：启动层(汇编) -> 运行时层 -> 驱动层 -> 内核层。使用 `namespace benos` 包裹所有代码。保持现有目录结构，渐进式添加 C++ 文件。

**Tech Stack:** C++17 (freestanding), aarch64-linux-gnu-g++, QEMU raspi4b

---

## 文件结构 (使用现有目录)

```
chapter_21/lab21-2/benos/
├── README.md                          # 项目说明
├── docs/
│   ├── knowledge.md                   # 知识点讲解
│   ├── code_explanation.md            # 代码说明
│   └── exercises.md                   # 练习题
├── scripts/
│   ├── build.sh                       # 编译脚本
│   └── run.sh                         # 运行脚本
├── include/benos/                     # 新增: C++ 头文件目录
│   ├── benos.hpp                      # 主头文件
│   ├── types.hpp                      # 类型定义
│   ├── error.hpp                      # 错误码
│   ├── new.hpp                        # placement new
│   ├── mm/
│   │   └── page_allocator.hpp         # 页分配器
│   ├── kernel/
│   │   ├── process.hpp                # 进程管理
│   │   └── console.hpp                # 控制台
│   └── driver/
│       ├── uart.hpp                   # UART 驱动
│       └── timer.hpp                  # 定时器驱动
├── init/
│   ├── main.cpp                       # 重构: 内核入口
│   └── Makefile                       # 更新: 添加 C++ 支持
├── mm/
│   ├── page_alloc.cpp                 # 重构: 页分配器
│   └── Makefile                       # 更新
├── kernel/
│   ├── fork.cpp                       # 重构: 进程管理
│   └── Makefile                       # 更新
├── lib/
│   ├── printk.cpp                     # 重构: 控制台
│   └── Makefile                       # 更新
├── arch/arm64/                        # 保留汇编文件
│   ├── kernel/
│   │   ├── head.S                     # 启动汇编 (保留)
│   │   ├── entry.S                    # 异常向量表 (保留)
│   │   └── ...
│   └── mach-rpi/
│       └── ...                        # 硬件驱动 (保留 C 文件)
├── Makefile                           # 更新: C++ 支持
└── linker.ld                          # 链接脚本
```

---

## Task 1: 基础头文件和类型定义

**Files:**
- Create: `include/benos/types.hpp`
- Create: `include/benos/error.hpp`
- Create: `include/benos/new.hpp`
- Create: `include/benos/benos.hpp`

- [ ] **Step 1: 创建 include/benos/ 目录**

```bash
mkdir -p include/benos/mm include/benos/kernel include/benos/driver
```

- [ ] **Step 2: 创建 types.hpp**

```cpp
// include/benos/types.hpp
#pragma once

#include <stdint.h>
#include <stddef.h>

namespace benos {

using u8   = uint8_t;
using u16  = uint16_t;
using u32  = uint32_t;
using u64  = uint64_t;
using usize = size_t;
using uptr = uintptr_t;

using i8   = int8_t;
using i16  = int16_t;
using i32  = int32_t;
using i64  = int64_t;
using isize = ptrdiff_t;
using iptr = intptr_t;

// Memory constants
constexpr usize PAGE_SHIFT = 12;
constexpr usize PAGE_SIZE  = 1UL << PAGE_SHIFT;
constexpr usize PAGE_MASK  = ~(PAGE_SIZE - 1);

constexpr auto page_align(uptr addr) -> uptr {
    return (addr + PAGE_SIZE - 1) & PAGE_MASK;
}

} // namespace benos
```

- [ ] **Step 3: 创建 error.hpp**

```cpp
// include/benos/error.hpp
#pragma once

#include "types.hpp"

namespace benos {

enum class Error : i32 {
    OK = 0,
    Unknown = -1,
    NoMemory = -2,
    InvalidParam = -3,
};

constexpr auto success(i32 result) -> bool { return result >= 0; }
constexpr auto failed(i32 result) -> bool { return result < 0; }

} // namespace benos
```

- [ ] **Step 4: 创建 new.hpp (placement new for bare metal)**

```cpp
// include/benos/new.hpp
#pragma once

#include "types.hpp"

// Placement new for bare metal environment
inline void* operator new(benos::usize, void* p) noexcept { return p; }
inline void* operator new[](benos::usize, void* p) noexcept { return p; }
inline void operator delete(void*, void*) noexcept {}
inline void operator delete[](void*, void*) noexcept {}
```

- [ ] **Step 5: 创建 benos.hpp (不包含 timer，后续添加)**

```cpp
// include/benos/benos.hpp
#pragma once

#include "types.hpp"
#include "error.hpp"
#include "new.hpp"

// Subsystems (添加时取消注释)
// #include "mm/page_allocator.hpp"
// #include "kernel/process.hpp"
// #include "kernel/console.hpp"
// #include "driver/uart.hpp"
// #include "driver/timer.hpp"
```

- [ ] **Step 6: 提交**

```bash
git add include/benos/
git commit -m "feat(benos): add core type definitions

- Add types.hpp with u8/u16/u32/u64 aliases
- Add error.hpp with Error enum class
- Add new.hpp with placement new for bare metal

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 2: UART 驱动类化

**Files:**
- Create: `include/benos/driver/uart.hpp`
- Create: `lib/uart_wrapper.cpp` (使用现有 lib/ 目录)

- [ ] **Step 1: 创建 uart.hpp**

```cpp
// include/benos/driver/uart.hpp
#pragma once

#include "benos/types.hpp"

namespace benos {

class UartDriver {
public:
    void init();
    void send(char c);
    auto recv() -> char;
    void send_string(const char* str);

    static auto instance() -> UartDriver&;

private:
    UartDriver() = default;
    bool initialized_ = false;
};

extern UartDriver& g_uart;

} // namespace benos

// C compatibility - 调用现有 C 实现
extern "C" {
    void uart_init(void);
    void uart_send(char c);
    char uart_recv(void);
    void uart_send_string(const char* str);
}
```

- [ ] **Step 2: 创建 uart_wrapper.cpp**

```cpp
// lib/uart_wrapper.cpp
#include <benos/driver/uart.hpp>

namespace benos {

void UartDriver::init() {
    if (initialized_) return;
    ::uart_init();  // 调用现有 C 实现
    initialized_ = true;
}

void UartDriver::send(char c) { ::uart_send(c); }
auto UartDriver::recv() -> char { return ::uart_recv(); }
void UartDriver::send_string(const char* str) { ::uart_send_string(str); }

auto UartDriver::instance() -> UartDriver& {
    static UartDriver uart;
    return uart;
}

UartDriver& g_uart = UartDriver::instance();

} // namespace benos
```

- [ ] **Step 3: 提交**

```bash
git add include/benos/driver/uart.hpp lib/uart_wrapper.cpp
git commit -m "feat(benos): add UartDriver class wrapper

- Wrap existing C UART functions in UartDriver class
- Use existing uart_init/uart_send/uart_recv C functions

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 3: Timer 驱动类化

**Files:**
- Create: `include/benos/driver/timer.hpp`
- Create: `lib/timer_wrapper.cpp`

- [ ] **Step 1: 创建 timer.hpp**

```cpp
// include/benos/driver/timer.hpp
#pragma once

#include "benos/types.hpp"

namespace benos {

class TimerDriver {
public:
    void init();
    void handle_irq();
    void reset();

    static auto instance() -> TimerDriver&;

private:
    TimerDriver() = default;
};

extern TimerDriver& g_timer;

} // namespace benos

// C compatibility
extern "C" {
    void timer_init(void);
    void timer_interrupt_handler(void);
}
```

- [ ] **Step 2: 创建 timer_wrapper.cpp**

```cpp
// lib/timer_wrapper.cpp
#include <benos/driver/timer.hpp>

namespace benos {

void TimerDriver::init() { ::timer_init(); }
void TimerDriver::handle_irq() { ::timer_interrupt_handler(); }
void TimerDriver::reset() { /* TODO */ }

auto TimerDriver::instance() -> TimerDriver& {
    static TimerDriver timer;
    return timer;
}

TimerDriver& g_timer = TimerDriver::instance();

} // namespace benos
```

- [ ] **Step 3: 提交**

```bash
git add include/benos/driver/timer.hpp lib/timer_wrapper.cpp
git commit -m "feat(benos): add TimerDriver class wrapper

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 4: 页分配器类化

**Files:**
- Create: `include/benos/mm/page_allocator.hpp`
- Modify: `mm/page_alloc.c` -> `mm/page_alloc.cpp` (保留原有逻辑)

- [ ] **Step 1: 创建 page_allocator.hpp**

```cpp
// include/benos/mm/page_allocator.hpp
#pragma once

#include "benos/types.hpp"

namespace benos {

class PageAllocator {
public:
    static constexpr usize PAGE_SIZE = 4096;

    void init(uptr start, uptr end);
    auto alloc() -> void*;
    void free(void* page);

    auto total_pages() const -> usize;
    auto free_pages() const -> usize;

    static auto instance() -> PageAllocator&;

private:
    PageAllocator() = default;
};

extern PageAllocator& g_page_allocator;

} // namespace benos

// C compatibility - 与现有接口一致
extern "C" {
    void mem_init(unsigned long start_mem, unsigned long end_mem);
    unsigned long get_free_page(void);
    void free_page(unsigned long page);
}
```

- [ ] **Step 2: 重命名并更新 page_alloc.c -> page_alloc.cpp**

保持原有实现逻辑，添加 C++ wrapper：

```cpp
// mm/page_alloc.cpp
#include <benos/mm/page_allocator.hpp>
#include <printk.h>
#include <page.h>

// 使用原有静态数组和逻辑
#define NR_PAGES (TOTAL_MEMORY / PAGE_SIZE)

static unsigned short mem_map[NR_PAGES] = {0,};
static unsigned long phy_start_address;

namespace benos {

void PageAllocator::init(uptr start, uptr end) {
    // 委托给 C 实现
    ::mem_init(start, end);
}

auto PageAllocator::alloc() -> void* {
    return reinterpret_cast<void*>(::get_free_page());
}

void PageAllocator::free(void* page) {
    ::free_page(reinterpret_cast<unsigned long>(page));
}

auto PageAllocator::total_pages() const -> usize {
    return NR_PAGES;
}

auto PageAllocator::free_pages() const -> usize {
    // 简化实现
    usize count = 0;
    for (usize i = 0; i < NR_PAGES; ++i) {
        if (mem_map[i] == 0) count++;
    }
    return count;
}

auto PageAllocator::instance() -> PageAllocator& {
    static PageAllocator allocator;
    return allocator;
}

PageAllocator& g_page_allocator = PageAllocator::instance();

} // namespace benos

// 保持原有 C 实现
extern "C" {

void mem_init(unsigned long start_mem, unsigned long end_mem) {
    unsigned long nr_free_pages = 0;
    unsigned long free;

    start_mem = PAGE_ALIGN(start_mem);
    phy_start_address = start_mem;
    end_mem &= PAGE_MASK;
    free = end_mem - start_mem;

    while (start_mem < end_mem) {
        nr_free_pages++;
        start_mem += PAGE_SIZE;
    }

    printk("Memory: %uKB available, %u free pages\n", free/1024, nr_free_pages);
}

unsigned long get_free_page(void) {
    int i;
    for (i = 0; i < NR_PAGES; i++) {
        if (mem_map[i] == 0) {
            mem_map[i] = 1;
            return LOW_MEMORY + i * PAGE_SIZE;
        }
    }
    return 0;
}

void free_page(unsigned long p) {
    mem_map[(p - LOW_MEMORY)/PAGE_SIZE] = 0;
}

} // extern "C"
```

- [ ] **Step 3: 提交**

```bash
git rm mm/page_alloc.c
git add include/benos/mm/page_allocator.hpp mm/page_alloc.cpp
git commit -m "feat(benos): add PageAllocator class

- Rename page_alloc.c to page_alloc.cpp
- Add PageAllocator wrapper class
- Keep original C implementation logic

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 5: 控制台类化

**Files:**
- Create: `include/benos/kernel/console.hpp`
- Modify: `lib/printk.c` -> `lib/printk.cpp`

- [ ] **Step 1: 创建 console.hpp**

```cpp
// include/benos/kernel/console.hpp
#pragma once

#include "benos/types.hpp"
#include <stdarg.h>

namespace benos {

class Console {
public:
    int printf(const char* fmt, ...);
    int vprintf(const char* fmt, va_list args);
    void putchar(char c);
    void puts(const char* s);

    static auto instance() -> Console&;

private:
    Console() = default;
};

extern Console& g_console;

} // namespace benos

// C compatibility
extern "C" {
    int printk(const char* fmt, ...);
    int vprintk(const char* fmt, va_list args);
}
```

- [ ] **Step 2: 重命名 printk.c -> printk.cpp**

```bash
mv lib/printk.c lib/printk.cpp
```

在文件开头添加 extern "C" 包装：

```cpp
// lib/printk.cpp
// ... 原有代码 ...
// 在文件末尾添加:

namespace benos {

int Console::printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = ::vprintk(fmt, args);
    va_end(args);
    return ret;
}

int Console::vprintf(const char* fmt, va_list args) {
    return ::vprintk(fmt, args);
}

void Console::putchar(char c) {
    // 使用 UART
    uart_send(c);
}

void Console::puts(const char* s) {
    uart_send_string(s);
}

auto Console::instance() -> Console& {
    static Console console;
    return console;
}

Console& g_console = Console::instance();

} // namespace benos
```

- [ ] **Step 3: 提交**

```bash
git rm lib/printk.c
git add include/benos/kernel/console.hpp lib/printk.cpp
git commit -m "feat(benos): add Console class

- Rename printk.c to printk.cpp
- Add Console wrapper class

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 6: 进程管理类化

**Files:**
- Create: `include/benos/kernel/process.hpp`
- Modify: `kernel/fork.c` -> `kernel/fork.cpp`

- [ ] **Step 1: 创建 process.hpp (使用现有 cpu_context)**

```cpp
// include/benos/kernel/process.hpp
#pragma once

#include "benos/types.hpp"

// 使用现有的 cpu_context 定义
extern "C" {
#include <asm/processor.h>  // struct cpu_context
}

namespace benos {

// Process states - 与原 enum task_state 一致
enum class ProcessState : i32 {
    Running = 0,
    Interruptible = 1,
    Uninterruptible = 2,
    Zombie = 3,
    Stopped = 4,
};

// Process flags
enum class ProcessFlags : u32 {
    KernelThread = 1 << 0,
};

// Process class - 包装 task_struct
class Process {
public:
    ProcessState state = ProcessState::Running;
    u32 flags = 0;
    i64 count = 0;
    i32 priority = 1;
    i32 pid = -1;
    cpu_context cpu_ctx = {};

    static auto current() -> Process&;
    static void set_current(Process* proc);
};

class ProcessManager {
public:
    static constexpr i32 MAX_PROCESSES = 128;

    auto init() -> void;
    auto create_process(void (*fn)(), u64 arg = 0) -> i32;
    auto find_by_pid(i32 pid) -> Process*;

    auto current_process() -> Process* { return current_; }

    static auto instance() -> ProcessManager&;

private:
    Process* processes_[MAX_PROCESSES] = {};
    Process* current_ = nullptr;
};

extern ProcessManager& g_process_manager;

} // namespace benos

// C compatibility
extern "C" {
    int do_fork(unsigned long clone_flags, unsigned long fn, unsigned long arg);
    void switch_to(struct task_struct* next);
}
```

- [ ] **Step 2: 重命名 fork.c -> fork.cpp**

```bash
mv kernel/fork.c kernel/fork.cpp
```

在文件末尾添加 C++ wrapper：

```cpp
// kernel/fork.cpp
// ... 原有代码 ...

namespace benos {

Process& Process::current() {
    return *reinterpret_cast<Process*>(current);
}

void Process::set_current(Process* proc) {
    current = reinterpret_cast<struct task_struct*>(proc);
}

auto ProcessManager::init() -> void {
    // 初始化使用现有的 init_task_union
    processes_[0] = reinterpret_cast<Process*>(&init_task_union.task);
    current_ = processes_[0];
}

auto ProcessManager::create_process(void (*fn)(), u64 arg) -> i32 {
    return ::do_fork(PF_KTHREAD, reinterpret_cast<unsigned long>(fn), arg);
}

auto ProcessManager::find_by_pid(i32 pid) -> Process* {
    if (pid >= 0 && pid < MAX_PROCESSES) {
        return reinterpret_cast<Process*>(g_task[pid]);
    }
    return nullptr;
}

auto ProcessManager::instance() -> ProcessManager& {
    static ProcessManager manager;
    return manager;
}

ProcessManager& g_process_manager = ProcessManager::instance();

} // namespace benos
```

- [ ] **Step 3: 提交**

```bash
git rm kernel/fork.c
git add include/benos/kernel/process.hpp kernel/fork.cpp
git commit -m "feat(benos): add Process and ProcessManager classes

- Rename fork.c to fork.cpp
- Use existing cpu_context from processor.h
- Wrap existing task_struct and do_fork

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 7: 更新 main.cpp

**Files:**
- Modify: `init/main.cpp`

- [ ] **Step 1: 更新 main.cpp 使用 C++ 类**

```cpp
// init/main.cpp
#include <benos/benos.hpp>
#include <benos/mm/page_allocator.hpp>
#include <benos/kernel/process.hpp>
#include <benos/kernel/console.hpp>
#include <benos/driver/uart.hpp>
#include <benos/driver/timer.hpp>

#include <asm/sysregs.h>
#include <asm/irq.h>
#include <sched.h>

extern "C" {
extern char _text_boot[], _etext_boot[];
extern char _text[], _etext[];
extern char _rodata[], _erodata[];
extern char _data[], _edata[];
extern char _bss[], _ebss[];

extern union task_union init_task_union;
}

using namespace benos;

static void print_memory_layout() {
    g_console.printf("BenOS image layout:\n");
    g_console.printf("  .text.boot: 0x%08lx - 0x%08lx\n",
            (u64)_text_boot, (u64)_etext_boot);
    g_console.printf("       .text: 0x%08lx - 0x%08lx\n",
            (u64)_text, (u64)_etext);
    g_console.printf("     .rodata: 0x%08lx - 0x%08lx\n",
            (u64)_rodata, (u64)_erodata);
    g_console.printf("       .data: 0x%08lx - 0x%08lx\n",
            (u64)_data, (u64)_edata);
    g_console.printf("        .bss: 0x%08lx - 0x%08lx\n",
            (u64)_bss, (u64)_ebss);
}

static void delay(int n) {
    while (n--) ;
}

extern "C" void kernel_thread(void) {
    while (true) {
        delay(10000000);
        g_console.printf("%s: 12345\n", __func__);
    }
}

register unsigned long current_stack_pointer asm("sp");

extern "C" void kernel_main(void) {
    int el;

    // Initialize UART
    g_uart.init();
    g_uart.send_string("=Welcome BenOS!\r\n");

    // Print EL
    g_console.printf("running on EL:");
    el = read_sysreg(CurrentEL) >> 2;
    g_console.printf("%d\n", el);

    // Initialize memory
    g_page_allocator.init((u64)_ebss, TOTAL_MEMORY);
    print_memory_layout();

    // Print process info
    g_console.printf("0 thread's task_struct: 0x%lx\n", &init_task_union.task);
    g_console.printf("SP: 0x%lx\n", current_stack_pointer);

    // Initialize timer and enable interrupts
    g_timer.init();
    raw_local_irq_enable();

    // Create kernel thread
    int pid = g_process_manager.create_process(kernel_thread);
    if (pid < 0) {
        g_console.printf("create thread fail\n");
    }

    // Switch to new process
    Process* next = g_process_manager.find_by_pid(pid);
    if (next) {
        switch_to(reinterpret_cast<struct task_struct*>(next));
    }

    // Main loop
    while (true) {
        g_uart.send(g_uart.recv());
    }
}
```

- [ ] **Step 2: 提交**

```bash
git add init/main.cpp
git commit -m "refactor(benos): rewrite kernel_main using C++ classes

- Use g_uart, g_console, g_page_allocator, g_process_manager
- Clean up code style

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 8: 更新 Makefile 支持 C++

**Files:**
- Modify: `scripts/Makefile.build`
- Modify: `init/Makefile`
- Modify: `mm/Makefile`
- Modify: `kernel/Makefile`
- Modify: `lib/Makefile`

- [ ] **Step 1: 更新 scripts/Makefile.build 添加 C++ 规则**

找到现有的 `.c.o` 规则，在其后添加：

```makefile
# C++ compilation
quiet_cmd_cpp_s_cpp = CPP     $(quiet_modtag) $@
cmd_cpp_s_cpp       = $(CXX) $(cpp_flags) -S -o $@ $<

$(obj)/%.s: $(src)/%.cpp FORCE
	$(call if_changed_dep,cpp_s_cpp)

quiet_cmd_cpp_o_cpp = CXX     $(quiet_modtag) $@
cmd_cpp_o_cpp       = $(CXX) $(cpp_flags) $(CXXFLAGS) -c -o $@ $<

$(obj)/%.o: $(src)/%.cpp FORCE
	$(call if_changed_dep,cpp_o_cpp)
```

- [ ] **Step 2: 更新子目录 Makefile**

```makefile
# init/Makefile
obj-y := main.o

# mm/Makefile
obj-y := page_alloc.o

# kernel/Makefile
obj-y := fork.o

# lib/Makefile
obj-y := printk.o uart_wrapper.o timer_wrapper.o string.o
```

- [ ] **Step 3: 提交**

```bash
git add scripts/Makefile.build init/Makefile mm/Makefile kernel/Makefile lib/Makefile
git commit -m "build(benos): add C++ compilation support

- Add C++ rules to Makefile.build
- Update subdirectory Makefiles for C++ sources

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 9: 添加构建脚本和文档

**Files:**
- Create: `scripts/build.sh`
- Create: `scripts/run.sh`
- Create: `README.md`
- Create: `docs/knowledge.md`
- Create: `docs/code_explanation.md`
- Create: `docs/exercises.md`

- [ ] **Step 1: 创建 build.sh**

```bash
#!/bin/bash
set -e

BENOS_DIR=$(dirname "$(dirname "$(readlink -f "$0")")")
CROSS_COMPILE=${CROSS_COMPILE:-aarch64-linux-gnu-}

log_info() { echo -e "\033[0;32m[INFO]\033[0m $1"; }

build() {
    log_info "Building BenOS..."
    cd "$BENOS_DIR"
    make clean 2>/dev/null || true
    make CROSS_COMPILE=$CROSS_COMPILE
    log_info "Build complete: benos.bin"
}

run() {
    [ ! -f "$BENOS_DIR/benos.bin" ] && build
    log_info "Running in QEMU..."
    qemu-system-aarch64 -M raspi4b -kernel "$BENOS_DIR/benos.bin" -nographic -serial mon:stdio
}

case "${1:-build}" in
    build) build ;;
    run) run ;;
    *) echo "Usage: $0 [build|run]" ;;
esac
```

- [ ] **Step 2: 创建 run.sh**

```bash
#!/bin/bash
exec "$(dirname "$0")/build.sh" run
```

- [ ] **Step 3: 创建文档** (参考设计文档中的模板)

- [ ] **Step 4: 提交**

```bash
chmod +x scripts/build.sh scripts/run.sh
git add scripts/ README.md docs/
git commit -m "docs(benos): add build scripts and documentation

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## Task 10: 验证

- [ ] **Step 1: 编译验证**

```bash
cd chapter_21/lab21-2/benos
./scripts/build.sh
```

Expected: 编译成功，生成 benos.bin

- [ ] **Step 2: 运行验证**

```bash
./scripts/run.sh
```

Expected: QEMU 启动，显示 "Welcome BenOS!" 和启动信息

- [ ] **Step 3: 最终提交**

```bash
git add -A
git commit -m "feat(benos): complete C++ refactor

BenOS is now fully refactored to C++:
- All code uses namespace benos
- Core classes: UartDriver, TimerDriver, PageAllocator, Console, Process, ProcessManager
- C compatibility layer maintained for assembly calls
- Complete documentation and build scripts

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>"
```

---

## 完成标准

- [ ] 所有代码使用 `namespace benos`
- [ ] 关键数据结构封装为类
- [ ] 保持 C 兼容层供汇编调用
- [ ] 使用现有目录结构 (init/, mm/, kernel/, lib/)
- [ ] 编译成功无警告
- [ ] QEMU 运行正常
- [ ] 文档完整
- [ ] 脚本可用