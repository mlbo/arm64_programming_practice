# GICv2 中断控制器 - 知识点讲解

## 1. GIC 架构概述

GIC (Generic Interrupt Controller) 是 ARM 的中断控制器架构。GICv2 是常用版本，支持最多 8 个 CPU 核心。

### GICv2 组成

```
         +------------------+
         |   Distributor    |  <-- 中断分发
         |   (GICD)         |
         +--------+---------+
                  |
    +-------------+-------------+
    |             |             |
+---+---+    +----+----+    +---+---+
| CPU 0  |    | CPU 1   |    | CPU N |
| GICC   |    | GICC    |    | GICC  |
+--------+    +---------+    +-------+
```

- **Distributor (GICD)**: 管理中断源，决定中断路由到哪个 CPU
- **CPU Interface (GICC)**: 每个 CPU 一个，向 CPU 呈现中断

## 2. 中断类型

| 类型 | ID 范围 | 说明 |
|-----|--------|-----|
| **SGI** (Software Generated Interrupt) | 0-15 | 软件生成，用于核间通信 |
| **PPI** (Private Peripheral Interrupt) | 16-31 | 私有外设中断，每个 CPU 独立 |
| **SPI** (Shared Peripheral Interrupt) | 32-1019 | 共享外设中断，可路由到任意 CPU |

### 常用中断 ID

```cpp
// SGI
#define SGI_IRQ_BASE    0

// PPI
#define PPI_IRQ_BASE    16
#define VIRTUAL_TIMER_IRQ   27  // 虚拟定时器
#define HYPERVISOR_TIMER_IRQ 26 // Hypervisor 定时器
#define SECURE_TIMER_IRQ    29  // 安全定时器
#define NONSECURE_TIMER_IRQ 30  // 非安全定时器 (常用)

// SPI
#define SPI_IRQ_BASE    32
#define SYSTEM_TIMER1_IRQ  65  // 树莓派系统定时器
#define UART_IRQ           89  // UART 中断
```

## 3. GIC Distributor 寄存器

| 寄存器 | 偏移 | 说明 |
|-------|-----|------|
| GICD_CTLR | 0x000 | 控制寄存器 |
| GICD_TYPER | 0x004 | 类型寄存器 (中断数量) |
| GICD_ISENABLER | 0x100 | 中断使能设置 |
| GICD_ICENABLER | 0x180 | 中断使能清除 |
| GICD_IPRIORITYR | 0x400 | 优先级设置 |
| GICD_ITARGETSR | 0x800 | 中断目标 CPU |
| GICD_ICFGR | 0xC00 | 中断配置 (边沿/电平) |

### GICD_CTLR 控制位

```
Bit 0: EnableGrp0  - 使能 Group 0 中断 (FIQ)
Bit 1: EnableGrp1  - 使能 Group 1 中断 (IRQ)
```

## 4. GIC CPU Interface 寄存器

| 寄存器 | 偏移 | 说明 |
|-------|-----|------|
| GICC_CTLR | 0x000 | 控制寄存器 |
| GICC_PMR | 0x004 | 优先级屏蔽 |
| GICC_BPR | 0x008 | 二进制点寄存器 |
| GICC_IAR | 0x00C | 中断确认寄存器 |
| GICC_EOIR | 0x010 | 中断结束寄存器 |

### GICC_IAR 读取流程

```cpp
// 读取 IAR 获取中断 ID
u32 iar = readl(GICC_BASE + GICC_IAR);
u32 irq_id = iar & 0x3FF;  // 低 10 位是中断 ID

if (irq_id == 1023) {
    // 无中断 (spurious interrupt)
}
```

### GICC_EOIR 写入流程

```cpp
// 处理完中断后，写入 EOI
writel(irq_id, GICC_BASE + GICC_EOIR);
```

## 5. 中断处理流程

```
外设产生中断
    │
    ▼
GIC Distributor 接收
    │
    ├─ 检查中断是否使能
    ├─ 检查优先级
    └─ 决定路由到哪个 CPU
    │
    ▼
GIC CPU Interface
    │
    ├─ 与 PMR 比较优先级
    └─ 向 CPU 发送 IRQ 信号
    │
    ▼
CPU 响应 IRQ
    │
    ├─ 读取 GICC_IAR 获取中断 ID
    ├─ 执行中断处理函数
    └─ 写入 GICC_EOIR 结束中断
```

## 6. 中断优先级

GICv2 支持 256 级优先级 (0-255):
- **0**: 最高优先级
- **255**: 最低优先级

### 优先级屏蔽 (PMR)

```cpp
// 设置优先级阈值，低于此值的中断不会被响应
writel(0xF0, GICC_BASE + GICC_PMR);  // 只响应优先级 >= 0xF0
```

## 7. 中断目标路由

通过 GICD_ITARGETSR 设置中断目标 CPU:

```cpp
// 设置 SPI 中断路由到 CPU 0
// 每个寄存器控制 4 个中断，每个中断 8 位
void set_irq_target(u32 irq_id, u8 cpu_mask) {
    u32 offset = GICD_ITARGETSR + (irq_id / 4) * 4;
    u32 shift = (irq_id % 4) * 8;
    u32 val = readl(GICD_BASE + offset);
    val &= ~(0xFF << shift);
    val |= (cpu_mask << shift);
    writel(val, GICD_BASE + offset);
}
```

## 8. 初始化顺序

```cpp
void gic_init() {
    // 1. 禁用 Distributor
    writel(0, GICD_BASE + GICD_CTLR);

    // 2. 设置中断路由和优先级
    // ...

    // 3. 使能 Distributor
    writel(1, GICD_BASE + GICD_CTLR);

    // 4. 初始化 CPU Interface
    writel(0xF0, GICC_BASE + GICC_PMR);  // 优先级屏蔽
    writel(1, GICC_BASE + GICC_CTLR);    // 使能
}
```