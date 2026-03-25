# BenOS 知识点讲解

本文档讲解BenOS涉及的核心知识点，帮助读者理解ARM64操作系统开发的基础概念。

## 1. ARM64异常级别 (Exception Levels)

ARM64架构定义了4个异常级别（EL0-EL3），每个级别有不同的权限和用途：

| 异常级别 | 名称 | 用途 |
|---------|------|------|
| EL0 | 用户模式 | 运行用户应用程序 |
| EL1 | 内核模式 | 运行操作系统内核 |
| EL2 | 虚拟化模式 | 运行虚拟机监控器（Hypervisor） |
| EL3 | 安全监控模式 | 运行安全监控程序（Secure Monitor） |

### 异常级别切换

BenOS启动时会从高异常级别切换到EL1：

```
启动流程:
EL3 (固件) -> EL2 (可选) -> EL1 (内核)
```

### 关键代码解析

在 `head.S` 中，BenOS检测当前异常级别并执行相应的切换：

```assembly
mrs x5, CurrentEL       // 读取当前异常级别
cmp x5, #CurrentEL_EL3
b.eq el3_entry          // 如果在EL3，跳转到EL3处理
b el2_entry             // 否则跳转到EL2处理
```

### EL3到EL2的切换

```assembly
el3_entry:
    ldr x0, =SCTLR_EL2_VALUE_MMU_DISABLED
    msr sctlr_el2, x0       // 配置EL2系统控制寄存器

    ldr x0, =HCR_HOST_NVHE_FLAGS
    msr hcr_el2, x0         // 配置Hypervisor配置寄存器

    ldr x0, =SCR_VALUE
    msr scr_el3, x0         // 配置安全配置寄存器

    ldr x0, =SPSR_EL2
    msr spsr_el3, x0        // 设置目标处理器状态

    adr x0, el2_entry
    msr elr_el3, x0         // 设置返回地址

    eret                    // 异常返回，切换到EL2
```

### EL2到EL1的切换

```assembly
el2_entry:
    // 配置EL1和EL2的系统寄存器
    ldr x0, =SCTLR_EL1_VALUE_MMU_DISABLED
    msr sctlr_el1, x0

    ldr x0, =SPSR_EL1
    msr spsr_el2, x0        // 设置目标状态为EL1

    adr x0, el1_entry
    msr elr_el2, x0         // 设置返回地址

    eret                    // 切换到EL1
```

## 2. UART编程

UART（Universal Asynchronous Receiver-Transmitter）是串口通信的基础。在BenOS中，UART用于内核的早期调试输出。

### 树莓派UART

树莓派4B使用PL011 UART或mini UART。BenOS主要使用PL011 UART。

### PL011 UART寄存器

| 寄存器 | 偏移 | 说明 |
|-------|------|------|
| UART_DR | 0x00 | 数据寄存器 |
| UART_FR | 0x18 | 标志寄存器 |
| UART_IBRD | 0x24 | 整数波特率除数 |
| UART_FBRD | 0x28 | 分数波特率除数 |
| UART_LCRH | 0x2C | 线控制寄存器 |
| UART_CR | 0x30 | 控制寄存器 |

### 发送字符

```c
void uart_send(unsigned int c) {
    // 等待发送缓冲区为空
    while (readl(PL011_UART_BASE + UART_FR) & UART_FR_TXFF)
        ;

    // 写入数据寄存器
    writel(c, PL011_UART_BASE + UART_DR);
}
```

### 接收字符

```c
unsigned char uart_recv(void) {
    // 等待接收缓冲区有数据
    while (readl(PL011_UART_BASE + UART_FR) & UART_FR_RXFE)
        ;

    // 读取数据寄存器
    return readl(PL011_UART_BASE + UART_DR);
}
```

## 3. 内存管理

BenOS实现了一个简单的页分配器，用于管理物理内存。

### 页分配器原理

页分配器将物理内存划分为固定大小的页（通常为4KB），并通过引用计数跟踪每页的使用状态。

### 核心数据结构

```c
#define NR_PAGES (TOTAL_MEMORY / PAGE_SIZE)

static unsigned short mem_map[NR_PAGES] = {0,};
```

`mem_map` 数组记录每页的引用计数：
- 0：页面空闲
- >0：页面被使用，值表示引用次数

### 分配页面

```c
unsigned long get_free_page(void) {
    int i;

    for (i = 0; i < NR_PAGES; i++) {
        if (mem_map[i] == 0) {          // 找到空闲页
            mem_map[i] = 1;              // 标记为已使用
            return LOW_MEMORY + i * PAGE_SIZE; // 返回物理地址
        }
    }
    return 0;  // 没有可用页面
}
```

### 释放页面

```c
void free_page(unsigned long p) {
    mem_map[(p - LOW_MEMORY) / PAGE_SIZE] = 0;  // 清除引用计数
}
```

### C++封装

BenOS使用C++类封装了页分配器：

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

} // namespace benos
```

## 4. 进程管理

进程是操作系统中程序执行的基本单位。BenOS实现了简单的进程创建和调度机制。

### 进程控制块 (PCB)

进程控制块是内核中最重要的数据结构之一，保存了进程的所有信息：

```c
struct task_struct {
    enum task_state state;      // 进程状态
    enum task_flags flags;      // 进程标志
    long count;                 // 时间片计数
    int priority;               // 优先级
    int pid;                    // 进程ID
    struct cpu_context cpu_context; // CPU上下文（寄存器）
};
```

### 进程状态

```
TASK_RUNNING        -> 运行中或就绪
TASK_INTERRUPTIBLE  -> 可被信号唤醒的睡眠
TASK_UNINTERRUPTIBLE -> 不可被信号唤醒的睡眠
TASK_ZOMBIE         -> 已终止但父进程未回收
TASK_STOPPED        -> 被暂停
```

### CPU上下文

`cpu_context` 结构保存了进程切换时需要保存的寄存器：

```c
struct cpu_context {
    unsigned long x19;
    unsigned long x20;
    unsigned long x21;
    // ...
    unsigned long x29;  // FP
    unsigned long sp;   // 栈指针
    unsigned long pc;   // 程序计数器
};
```

### 进程创建 (fork)

进程创建的核心步骤：

1. **分配内存**：为新进程分配一个页面用于内核栈
2. **分配PID**：在进程表中找到空位
3. **初始化上下文**：设置新进程的初始寄存器状态
4. **设置返回地址**：让新进程从 `ret_from_fork` 开始执行

```c
int do_fork(unsigned long clone_flags, unsigned long fn, unsigned long arg) {
    struct task_struct *p;
    int pid;

    // 1. 分配页面
    p = (struct task_struct *)get_free_page();
    if (!p)
        goto error;

    // 2. 分配PID
    pid = find_empty_task();
    if (pid < 0)
        goto error;

    // 3. 初始化上下文
    if (copy_thread(clone_flags, p, fn, arg))
        goto error;

    // 4. 设置进程属性
    p->state = TASK_RUNNING;
    p->pid = pid;
    g_task[pid] = p;

    return pid;

error:
    return -1;
}
```

### 进程切换 (context switch)

进程切换的核心是保存当前进程的上下文并恢复目标进程的上下文：

```assembly
// cpu_switch_to(prev, next)
.globl cpu_switch_to
cpu_switch_to:
    mov     x10, #THREAD_CPU_CONTEXT
    add     x8, x0, x10          // prev->cpu_context
    mov     x9, sp
    stp     x19, x20, [x8], #16  // 保存x19-x28
    stp     x21, x22, [x8], #16
    stp     x23, x24, [x8], #16
    stp     x25, x26, [x8], #16
    stp     x27, x28, [x8], #16
    stp     x29, x9, [x8], #16   // 保存FP和SP
    str     x30, [x8]            // 保存PC

    add     x8, x1, x10          // next->cpu_context
    ldp     x19, x20, [x8], #16  // 恢复x19-x28
    ldp     x21, x22, [x8], #16
    ldp     x23, x24, [x8], #16
    ldp     x25, x26, [x8], #16
    ldp     x27, x28, [x8], #16
    ldp     x29, x9, [x8], #16   // 恢复FP和SP
    mov     sp, x9
    ldr     x30, [x8]            // 恢复PC
    ret
```

## 5. 异常处理

异常处理是操作系统响应外部事件（如中断）和内部错误（如缺页）的机制。

### 异常向量表

ARM64使用向量表来定位异常处理程序。每个异常级别都有自己的向量表基地址寄存器（VBAR_EL1/2/3）。

```assembly
// 设置异常向量表
ldr     x5, =vectors
msr     vbar_el1, x5
isb
```

### 异常向量表结构

向量表包含16个入口，每条128字节：

```
Offset  Type                  Description
0x000   Synchronous EL1       同步异常（如系统调用）
0x080   IRQ EL1               IRQ中断
0x100   FIQ EL1               FIQ中断
0x180   Error EL1             SError（如外部中止）
0x200   Synchronous EL1_64    64位同步异常
0x280   IRQ EL1_64            64位IRQ
0x300   FIQ EL1_64            64位FIQ
0x380   Error EL1_64          64位SError
```

### 中断处理流程

1. 硬件自动保存PC、PSTATE到ELR_EL1和SPSR_EL1
2. 跳转到向量表中对应的入口
3. 软件保存通用寄存器
4. 执行中断处理程序
5. 恢复通用寄存器
6. 执行 `eret` 返回

## 6. 定时器中断

定时器中断是实现进程调度的关键机制。

### ARM64通用定时器

ARM64架构定义了每个CPU核心都有一个通用定时器，包括：

- 物理定时器（EL1物理时间）
- 虚拟定时器（EL1虚拟时间）
- EL2物理定时器
- EL3物理定时器

### 定时器初始化

```c
void timer_init(void) {
    unsigned int tmp;
    unsigned int interval = HZ;  // 中断间隔

    // 设置下一次定时器中断
    asm volatile("msr cntv_tval_el0, %0" : : "r" (interval));

    // 启用定时器
    tmp = 1;
    asm volatile("msr cntv_ctl_el0, %0" : : "r" (tmp));
}
```

### 定时器中断处理

```c
void timer_handler(void) {
    // 重置定时器
    unsigned int interval = HZ;
    asm volatile("msr cntv_tval_el0, %0" : : "r" (interval));

    // 触发进程调度
    schedule();
}
```

## 总结

BenOS展示了ARM64操作系统开发的核心概念：

1. **异常级别**：理解ARM64的权限分级机制
2. **UART编程**：实现早期内核调试输出
3. **内存管理**：简单的页分配器实现
4. **进程管理**：PCB设计和进程创建
5. **异常处理**：中断和异常的处理流程
6. **定时器**：实现时间片调度的基础

这些概念是理解现代操作系统（如Linux）的基础。