# 中断处理与栈帧 - 代码说明

## 文件结构

### 1. include/arch/arm64/pt_regs.hpp

```cpp
#pragma once

#include <stdint.h>
#include <type_traits>

namespace benos::arch::arm64 {

// pt_regs 栈帧结构
// 必须与汇编 kernel_entry/kernel_exit 宏匹配
struct PtRegs {
    u64 x0, x1, x2, x3, x4, x5, x6, x7;
    u64 x8, x9, x10, x11, x12, x13, x14, x15;
    u64 x16, x17, x18, x19, x20, x21, x22, x23;
    u64 x24, x25, x26, x27, x28, x29, x30;
    u64 sp;
    u64 pc;
    u64 pstate;
};

// 编译时检查
static_assert(sizeof(PtRegs) == 272, "PtRegs size mismatch");
static_assert(std::is_standard_layout_v<PtRegs>, "PtRegs must be standard layout");

} // namespace benos::arch::arm64
```

**设计要点：**
- 使用 `static_assert` 确保结构体大小正确
- `std::is_standard_layout` 确保 C++ 与 C/汇编兼容
- 命名空间 `benos::arch::arm64` 遵循项目规范

### 2. src/entry.S

异常向量表和中断入口/出口宏。

**关键宏：**

```asm
// 栈帧偏移量定义
#define S_FRAME_SIZE 272
#define S_X0  0
#define S_LR  240    // x30
#define S_SP  248
#define S_PC  256
#define S_PSTATE 264

// 中断入口宏
.macro kernel_entry
    sub sp, sp, #S_FRAME_SIZE
    stp x0, x1, [sp, #16 * 0]
    // ... 保存所有寄存器 ...
.endm

// 中断出口宏
.macro kernel_exit
    ldp x21, x22, [sp, #S_PC]
    msr elr_el1, x21
    msr spsr_el1, x22
    // ... 恢复所有寄存器 ...
    eret
.endm
```

**向量表定义：**

```asm
.align 11
.global vectors
vectors:
    // Current EL with SP0 (无效)
    .align 7
    b el1_sync_invalid    // 0x000
    .align 7
    b el1_irq_invalid     // 0x080
    // ...

    // Current EL with SPx (有效)
    .align 7
    b el1_sync_invalid    // 0x200
    .align 7
    b el1_irq             // 0x280 <-- IRQ 处理入口
    // ...
```

### 3. 中断处理流程

```
IRQ 发生
    │
    ▼
硬件自动:
  - 保存 PSTATE 到 SPSR_EL1
  - 保存返回地址到 ELR_EL1
  - 切换到 EL1 (如果不在 EL1)
  - 跳转到向量表偏移 0x280
    │
    ▼
el1_irq (entry.S):
  - kernel_entry 宏保存寄存器
  - 调用 C 处理函数
  - kernel_exit 宏恢复寄存器
    │
    ▼
eret 返回
```

### 4. C++ 与汇编交互

C++ 声明：
```cpp
extern "C" void do_irq(PtRegs* regs);
```

汇编调用：
```asm
el1_irq:
    kernel_entry
    mov x0, sp          // x0 = PtRegs 指针
    bl do_irq           // 调用 C++ 处理函数
    kernel_exit
```

**注意事项：**
- 使用 `extern "C"` 避免 C++ 名称修饰
- 汇编中 `x0` 作为第一个参数传递
- `sp` 指向栈帧顶部

### 5. Makefile 修改

```makefile
# C++ 源文件
CXX = aarch64-linux-gnu-g++
CXXFLAGS = -g -Wall -nostdlib -ffreestanding -I include

# 编译 C++ 文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
```