// chapter_18/lab18-1/include/memory_model.hpp
// 内存模型工具
// 参考 ncnn 项目风格

#pragma once

#include <stdint.h>
#include <stddef.h>

namespace arm64lab {

// 无符号整数类型
using u8   = uint8_t;
using u16  = uint16_t;
using u32  = uint32_t;
using u64  = uint64_t;
using usize = size_t;

// 有符号整数类型
using i8   = int8_t;
using i16  = int16_t;
using i32  = int32_t;
using i64  = int64_t;

// ============================================================================
// 内存屏障
// ============================================================================

// 数据内存屏障 (Data Memory Barrier)
// 确保 DMB 之前的内存访问在 DMB 之后的内存访问之前完成
inline void dmb() {
    asm volatile("dmb ish" ::: "memory");
}

// 数据内存屏障 - 仅存储
// 确保之前的存储在之后的存储之前完成
inline void dmb_st() {
    asm volatile("dmb ishst" ::: "memory");
}

// 数据内存屏障 - 仅加载
// 确保之前的加载在之后的加载之前完成
inline void dmb_ld() {
    asm volatile("dmb ishld" ::: "memory");
}

// 数据同步屏障 (Data Synchronization Barrier)
// 确保所有之前的内存访问完成
inline void dsb() {
    asm volatile("dsb ish" ::: "memory");
}

// 指令同步屏障 (Instruction Synchronization Barrier)
// 清除指令流水线
inline void isb() {
    asm volatile("isb" ::: "memory");
}

// ============================================================================
// 编译器屏障
// ============================================================================

// 防止编译器重排
inline void compiler_barrier() {
    asm volatile("" ::: "memory");
}

// ============================================================================
// 原子操作宏 (简化版)
// ============================================================================

// READ_ONCE: 防止编译器优化读取
template<typename T>
inline T read_once(const T& addr) {
    T val;
    asm volatile("ldr %w0, [%1]"
                 : "=r"(val)
                 : "r"(&addr)
                 : "memory");
    return val;
}

// WRITE_ONCE: 防止编译器优化写入
template<typename T>
inline void write_once(T& addr, T val) {
    asm volatile("str %w0, [%1]"
                 :
                 : "r"(val), "r"(&addr)
                 : "memory");
}

// ============================================================================
// 内存序
// ============================================================================

enum class MemoryOrder {
    Relaxed,    // 无顺序保证
    Acquire,    // 获取语义
    Release,    // 释放语义
    AcqRel,     // 获取-释放语义
    SeqCst,     // 顺序一致性
};

// ============================================================================
// Litmus 测试框架
// ============================================================================

// Litmus 测试结果
struct LitmusResult {
    int total_runs;
    int failures;  // 观察到重排的次数
    double failure_rate;
};

// 运行 litmus 测试的模板
template<typename TestFunc>
auto run_litmus_test(TestFunc func, int iterations) -> LitmusResult {
    int failures = 0;
    for (int i = 0; i < iterations; ++i) {
        if (!func()) {
            ++failures;
        }
    }
    return {iterations, failures, (double)failures / iterations * 100.0};
}

} // namespace arm64lab