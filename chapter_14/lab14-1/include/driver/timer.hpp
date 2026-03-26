// include/driver/timer.hpp
// ARM Generic Timer 驱动
// 参考 ncnn 项目风格

#pragma once

#include <stdint.h>

namespace benos {

/// ARM Generic Timer 驱动
///
/// 使用 EL1 Physical Timer (CNTP_*_EL0)
///
/// 使用示例:
/// ```cpp
/// TimerDriver& timer = TimerDriver::instance();
/// timer.init();
/// timer.set_interval_ms(10);  // 10ms 间隔
/// ```
class TimerDriver {
public:
    /// 初始化定时器
    void init();

    /// 处理定时器中断
    void handle_irq();

    /// 设置定时器间隔 (纳秒)
    void set_interval_ns(u64 ns);

    /// 设置定时器间隔 (毫秒)
    void set_interval_ms(u64 ms) {
        set_interval_ns(ms * 1000000ULL);
    }

    /// 设置定时器间隔 (ticks)
    void set_interval_ticks(u64 ticks);

    /// 重置定时器 (重新加载间隔)
    void reset();

    /// 获取定时器频率 (Hz)
    auto frequency() const -> u64 { return frequency_; }

    /// 获取当前计数器值
    static auto get_count() -> u64;

    /// 获取经过的 tick 数
    auto tick_count() const -> u64 { return tick_count_; }

    /// 获取单例实例
    static auto instance() -> TimerDriver&;

    /// 默认间隔 (10ms = 250Hz)
    static constexpr u64 DEFAULT_INTERVAL_NS = 10000000ULL;
    static constexpr u64 DEFAULT_HZ = 250;

private:
    TimerDriver() = default;
    TimerDriver(const TimerDriver&) = delete;
    TimerDriver& operator=(const TimerDriver&) = delete;

    u64 frequency_{0};
    u64 interval_ticks_{0};
    u64 tick_count_{0};
};

/// 全局定时器实例引用
extern TimerDriver& g_timer;

} // namespace benos

// C 兼容接口
extern "C" {
    void timer_init(void);
    void handle_timer_irq(void);
}

// ============================================================================
// 定时器寄存器定义
// ============================================================================

namespace benos::timer {

// 树莓派本地定时器寄存器
constexpr u64 TIMER_CNTRL0 = 0xFF800040;  ///< 定时器控制
constexpr u64 TIMER_CLO    = 0xFF800004;  ///< 计数器低 32 位
constexpr u64 TIMER_CHI    = 0xFF800008;  ///< 计数器高 32 位
constexpr u64 TIMER_C1     = 0xFF800010;  ///< 比较器 1
constexpr u64 TIMER_CS     = 0xFF800000;  ///< 控制状态

// 控制位
constexpr u32 CNT_PNS_IRQ = (1 << 1);  ///< 使能非安全物理定时器中断

} // namespace benos::timer