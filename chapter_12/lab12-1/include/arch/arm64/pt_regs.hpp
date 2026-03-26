// include/arch/arm64/pt_regs.hpp
// ARM64 pt_regs 栈帧结构定义
// 参考 ncnn 项目风格

#pragma once

#include <stdint.h>
#include <type_traits>

namespace benos::arch::arm64 {

/// pt_regs 栈帧结构
///
/// 用于保存中断/异常现场的寄存器状态。
/// 必须与汇编 kernel_entry/kernel_exit 宏匹配。
///
/// 内存布局:
///   [sp+0x000] x0, x1
///   [sp+0x010] x2, x3
///   ...
///   [sp+0x0E0] x28, x29
///   [sp+0x0F0] x30 (LR), SP
///   [sp+0x100] PC (ELR_EL1), PSTATE (SPSR_EL1)
///
/// 总大小: 272 字节
struct PtRegs {
    // 通用寄存器 x0 ~ x30
    u64 x0, x1, x2, x3, x4, x5, x6, x7;
    u64 x8, x9, x10, x11, x12, x13, x14, x15;
    u64 x16, x17, x18, x19, x20, x21, x22, x23;
    u64 x24, x25, x26, x27, x28, x29, x30;

    // 特殊寄存器
    u64 sp;      // 栈指针
    u64 pc;      // 程序计数器 (ELR_EL1)
    u64 pstate;  // 处理器状态 (SPSR_EL1)
};

// 编译时检查
static_assert(sizeof(PtRegs) == 272, "PtRegs size must be 272 bytes");
static_assert(std::is_standard_layout_v<PtRegs>, "PtRegs must be standard layout");
static_assert(alignof(PtRegs) == 8, "PtRegs alignment must be 8 bytes");

/// 栈帧偏移量常量 (与汇编宏匹配)
namespace pt_regs_offset {
    constexpr u64 X0     = 0;
    constexpr u64 X1     = 8;
    constexpr u64 LR     = 240;  // x30
    constexpr u64 SP     = 248;
    constexpr u64 PC     = 256;
    constexpr u64 PSTATE = 264;
    constexpr u64 SIZE   = 272;
} // namespace pt_regs_offset

/// 异常原因类型
enum class BadMode : u32 {
    Sync  = 0,
    Irq   = 1,
    Fiq   = 2,
    Error = 3,
};

/// C 入口函数声明
extern "C" {
    /// IRQ 处理入口
    void do_irq(PtRegs* regs);

    /// 同步异常处理入口
    void do_sync_exception(PtRegs* regs, u32 esr);

    /// 无效异常处理
    void bad_mode(PtRegs* regs, int reason, u32 esr);
}

} // namespace benos::arch::arm64

// C 兼容类型别名
extern "C" {
    using pt_regs = benos::arch::arm64::PtRegs;
}