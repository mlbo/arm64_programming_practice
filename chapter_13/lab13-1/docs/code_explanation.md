# GICv2 中断控制器 - 代码说明

## 文件结构

### 1. include/driver/gicv2.hpp

GICv2 驱动类定义，采用单例模式。

```cpp
#pragma once

#include <stdint.h>

namespace benos {

/// GICv2 中断控制器驱动
class GicV2 {
public:
    /// 初始化 GIC
    auto init() -> int;

    /// 使能中断
    auto enable_irq(u32 irq_id) -> int;

    /// 禁用中断
    auto disable_irq(u32 irq_id) -> int;

    /// 确认中断，返回中断 ID
    auto acknowledge() -> u32;

    /// 结束中断 (EOI)
    void eoi(u32 irq_id);

    /// 获取单例实例
    static auto instance() -> GicV2&;

private:
    GicV2() = default;

    void init_distributor();
    void init_cpu_interface();

    // GICv2 基地址
    static constexpr u64 GICD_BASE = 0xFF841000;
    static constexpr u64 GICC_BASE = 0xFF842000;

    u32 irq_count_{0};
    bool initialized_{false};
};

/// 全局 GIC 实例引用
extern GicV2& g_gic;

} // namespace benos
```

### 2. src/driver/gicv2.cpp

GICv2 驱动实现。

#### 初始化流程

```cpp
auto GicV2::init() -> int {
    // 读取中断数量
    u32 typer = readl(GICD_BASE + GICD_TYPER);
    irq_count_ = ((typer & 0x1F) + 1) * 32;

    init_distributor();
    init_cpu_interface();

    initialized_ = true;
    return 0;
}
```

#### Distributor 初始化

```cpp
void GicV2::init_distributor() {
    // 1. 禁用中断分发
    writel(0, GICD_BASE + GICD_CTLR);

    // 2. 设置 SPI 中断路由到 CPU 0
    u32 cpu_mask = get_cpu_mask();
    for (u32 i = 32; i < irq_count_; i += 4) {
        writel(cpu_mask, GICD_BASE + GICD_ITARGETSR + i);
    }

    // 3. 设置中断优先级
    for (u32 i = 0; i < irq_count_; i += 4) {
        writel(0xA0A0A0A0, GICD_BASE + GICD_IPRIORITYR + i);
    }

    // 4. 禁用所有中断
    for (u32 i = 0; i < irq_count_; i += 32) {
        writel(0xFFFFFFFF, GICD_BASE + GICD_ICENABLER + i / 8);
    }

    // 5. 使能 SGI (0-15)
    writel(0xFFFF, GICD_BASE + GICD_ISENABLER);

    // 6. 使能中断分发
    writel(1, GICD_BASE + GICD_CTLR);
}
```

#### CPU Interface 初始化

```cpp
void GicV2::init_cpu_interface() {
    // 1. 设置优先级屏蔽
    writel(0xF0, GICC_BASE + GICC_PMR);

    // 2. 使能 CPU Interface
    writel(1, GICC_BASE + GICC_CTLR);
}
```

#### 中断使能/禁用

```cpp
auto GicV2::enable_irq(u32 irq_id) -> int {
    u32 mask = 1 << (irq_id % 32);
    writel(mask, GICD_BASE + GICD_ISENABLER + (irq_id / 32) * 4);
    return 0;
}

auto GicV2::disable_irq(u32 irq_id) -> int {
    u32 mask = 1 << (irq_id % 32);
    writel(mask, GICD_BASE + GICD_ICENABLER + (irq_id / 32) * 4);
    return 0;
}
```

#### 中断确认与 EOI

```cpp
auto GicV2::acknowledge() -> u32 {
    u32 iar = readl(GICC_BASE + GICC_IAR);
    return iar & 0x3FF;  // 返回中断 ID
}

void GicV2::eoi(u32 irq_id) {
    writel(irq_id, GICC_BASE + GICC_EOIR);
}
```

### 3. 中断处理流程

```cpp
// entry.S
el1_irq:
    kernel_entry
    bl gic_handle_irq
    kernel_exit

// gicv2.cpp
void gic_handle_irq() {
    GicV2& gic = GicV2::instance();

    u32 irq_id = gic.acknowledge();

    if (irq_id == GENERIC_TIMER_IRQ) {
        handle_timer_irq();
    }

    gic.eoi(irq_id);
}
```

### 4. 与 C 代码兼容

为了兼容现有 C 代码，提供 C 接口:

```cpp
extern "C" {
    void gicv2_mask_irq(int irq) {
        benos::g_gic.disable_irq(irq);
    }

    void gicv2_unmask_irq(int irq) {
        benos::g_gic.enable_irq(irq);
    }

    void gicv2_eoi_irq(int irq) {
        benos::g_gic.eoi(irq);
    }
}
```

### 5. 寄存器宏定义

```cpp
// Distributor 寄存器偏移
constexpr u64 GICD_CTLR      = 0x000;
constexpr u64 GICD_TYPER     = 0x004;
constexpr u64 GICD_ISENABLER = 0x100;
constexpr u64 GICD_ICENABLER = 0x180;
constexpr u64 GICD_IPRIORITYR = 0x400;
constexpr u64 GICD_ITARGETSR = 0x800;

// CPU Interface 寄存器偏移
constexpr u64 GICC_CTLR = 0x000;
constexpr u64 GICC_PMR  = 0x004;
constexpr u64 GICC_IAR  = 0x00C;
constexpr u64 GICC_EOIR = 0x010;
```