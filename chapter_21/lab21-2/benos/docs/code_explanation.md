# BenOS 代码说明

本文档详细解释BenOS的代码结构和关键函数实现。

## 启动流程

### 1. 汇编入口 (arch/arm64/kernel/head.S)

内核启动的第一步是汇编代码，完成以下任务：

```
启动流程:
_start -> 检查CPU ID -> EL检查和切换 -> 设置异常向量表 -> 清空BSS -> 设置栈 -> kernel_main
```

#### 入口点 `_start`

```assembly
.globl _start
_start:
    mrs x0, mpidr_el1
    and x0, x0, #0xFF        // 获取CPU ID
    cbz x0, master           // 主CPU继续执行
    b proc_hang               // 其他CPU挂起
```

- **多核处理**：BenOS目前只使用主CPU（CPU ID = 0），其他CPU进入无限循环
- **MPIDR_EL1**：多处理器亲和寄存器，包含CPU ID

#### 异常级别切换

从EL3/EL2切换到EL1的关键步骤：

```assembly
el2_entry:
    ldr x0, =SCTLR_EL1_VALUE_MMU_DISABLED
    msr sctlr_el1, x0        // 配置EL1系统控制

    ldr x0, =SPSR_EL1
    msr spsr_el2, x0         // 设置目标处理器状态

    adr x0, el1_entry
    msr elr_el2, x0          // 设置返回地址

    eret                      // 切换到EL1
```

- **SCTLR_EL1**：系统控制寄存器，控制MMU、缓存等
- **SPSR_EL2**：保存的程序状态寄存器，决定EL1的执行状态
- **ELR_EL2**：异常链接寄存器，存储eret后的跳转地址

#### 进入C++内核

```assembly
el1_entry:
    // 设置异常向量表
    ldr     x5, =vectors
    msr     vbar_el1, x5
    isb

    // 清空BSS段
    adr x0, bss_begin
    adr x1, bss_end
    sub x1, x1, x0
    bl memzero

    // 设置栈指针
    ldr x8, =sp_addr
    ldr x9, [x8]
    mov sp, x9

    // 调用内核主函数
    bl kernel_main
```

### 2. 内核主函数 (init/main.cpp)

`kernel_main()` 是C++代码的入口点，完成内核初始化：

```cpp
extern "C" void kernel_main(void) {
    int el;

    // 初始化UART
    g_uart.init();
    g_uart.send_string("=Welcome BenOS!\r\n");

    // 打印当前异常级别
    g_console.printf("running on EL:");
    el = read_sysreg(CurrentEL) >> 2;
    g_console.printf("%d\n", el);

    // 初始化内存管理
    g_page_allocator.init((u64)_ebss, TOTAL_MEMORY);
    print_memory_layout();

    // 初始化定时器
    g_timer.init();
    raw_local_irq_enable();

    // 创建内核线程
    int pid = g_process_manager.create_process(kernel_thread);
    if (pid < 0) {
        g_console.printf("create thread fail\n");
    }

    // 切换到新进程
    struct task_struct* next = g_process_manager.find_by_pid(pid);
    if (next) {
        switch_to(next);
    }

    // 主循环
    while (true) {
        g_uart.send(g_uart.recv());
    }
}
```

## 核心模块详解

### UART驱动

#### 接口定义 (include/benos/driver/uart.hpp)

```cpp
namespace benos {

class UartDriver {
public:
    void init();
    void send(char c);
    char recv();
    void send_string(const char* s);

    static auto instance() -> UartDriver&;

private:
    UartDriver() = default;
    bool initialized_ = false;
};

extern UartDriver& g_uart;

} // namespace benos
```

#### 实现 (lib/uart_wrapper.cpp)

```cpp
namespace benos {

void UartDriver::init() {
    if (initialized_) return;

    // 调用底层UART初始化
    uart_init();
    initialized_ = true;
}

void UartDriver::send(char c) {
    uart_send(c);
}

char UartDriver::recv() {
    return uart_recv();
}

void UartDriver::send_string(const char* s) {
    uart_send_string(const_cast<char*>(s));
}

UartDriver& g_uart = UartDriver::instance();

} // namespace benos
```

### 控制台模块

#### 接口定义 (include/benos/kernel/console.hpp)

```cpp
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
```

#### 实现 (lib/printk.cpp)

控制台的printf函数是对printk的封装：

```cpp
int Console::printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = ::vprintk(fmt, args);
    va_end(args);
    return ret;
}
```

### 页分配器

#### 接口定义 (include/benos/mm/page_allocator.hpp)

```cpp
namespace benos {

class PageAllocator {
public:
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
```

#### 实现 (mm/page_alloc.cpp)

```cpp
namespace benos {

void PageAllocator::init(uptr start, uptr end) {
    ::mem_init(start, end);
}

auto PageAllocator::alloc() -> void* {
    return reinterpret_cast<void*>(::get_free_page());
}

void PageAllocator::free(void* page) {
    ::free_page(reinterpret_cast<unsigned long>(page));
}

auto PageAllocator::free_pages() const -> usize {
    usize count = 0;
    for (usize i = 0; i < NR_PAGES; ++i) {
        if (mem_map[i] == 0) count++;
    }
    return count;
}

PageAllocator& g_page_allocator = PageAllocator::instance();

} // namespace benos
```

### 进程管理

#### 接口定义 (include/benos/kernel/process.hpp)

```cpp
namespace benos {

enum class ProcessState {
    Running = 0,
    Interruptible = 1,
    Uninterruptible = 2,
    Zombie = 3,
    Stopped = 4,
};

enum class ProcessFlags : unsigned long {
    KThread = 1 << 0,
};

class ProcessManager {
public:
    auto create_process(void (*fn)(), u64 arg = 0) -> i32;
    auto find_by_pid(i32 pid) -> task_struct*;

    static auto instance() -> ProcessManager&;

private:
    ProcessManager() = default;
};

extern ProcessManager& g_process_manager;

} // namespace benos
```

#### 实现 (kernel/process_manager.cpp)

```cpp
namespace benos {

auto ProcessManager::create_process(void (*fn)(), u64 arg) -> i32 {
    return do_fork(static_cast<unsigned long>(ProcessFlags::KThread),
                   reinterpret_cast<unsigned long>(fn),
                   static_cast<unsigned long>(arg));
}

auto ProcessManager::find_by_pid(i32 pid) -> task_struct* {
    if (pid < 0 || pid >= NR_TASK) {
        return nullptr;
    }
    return g_task[pid];
}

ProcessManager& g_process_manager = ProcessManager::instance();

} // namespace benos
```

### 进程创建 (kernel/fork.c)

#### 核心函数 `do_fork`

```c
int do_fork(unsigned long clone_flags, unsigned long fn, unsigned long arg)
{
    struct task_struct *p;
    int pid;

    // 分配内核栈和task_struct
    p = (struct task_struct *)get_free_page();
    if (!p)
        goto error;

    // 分配PID
    pid = find_empty_task();
    if (pid < 0)
        goto error;

    // 设置进程上下文
    if (copy_thread(clone_flags, p, fn, arg))
        goto error;

    // 设置进程状态
    p->state = TASK_RUNNING;
    p->pid = pid;
    g_task[pid] = p;

    return pid;

error:
    return -1;
}
```

#### 线程设置 `copy_thread`

```c
static int copy_thread(unsigned long clone_flags, struct task_struct *p,
        unsigned long fn, unsigned long arg)
{
    struct pt_regs *childregs;

    // 计算pt_regs在栈中的位置
    childregs = task_pt_regs(p);

    // 清空寄存器
    memset(childregs, 0, sizeof(struct pt_regs));
    memset(&p->cpu_context, 0, sizeof(struct cpu_context));

    if (clone_flags & PF_KTHREAD) {
        // 内核线程：设置入口函数
        childregs->pstate = PSR_MODE_EL1h;
        p->cpu_context.x19 = fn;   // 入口函数
        p->cpu_context.x20 = arg;  // 参数
    }

    // 设置返回地址（从ret_from_fork开始执行）
    p->cpu_context.pc = (unsigned long)ret_from_fork;
    p->cpu_context.sp = (unsigned long)childregs;

    return 0;
}
```

### 进程切换

#### C接口 (kernel/fork.c)

```c
void switch_to(struct task_struct *next)
{
    struct task_struct *prev = current;

    if (current == next)
        return;

    current = next;
    cpu_switch_to(prev, next);
}
```

#### 汇编实现 (arch/arm64/kernel/entry.S)

```assembly
.globl cpu_switch_to
cpu_switch_to:
    mov     x10, #THREAD_CPU_CONTEXT
    add     x8, x0, x10          // prev->cpu_context地址

    // 保存当前进程上下文
    stp     x19, x20, [x8], #16
    stp     x21, x22, [x8], #16
    stp     x23, x24, [x8], #16
    stp     x25, x26, [x8], #16
    stp     x27, x28, [x8], #16
    stp     x29, x9, [x8], #16   // x29=FP, x9=SP
    str     x30, [x8]            // x30=PC

    add     x8, x1, x10          // next->cpu_context地址

    // 恢复目标进程上下文
    ldp     x19, x20, [x8], #16
    ldp     x21, x22, [x8], #16
    ldp     x23, x24, [x8], #16
    ldp     x25, x26, [x8], #16
    ldp     x27, x28, [x8], #16
    ldp     x29, x9, [x8], #16
    mov     sp, x9
    ldr     x30, [x8]

    ret
```

## 构建系统

### Makefile结构

BenOS使用类似Linux内核的构建系统：

```
顶层Makefile
├── arch/arm64/Makefile      # 架构相关配置
├── scripts/Kbuild.include   # 构建系统辅助
├── init/Makefile            # 初始化代码
├── kernel/Makefile          # 内核核心
├── mm/Makefile              # 内存管理
└── lib/Makefile             # 库函数
```

### 关键编译选项

```makefile
# 交叉编译器
CROSS_COMPILE ?= aarch64-linux-gnu-

# 裸机编译选项
CFLAGS += -nostdlib -mgeneral-regs-only -ffreestanding -nostartfiles

# C++选项
CXXFLAGS := $(CFLAGS) -fno-exceptions -fno-rtti
```

- **-nostdlib**: 不链接标准库
- **-mgeneral-regs-only**: 不使用浮点/向量寄存器
- **-ffreestanding**: 裸机环境
- **-fno-exceptions**: 禁用C++异常
- **-fno-rtti**: 禁用运行时类型信息

## 调试技巧

### QEMU调试

```bash
# 启动QEMU调试模式
qemu-system-aarch64 -M raspi4b -kernel benos -nographic -S -s

# 连接GDB
aarch64-linux-gnu-gdb benos
(gdb) target remote :1234
(gdb) break kernel_main
(gdb) continue
```

### 常用GDB命令

```gdb
# 反汇编
disassemble kernel_main

# 查看寄存器
info registers

# 查看内存
x/10i $pc

# 单步执行
si      # 单指令
ni      # 单指令（跳过函数调用）
```

### 早期调试（在UART初始化之前）

在 `head.S` 中，可以在EL切换后立即输出调试信息：

```assembly
#ifdef CONFIG_DEBUG_ON_EARLY_ASM
    bl __init_uart
    bl print_el
#endif
```

## 总结

BenOS的代码结构清晰地展示了操作系统开发的核心概念：

1. **启动代码**：汇编完成硬件初始化和模式切换
2. **内核入口**：C++完成各子系统初始化
3. **驱动封装**：C++类封装底层硬件操作
4. **进程管理**：PCB设计和上下文切换
5. **构建系统**：模块化的Makefile结构

理解这些代码将帮助读者更好地学习Linux内核的实现。