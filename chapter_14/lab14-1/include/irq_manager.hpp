// include/irq_manager.hpp
// 中断管理器
// 参考 ncnn 项目风格

#pragma once

#include <stdint.h>

namespace benos {

/// 中断处理函数类型
/// @param data 用户数据指针
using IrqHandler = void (*)(void* data);

/// 中断管理器
///
/// 统一管理所有中断处理函数。
///
/// 使用示例:
/// ```cpp
/// IrqManager& mgr = IrqManager::instance();
/// mgr.register_handler(30, timer_handler, nullptr);
///
/// // 在中断处理中
/// mgr.handle_irq(irq_id);
/// ```
class IrqManager {
public:
    /// 初始化中断管理器
    void init();

    /// 注册中断处理函数
    /// @param irq_id 中断 ID
    /// @param handler 处理函数
    /// @param data 用户数据
    void register_handler(u32 irq_id, IrqHandler handler, void* data);

    /// 注销中断处理函数
    void unregister_handler(u32 irq_id);

    /// 处理中断
    void handle_irq(u32 irq_id);

    /// 获取单例实例
    static auto instance() -> IrqManager&;

    static constexpr u32 MAX_IRQ = 256;

private:
    IrqManager() = default;
    IrqManager(const IrqManager&) = delete;
    IrqManager& operator=(const IrqManager&) = delete;

    struct IrqEntry {
        IrqHandler handler{nullptr};
        void* data{nullptr};
    };

    IrqEntry entries_[MAX_IRQ];
};

/// 全局中断管理器实例
extern IrqManager& g_irq_manager;

} // namespace benos

// C 兼容接口
extern "C" {
    void irq_manager_init(void);
    void irq_register_handler(u32 irq_id, void (*handler)(void*), void* data);
}