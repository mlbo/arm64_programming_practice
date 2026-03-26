# 异常向量表知识点

## 1. 异常向量表布局

ARM64 异常向量表包含 16 个入口点，每个 128 字节：

```
           +------------------+
0x000      | Current EL       |
           | SP_EL0           |
           | Sync | IRQ | FIQ | SError |
           +------------------+
0x200      | Current EL       |
           | SP_ELx           |
           | Sync | IRQ | FIQ | SError |
           +------------------+
0x400      | Lower EL         |
           | AArch64          |
           | Sync | IRQ | FIQ | SError |
           +------------------+
0x600      | Lower EL         |
           | AArch32          |
           | Sync | IRQ | FIQ | SError |
           +------------------+
```

## 2. 异常类型

| 类型 | 来源 |
|------|------|
| Synchronous | 系统调用、缺页、对齐错误 |
| IRQ | 普通中断 |
| FIQ | 快速中断 |
| SError | 系统错误（异步外部中止）|

## 3. 实现异常向量表

### 汇编实现

```asm
.align 11       ; 2048 字节对齐
.global vectors
vectors:
    // Current EL, SP_EL0
    .align 7
    b el1_sync_invalid
    .align 7
    b el1_irq_invalid
    .align 7
    b el1_fiq_invalid
    .align 7
    b el1_serror_invalid

    // Current EL, SP_ELx
    .align 7
    b el1_sync_invalid
    .align 7
    b el1_irq_invalid
    .align 7
    b el1_fiq_invalid
    .align 7
    b el1_serror_invalid

    // ... 其他表项
```

### C++ 结构体

```cpp
namespace benos::arch::arm64 {

// 异常向量表 (必须 2KB 对齐)
struct alignas(2048) ExceptionVector {
    u8 current_el_sp_el0[128 * 4];     // 4 个异常类型
    u8 current_el_sp_elx[128 * 4];
    u8 lower_el_aarch64[128 * 4];
    u8 lower_el_aarch32[128 * 4];
};

}  // namespace benos::arch::arm64
```

## 4. 异常处理函数

### 保存上下文

```asm
.macro kernel_entry
    sub sp, sp, #272    // 预留 pt_regs 空间
    stp x0, x1, [sp, #16]
    stp x2, x3, [sp, #32]
    // ... 保存所有寄存器
    mrs x0, elr_el1
    mrs x1, spsr_el1
    stp x0, x1, [sp, #256]
.endm
```

### 恢复上下文

```asm
.macro kernel_exit
    ldp x0, x1, [sp, #256]
    msr elr_el1, x0
    msr spsr_el1, x1
    // ... 恢复所有寄存器
    add sp, sp, #272
    eret
.endm
```

## 5. ESR_EL1 寄存器

异常症状寄存器 (Exception Syndrome Register)：

```
Bits [31:26]: EC (Exception Class)
Bits [25]:    IL (Instruction Length)
Bits [24:0]:  ISS (Instruction Specific Syndrome)
```

### 常见 EC 值

| EC | 含义 |
|----|------|
| 0x15 | SVC 指令执行 |
| 0x20 | 从 EL0 的指令中止 |
| 0x24 | 从 EL0 的数据中止 |
| 0x25 | 从 EL1 的数据中止 |