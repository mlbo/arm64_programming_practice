# EL 切换知识点

## 1. 关键寄存器

### ELR_ELx (Exception Link Register)

保存异常返回地址：

```asm
msr elr_el1, x0   ; 设置返回地址
```

### SPSR_ELx (Saved Program Status Register)

保存处理器状态：

```
Bits [3:0]:  M[3:0]  - 处理器模式 (EL0t, EL1t, EL1h, etc.)
Bit  [6]:    F       - FIQ 屏蔽
Bit  [7]:    I       - IRQ 屏蔽
Bit  [8]:    A       - SError 屏蔽
Bit  [9]:    D       - Debug 异常屏蔽
```

## 2. SPSR 值设置

```cpp
// EL0t 模式 (使用 SP_EL0)
constexpr u64 SPSR_EL0t = 0x0000;

// EL1t 模式 (使用 SP_EL0)
constexpr u64 SPSR_EL1t = 0x0004;

// EL1h 模式 (使用 SP_EL1)
constexpr u64 SPSR_EL1h = 0x0005;
```

## 3. 切换到 EL0

### 汇编实现

```asm
// 1. 设置用户栈
ldr x0, =user_stack_top
msr sp_el0, x0

// 2. 设置返回地址
ldr x0, =user_main
msr elr_el1, x0

// 3. 设置处理器状态
mov x0, #0          ; EL0t
msr spsr_el1, x0

// 4. 返回到 EL0
eret
```

### C++ 实现

```cpp
namespace benos::arch::arm64 {

void switch_to_el0(void (*entry)(), void* stack_top) {
    // 设置用户栈
    asm volatile("msr sp_el0, %0" :: "r" (stack_top));

    // 设置返回地址
    asm volatile("msr elr_el1, %0" :: "r" (entry));

    // 设置 SPSR (EL0t)
    u64 spsr = 0;  // EL0t 模式
    asm volatile("msr spsr_el1, %0" :: "r" (spsr));

    // 返回到 EL0
    asm volatile("eret");
}

}  // namespace benos::arch::arm64
```

## 4. 从更高 EL 下降

### EL3 → EL2

```asm
// 设置 EL2 的入口点
ldr x0, =el2_entry
msr elr_el3, x0

// 设置 SPSR (EL2h)
mov x0, #0x9       ; EL2h, 使用 SP_EL2
msr spsr_el3, x0

// 设置 SCR_EL3
mov x0, #(1 << 10)  ; RW=1, 下一级使用 AArch64
msr scr_el3, x0

eret
```

### EL2 → EL1

```asm
// 设置 EL1 的入口点
ldr x0, =el1_entry
msr elr_el2, x0

// 设置 SPSR (EL1h)
mov x0, #0x5       ; EL1h
msr spsr_el2, x0

eret
```

## 5. 用户态特性

在 EL0 时：
- 不能访问 EL1 的系统寄存器
- 只能访问受限的内存区域
- 使用 `svc` 指令进入内核

```asm
svc #0    ; 触发系统调用
```