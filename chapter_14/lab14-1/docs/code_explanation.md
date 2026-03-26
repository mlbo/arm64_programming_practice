# 定时器中断 - 代码说明

## 文件结构

### 1. include/driver/timer.hpp

```cpp
#pragma once

#include <stdint.h>

namespace benos {

/// 定时器驱动
class TimerDriver {
public:
    /// 初始化定时器
    void init();

    /// 处理定时器中断
    void handle_irq();

    /// 设置定时器间隔 (纳秒)
    void set_interval_ns(u64 ns);

    /// 设置定时器间隔 (ticks)
    void set_interval_ticks(u64 ticks);

    /// 获取定时器频率
    auto frequency() const -> u64 { return frequency_; }

    /// 获取当前计数
    static auto get_count() -> u64;

    /// 获取单例实例
    static auto instance() -> TimerDriver&;

    /// 默认间隔 (10ms)
    static constexpr u64 DEFAULT_INTERVAL_NS = 10000000ULL;

private:
    TimerDriver() = default;

    u64 frequency_{0};
    u64 interval_ticks_{0};
    u64 tick_count_{0};
};

/// 全局定时器实例
extern TimerDriver& g_timer;

} // namespace benos
```

### 2. src/driver/timer.cpp

#### 初始化实现

```cpp
void TimerDriver::init() {
    // 读取定时器频率
    asm volatile("mrs %0, cntfrq_el0" : "=r"(frequency_));

    // 设置默认间隔
    set_interval_ns(DEFAULT_INTERVAL_NS);

    // 使能 GIC 中断
    g_gic.enable_irq(GENERIC_TIMER_IRQ);

    // 使能本地定时器控制
    writel(CNT_PNS_IRQ, TIMER_CNTRL0);

    // 使能定时器
    asm volatile("msr cntp_ctl_el0, %0" :: "r"(1));
}
```

#### 设置间隔

```cpp
void TimerDriver::set_interval_ns(u64 ns) {
    // 纳秒转换为 ticks
    u64 ticks = (frequency_ * ns) / 1000000000ULL;
    set_interval_ticks(ticks);
}

void TimerDriver::set_interval_ticks(u64 ticks) {
    interval_ticks_ = ticks;
    asm volatile("msr cntp_tval_el0, %0" :: "r"(ticks));
}
```

#### 中断处理

```cpp
void TimerDriver::handle_irq() {
    // 重置定时器值
    asm volatile("msr cntp_tval_el0, %0" :: "r"(interval_ticks_));

    tick_count_++;
    printk("Core0 Timer interrupt received (tick: %lu)\r\n", tick_count_);
}
```

### 3. include/irq_manager.hpp

中断管理器，统一管理中断处理函数。

```cpp
#pragma once

#include <stdint.h>

namespace benos {

/// 中断处理函数类型
using IrqHandler = void (*)(void* data);

/// 中断管理器
class IrqManager {
public:
    /// 初始化
    void init();

    /// 注册中断处理函数
    void register_handler(u32 irq_id, IrqHandler handler, void* data);

    /// 处理中断
    void handle_irq(u32 irq_id);

    /// 获取单例实例
    static auto instance() -> IrqManager&;

    static constexpr u32 MAX_IRQ = 256;

private:
    IrqManager() = default;

    struct IrqEntry {
        IrqHandler handler{nullptr};
        void* data{nullptr};
    };

    IrqEntry entries_[MAX_IRQ];
};

extern IrqManager& g_irq_manager;

} // namespace benos
```

### 4. 中断处理流程

```cpp
// gicv2.cpp
void gic_handle_irq() {
    GicV2& gic = GicV2::instance();

    do {
        u32 irq_id = gic.acknowledge();

        if (irq_id >= 1020) {
            break;  // Spurious interrupt
        }

        IrqManager::instance().handle_irq(irq_id);
        gic.eoi(irq_id);

    } while (true);
}

// irq_manager.cpp
void IrqManager::handle_irq(u32 irq_id) {
    if (irq_id < MAX_IRQ && entries_[irq_id].handler) {
        entries_[irq_id].handler(entries_[irq_id].data);
    } else {
        printk("Unhandled IRQ: %u\n", irq_id);
    }
}

// kernel.cpp
void kernel_main() {
    uart_init();
    g_gic.init();
    g_timer.init();

    // 注册定时器中断处理函数
    g_irq_manager.register_handler(GENERIC_TIMER_IRQ,
        [](void*) { g_timer.handle_irq(); },
        nullptr);

    // 使能中断
    asm volatile("msr daifclr, #2");

    while (1) {
        asm volatile("wfi");
    }
}
```

### 5. 内联汇编读取定时器

```cpp
// 读取当前计数
auto TimerDriver::get_count() -> u64 {
    u64 count;
    asm volatile("mrs %0, cntpct_el0" : "=r"(count));
    return count;
}

// 微秒延时
void udelay(u64 us) {
    u64 freq;
    asm volatile("mrs %0, cntfrq_el0" : "=r"(freq));

    u64 ticks = (freq * us) / 1000000;
    u64 start;
    asm volatile("mrs %0, cntpct_el0" : "=r"(start));

    while (true) {
        u64 now;
        asm volatile("mrs %0, cntpct_el0" : "=r"(now));
        if ((now - start) >= ticks) break;
    }
}
```