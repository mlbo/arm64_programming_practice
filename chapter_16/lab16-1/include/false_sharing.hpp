// chapter_16/lab16-1/include/false_sharing.hpp
// False Sharing 演示
// 参考 ncnn 项目风格

#pragma once

#include "cache_aligned.hpp"
#include <pthread.h>

namespace arm64lab {

// 存在 False Sharing 的计数器
// x 和 y 可能在同一缓存行
struct CounterBad {
    u64 count1;
    u64 count2;
};

// 优化后的计数器
// 使用缓存行对齐避免 false sharing
struct CounterGood {
    alignas(CACHE_LINE_SIZE) u64 count1;
    alignas(CACHE_LINE_SIZE) u64 count2;
};

// False Sharing 测试类
class FalseSharingTest {
public:
    explicit FalseSharingTest(u64 iterations = 100000000);

    // 运行存在 false sharing 的测试
    auto test_bad() -> u64;

    // 运行优化后的测试
    auto test_good() -> u64;

    // 运行数组版本测试 (相邻元素)
    auto test_array_adjacent() -> u64;

    // 运行数组版本测试 (远距离元素)
    auto test_array_distant() -> u64;

private:
    u64 iterations_;

    // 测试数据
    CounterBad counter_bad_;
    CounterGood counter_good_;

public:
    u64 data_array_[100];

private:

    // 线程入口
    static auto thread_func_bad_count1(void* arg) -> void*;
    static auto thread_func_bad_count2(void* arg) -> void*;
    static auto thread_func_good_count1(void* arg) -> void*;
    static auto thread_func_good_count2(void* arg) -> void*;
    static auto thread_func_array(void* arg) -> void*;
};

// 计时工具
class Timer {
public:
    Timer();
    auto elapsed_ms() const -> u64;
    auto reset() -> void;

private:
    u64 start_sec_;
    u64 start_nsec_;
};

} // namespace arm64lab