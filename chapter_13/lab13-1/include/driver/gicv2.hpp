// include/driver/gicv2.hpp
// ARM GICv2 中断控制器驱动
// 参考 ncnn 项目风格

#pragma once

#include <stdint.h>

namespace benos {

/// GICv2 中断控制器驱动
///
/// 采用单例模式，全局唯一实例。
///
/// 使用示例:
/// ```cpp
/// GicV2& gic = GicV2::instance();
/// gic.init();
/// gic.enable_irq(GENERIC_TIMER_IRQ);
///
/// // 中断处理中
/// u32 irq_id = gic.acknowledge();
/// // ... 处理中断 ...
/// gic.eoi(irq_id);
/// ```
class GicV2 {
public:
    /// 初始化 GIC
    /// @return 0 表示成功，负数表示错误
    auto init() -> int;

    /// 使能中断
    /// @param irq_id 中断 ID (0-1019)
    /// @return 0 表示成功
    auto enable_irq(u32 irq_id) -> int;

    /// 禁用中断
    /// @param irq_id 中断 ID
    /// @return 0 表示成功
    auto disable_irq(u32 irq_id) -> int;

    /// 确认中断
    /// @return 中断 ID，1023 表示 spurious interrupt
    auto acknowledge() -> u32;

    /// 结束中断 (EOI)
    /// @param irq_id 中断 ID
    void eoi(u32 irq_id);

    /// 获取中断数量
    auto irq_count() const -> u32 { return irq_count_; }

    /// 获取单例实例
    static auto instance() -> GicV2&;

    // GICv2 基地址
    static constexpr u64 GICD_BASE = 0xFF841000;  ///< Distributor 基地址
    static constexpr u64 GICC_BASE = 0xFF842000;  ///< CPU Interface 基地址

private:
    GicV2() = default;
    GicV2(const GicV2&) = delete;
    GicV2& operator=(const GicV2&) = delete;

    void init_distributor();
    void init_cpu_interface();
    auto get_cpu_mask() -> u32;

    u32 irq_count_{0};
    bool initialized_{false};
};

/// 全局 GIC 实例引用
extern GicV2& g_gic;

} // namespace benos

// C 兼容接口
extern "C" {
    void gicv2_mask_irq(int irq);
    void gicv2_unmask_irq(int irq);
    void gicv2_eoi_irq(int irq);
    void gic_handle_irq(void);
}

// ============================================================================
// GIC 寄存器定义
// ============================================================================

namespace benos::gic {

// Distributor 寄存器偏移
constexpr u64 GICD_CTLR       = 0x000;  ///< 控制寄存器
constexpr u64 GICD_TYPER      = 0x004;  ///< 类型寄存器
constexpr u64 GICD_ISENABLER  = 0x100;  ///< 中断使能设置
constexpr u64 GICD_ICENABLER  = 0x180;  ///< 中断使能清除
constexpr u64 GICD_ISPENDR    = 0x200;  ///< 中断等待设置
constexpr u64 GICD_ICPENDR    = 0x280;  ///< 中断等待清除
constexpr u64 GICD_ISACTIVER  = 0x300;  ///< 中断激活设置
constexpr u64 GICD_ICACTIVER  = 0x380;  ///< 中断激活清除
constexpr u64 GICD_IPRIORITYR = 0x400;  ///< 优先级设置
constexpr u64 GICD_ITARGETSR  = 0x800;  ///< 中断目标 CPU
constexpr u64 GICD_ICFGR      = 0xC00;  ///< 中断配置
constexpr u64 GICD_SGIR       = 0xF00;  ///< 软件 generated 中断

// CPU Interface 寄存器偏移
constexpr u64 GICC_CTLR  = 0x000;  ///< 控制寄存器
constexpr u64 GICC_PMR   = 0x004;  ///< 优先级屏蔽
constexpr u64 GICC_BPR   = 0x008;  ///< 二进制点寄存器
constexpr u64 GICC_IAR   = 0x00C;  ///< 中断确认寄存器
constexpr u64 GICC_EOIR  = 0x010;  ///< 中断结束寄存器
constexpr u64 GICC_RPR   = 0x014;  ///< 运行优先级
constexpr u64 GICC_HPPIR = 0x018;  ///< 最高优先级中断

// 常量
constexpr u32 GICD_ENABLE = 0x1;
constexpr u32 GICC_ENABLE = 0x1;
constexpr u32 GICC_INT_PRI_THRESHOLD = 0xF0;
constexpr u32 GICC_IAR_INT_ID_MASK = 0x3FF;
constexpr u32 GICC_INT_SPURIOUS = 1023;

// 中断类型
constexpr u32 IRQ_TYPE_SGI = 0;  ///< SGI: 0-15
constexpr u32 IRQ_TYPE_PPI = 1;  ///< PPI: 16-31
constexpr u32 IRQ_TYPE_SPI = 2;  ///< SPI: 32+

// 常用中断 ID
constexpr u32 GENERIC_TIMER_IRQ = 30;     ///< 通用定时器中断
constexpr u32 SYSTEM_TIMER1_IRQ = 65;     ///< 系统定时器1
constexpr u32 UART_IRQ = 89;              ///< UART 中断

} // namespace benos::gic