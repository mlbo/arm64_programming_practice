// chapter_20/lab20-1/include/atomic.hpp
// 原子操作实现
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
// 内存屏障
// ============================================================================

inline void dmb() {
    asm volatile("dmb ish" ::: "memory");
}

inline void dmb_st() {
    asm volatile("dmb ishst" ::: "memory");
}

inline void dmb_ld() {
    asm volatile("dmb ishld" ::: "memory");
}

// ============================================================================
// Atomic<T> 模板类
// ============================================================================

template<typename T>
class Atomic {
public:
    explicit Atomic(T val = 0) : value_(val) {}

    // 加载
    auto load(MemoryOrder order = MemoryOrder::SeqCst) const -> T {
        (void)order;
        T result;
        asm volatile("ldar %w0, [%1]"
                     : "=r"(result)
                     : "r"(&value_)
                     : "memory");
        return result;
    }

    // 存储
    void store(T val, MemoryOrder order = MemoryOrder::SeqCst) {
        (void)order;
        asm volatile("stlr %w0, [%1]"
                     :
                     : "r"(val), "r"(&value_)
                     : "memory");
    }

    // 交换
    auto exchange(T val, MemoryOrder = MemoryOrder::SeqCst) -> T {
        T old_val;
        u32 tmp;
        asm volatile("1: ldxr %w0, [%2]\n"
                     "   stxr %w1, %w3, [%2]\n"
                     "   cbnz %w1, 1b"
                     : "=&r"(old_val), "=&r"(tmp)
                     : "r"(&value_), "r"(val)
                     : "memory");
        return old_val;
    }

    // 比较并交换 (强版本)
    auto compare_exchange_strong(T& expected, T desired,
                                  MemoryOrder = MemoryOrder::SeqCst,
                                  MemoryOrder = MemoryOrder::SeqCst) -> bool {
        u32 success_flag;
        T old_val;

        asm volatile("1: ldxr %w0, [%2]\n"
                     "   cmp %w0, %w3\n"
                     "   b.ne 2f\n"
                     "   stxr %w1, %w4, [%2]\n"
                     "   cbnz %w1, 1b\n"
                     "   mov %w1, #1\n"
                     "   b 3f\n"
                     "2: mov %w1, #0\n"
                     "3:"
                     : "=&r"(old_val), "=&r"(success_flag)
                     : "r"(&value_), "r"(expected), "r"(desired)
                     : "memory", "cc");

        if (!success_flag) {
            expected = old_val;
        }
        return success_flag;
    }

    // 比较并交换 (弱版本)
    auto compare_exchange_weak(T& expected, T desired,
                                MemoryOrder = MemoryOrder::SeqCst,
                                MemoryOrder = MemoryOrder::SeqCst) -> bool {
        u32 success_flag;
        T old_val;

        // 弱版本可能虚假失败，不重试
        asm volatile("ldxr %w0, [%2]\n"
                     "   cmp %w0, %w3\n"
                     "   b.ne 1f\n"
                     "   stxr %w1, %w4, [%2]\n"
                     "   b 2f\n"
                     "1: mov %w1, #0\n"
                     "2:"
                     : "=&r"(old_val), "=&r"(success_flag)
                     : "r"(&value_), "r"(expected), "r"(desired)
                     : "memory", "cc");

        expected = old_val;
        return success_flag;
    }

    // 原子加法
    auto fetch_add(T delta, MemoryOrder = MemoryOrder::SeqCst) -> T {
        T old_val;
        T new_val;
        u32 tmp;

        asm volatile("1: ldxr %w0, [%3]\n"
                     "   add %w1, %w0, %w4\n"
                     "   stxr %w2, %w1, [%3]\n"
                     "   cbnz %w2, 1b"
                     : "=&r"(old_val), "=&r"(new_val), "=&r"(tmp)
                     : "r"(&value_), "r"(delta)
                     : "memory");

        return old_val;
    }

    // 原子减法
    auto fetch_sub(T delta, MemoryOrder order = MemoryOrder::SeqCst) -> T {
        return fetch_add(static_cast<T>(-static_cast<i64>(delta)), order);
    }

    // 运算符重载
    auto operator++() -> T { return fetch_add(1) + 1; }
    auto operator++(int) -> T { return fetch_add(1); }
    auto operator--() -> T { return fetch_sub(1) - 1; }
    auto operator--(int) -> T { return fetch_sub(1); }

    auto operator+=(T delta) -> T { return fetch_add(delta) + delta; }
    auto operator-=(T delta) -> T { return fetch_sub(delta) - delta; }

private:
    mutable T value_;
};

// ============================================================================
// 64 位原子操作特化
// ============================================================================

template<>
class Atomic<u64> {
public:
    explicit Atomic(u64 val = 0) : value_(val) {}

    auto load(MemoryOrder = MemoryOrder::SeqCst) const -> u64 {
        u64 result;
        asm volatile("ldar %0, [%1]"
                     : "=r"(result)
                     : "r"(&value_)
                     : "memory");
        return result;
    }

    void store(u64 val, MemoryOrder = MemoryOrder::SeqCst) {
        asm volatile("stlr %0, [%1]"
                     :
                     : "r"(val), "r"(&value_)
                     : "memory");
    }

    auto exchange(u64 val, MemoryOrder = MemoryOrder::SeqCst) -> u64 {
        u64 old_val;
        u32 tmp;
        asm volatile("1: ldxr %0, [%2]\n"
                     "   stxr %w1, %3, [%2]\n"
                     "   cbnz %w1, 1b"
                     : "=&r"(old_val), "=&r"(tmp)
                     : "r"(&value_), "r"(val)
                     : "memory");
        return old_val;
    }

    auto compare_exchange_strong(u64& expected, u64 desired,
                                  MemoryOrder = MemoryOrder::SeqCst,
                                  MemoryOrder = MemoryOrder::SeqCst) -> bool {
        u32 success_flag;
        u64 old_val;

        asm volatile("1: ldxr %0, [%2]\n"
                     "   cmp %0, %3\n"
                     "   b.ne 2f\n"
                     "   stxr %w1, %4, [%2]\n"
                     "   cbnz %w1, 1b\n"
                     "   mov %w1, #1\n"
                     "   b 3f\n"
                     "2: mov %w1, #0\n"
                     "3:"
                     : "=&r"(old_val), "=&r"(success_flag)
                     : "r"(&value_), "r"(expected), "r"(desired)
                     : "memory", "cc");

        if (!success_flag) {
            expected = old_val;
        }
        return success_flag;
    }

    auto fetch_add(u64 delta, MemoryOrder = MemoryOrder::SeqCst) -> u64 {
        u64 old_val;
        u64 new_val;
        u32 tmp;

        asm volatile("1: ldxr %0, [%3]\n"
                     "   add %1, %0, %4\n"
                     "   stxr %w2, %1, [%3]\n"
                     "   cbnz %w2, 1b"
                     : "=&r"(old_val), "=&r"(new_val), "=&r"(tmp)
                     : "r"(&value_), "r"(delta)
                     : "memory");

        return old_val;
    }

    auto fetch_sub(u64 delta, MemoryOrder order = MemoryOrder::SeqCst) -> u64 {
        return fetch_add(static_cast<u64>(-static_cast<i64>(delta)), order);
    }

    auto operator++() -> u64 { return fetch_add(1) + 1; }
    auto operator++(int) -> u64 { return fetch_add(1); }
    auto operator--() -> u64 { return fetch_sub(1) - 1; }
    auto operator--(int) -> u64 { return fetch_sub(1); }

private:
    mutable u64 value_;
};

} // namespace arm64lab