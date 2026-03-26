# 中断处理与栈帧 - 知识点讲解

## 1. ARM64 异常处理概述

ARM64 架构定义了四种异常类型：

| 异常类型 | 说明 | 向量偏移 |
|---------|------|---------|
| Synchronous | 同步异常 (如 SVC, 数据中止) | 0x000 |
| IRQ | 普通中断请求 | 0x080 |
| FIQ | 快速中断请求 | 0x100 |
| SError | 系统错误 | 0x180 |

## 2. 异常向量表

异常向量表位于内存中，基地址由 `VBAR_EL1` 寄存器指定。每个向量表项占 128 字节，可以容纳最多 32 条指令。

向量表布局：
```
Offset  Vector
0x000   Current EL with SP0 - Synchronous
0x080   Current EL with SP0 - IRQ
0x100   Current EL with SP0 - FIQ
0x180   Current EL with SP0 - SError
0x200   Current EL with SPx - Synchronous
0x280   Current EL with SPx - IRQ       <-- 内核态 IRQ
0x300   Current EL with SPx - FIQ
0x380   Current EL with SPx - SError
...
```

## 3. pt_regs 栈帧结构

当异常发生时，需要保存处理器状态以便后续恢复。`pt_regs` 结构体定义了栈帧格式：

```cpp
struct PtRegs {
    u64 x0, x1, x2, x3, x4, x5, x6, x7;
    u64 x8, x9, x10, x11, x12, x13, x14, x15;
    u64 x16, x17, x18, x19, x20, x21, x22, x23;
    u64 x24, x25, x26, x27, x28, x29, x30;
    u64 sp;      // 栈指针
    u64 pc;      // 程序计数器 (从 ELR_EL1 恢复)
    u64 pstate;  // 处理器状态 (从 SPSR_EL1 恢复)
};
```

栈帧大小：31 × 8 + 8 + 8 + 8 = 272 字节

## 4. kernel_entry 宏详解

```asm
.macro kernel_entry
    sub sp, sp, #S_FRAME_SIZE    // 分配栈帧空间

    // 保存通用寄存器 x0~x29
    stp x0, x1, [sp, #16 * 0]
    stp x2, x3, [sp, #16 * 1]
    ...
    stp x28, x29, [sp, #16 * 14]

    // 保存特殊寄存器
    add x21, sp, #S_FRAME_SIZE   // x21 = 原始 SP
    mrs x22, elr_el1             // x22 = 返回地址
    mrs x23, spsr_el1            // x23 = 处理器状态

    stp lr, x21, [sp, #S_LR]     // 保存 LR 和 SP
    stp x22, x23, [sp, #S_PC]    // 保存 PC 和 PSTATE
.endm
```

## 5. kernel_exit 宏详解

```asm
.macro kernel_exit
    // 恢复特殊寄存器
    ldp x21, x22, [sp, #S_PC]    // 加载 ELR, SPSR
    msr elr_el1, x21             // 恢复返回地址
    msr spsr_el1, x22            // 恢复处理器状态

    // 恢复通用寄存器
    ldp x0, x1, [sp, #16 * 0]
    ...
    ldp x28, x29, [sp, #16 * 14]

    ldr lr, [sp, #S_LR]          // 恢复 LR
    add sp, sp, #S_FRAME_SIZE    // 释放栈帧
    eret                          // 异常返回
.endm
```

## 6. 关键寄存器

| 寄存器 | 说明 |
|-------|------|
| `ELR_EL1` | Exception Link Register，保存返回地址 |
| `SPSR_EL1` | Saved Program Status Register，保存处理器状态 |
| `ESR_EL1` | Exception Syndrome Register，异常原因 |
| `FAR_EL1` | Fault Address Register，故障地址 |
| `VBAR_EL1` | Vector Base Address Register，向量表基地址 |

## 7. 栈对齐要求

ARM64 要求栈指针 16 字节对齐。在异常处理中：
- `stp` 指令自动保证对齐
- 栈帧大小 272 字节是 16 的倍数

## 8. 异常返回

`eret` 指令实现异常返回：
1. 从 `ELR_EL1` 恢复 PC
2. 从 `SPSR_EL1` 恢复 PSTATE
3. 返回到异常发生前的指令继续执行