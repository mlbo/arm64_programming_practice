# 知识点: ARM64 裸机启动流程

## 1. 启动地址

树莓派启动后，GPU 会将内核镜像加载到内存地址 `0x80000` 并开始执行。

```ld
/* linker.ld */
SECTIONS
{
    . = 0x80000,
    .text.boot : { *(.text.boot) }
    /* ... */
}
```

## 2. 多核控制

ARM64 处理器通常有多个核心。通过读取 `MPIDR_EL1` 寄存器获取当前核心 ID:

```asm
mrs    x0, mpidr_el1
and    x0, x0, #0xFF     // 低 8 位是核心 ID
cbz    x0, master        // CPU0 执行主程序
b      proc_hang         // 其他核心挂起
```

## 3. BSS 段清零

C/C++ 程序要求 BSS 段（未初始化的全局变量）被清零:

```asm
adr    x0, bss_begin
adr    x1, bss_end
sub    x1, x1, x0
bl     memzero
```

## 4. UART 基地址

| 开发板 | 外设基地址 | UART 基地址 |
|--------|------------|-------------|
| Pi 3B  | 0x3F000000 | 0x3F201000  |
| Pi 4B  | 0xFE000000 | 0xFE201000  |

## 5. GPIO 配置

UART 需要使用 GPIO14 (TX) 和 GPIO15 (RX):

- GPIO14 占用 GPFSEL1 的 bit[14:12]
- GPIO15 占用 GPFSEL1 的 bit[17:15]
- 设置为 Alt0 模式 (值为 4)

```c
selector &= ~(7 << 12);  // 清除 GPIO14
selector |= 4 << 12;     // 设置 Alt0
```

## 6. 波特率计算

UART 时钟 48MHz，目标波特率 115200:

```
波特率分频 = UARTCLK / (16 * baud_rate)
         = 48 * 10^6 / (16 * 115200)
         = 26.0416666667

整数部分 = 26
小数部分 = (int)((0.0416666667 * 64) + 0.5) = 3
```

误差: |(115177 - 115200) / 115200 * 100| = 0.02%

## 7. MMIO 操作

内存映射 I/O 需要 volatile 访问和内存屏障:

```cpp
#define readl(c) ({ u32 __v = *(volatile u32*)(c); __iormb(); __v; })
#define writel(v,c) ({ u32 __v = v; __iowmb(); *(volatile u32*)(c) = __v; })
```

内存屏障 (`dmb()`) 确保读写顺序正确。