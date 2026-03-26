# 代码说明

## 异常向量表实现

### entry.S

```asm
#include "macros.S"

.macro inv_entry el, reason
    mov x0, sp
    mov x1, #\reason
    mrs x2, esr_el1
    b bad_mode
.endm

.macro vtentry label
    .align 7
    b \label
.endm

.align 11
.global vectors
vectors:
    // Current EL, SP_EL0
    vtentry el1_sync_invalid
    vtentry el1_irq_invalid
    vtentry el1_fiq_invalid
    vtentry el1_serror_invalid

    // Current EL, SP_ELx
    vtentry el1_sync_invalid
    vtentry el1_irq_invalid
    vtentry el1_fiq_invalid
    vtentry el1_serror_invalid

    // Lower EL, AArch64
    vtentry el0_sync_invalid
    vtentry el0_irq_invalid
    vtentry el0_fiq_invalid
    vtentry el0_serror_invalid

    // Lower EL, AArch32
    vtentry el0_sync_invalid
    vtentry el0_irq_invalid
    vtentry el0_fiq_invalid
    vtentry el0_serror_invalid

el1_sync_invalid:
    inv_entry 1, BAD_SYNC
el1_irq_invalid:
    inv_entry 1, BAD_IRQ
// ... 其他处理函数
```

### C++ 异常处理

```cpp
struct pt_regs {
    u64 x0, x1, x2, x3;
    u64 x4, x5, x6, x7;
    u64 x8, x9, x10, x11;
    u64 x12, x13, x14, x15;
    u64 x16, x17, x18, x19;
    u64 x20, x21, x22, x23;
    u64 x24, x25, x26, x27;
    u64 x28, x29, x30;
    u64 sp, pc, pstate;
};

extern "C" void bad_mode(pt_regs* regs, int reason, u32 esr) {
    const char* handler[] = {
        "Sync Abort",
        "IRQ",
        "FIQ",
        "SError"
    };

    u64 far;
    asm volatile("mrs %0, far_el1" : "=r" (far));

    printk("Bad mode for %s handler detected\n", handler[reason]);
    printk("  FAR: 0x%lx\n", far);
    printk("  ESR: 0x%x\n", esr);
    printk("  PC:  0x%lx\n", regs->pc);

    while (1) {
        asm volatile("wfi");
    }
}
```

## 初始化异常向量表

```cpp
void init_exception_vector() {
    u64 vbar = reinterpret_cast<u64>(&vectors);
    asm volatile("msr vbar_el1, %0" :: "r" (vbar));
}
```