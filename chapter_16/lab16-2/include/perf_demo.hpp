// chapter_16/lab16-2/include/perf_demo.hpp
// perf c2c 分析演示
// 参考 ncnn 项目风格

#pragma once

#include "cache_aligned.hpp"
#include <pthread.h>

namespace arm64lab {

// 存在 False Sharing 的数据结构
struct SharedDataBad {
    u64 counter1;
    u64 counter2;
    u64 counter3;
    u64 counter4;
};

// 优化的数据结构
struct SharedDataGood {
    alignas(CACHE_LINE_SIZE) u64 counter1;
    alignas(CACHE_LINE_SIZE) u64 counter2;
    alignas(CACHE_LINE_SIZE) u64 counter3;
    alignas(CACHE_LINE_SIZE) u64 counter4;
};

// perf 分析测试
class PerfDemo {
public:
    explicit PerfDemo(u64 iterations = 100000000);

    // 运行存在 false sharing 的测试
    auto run_bad() -> u64;

    // 运行优化后的测试
    auto run_good() -> u64;

    // 获取计数器值 (用于验证)
    auto get_bad_counter(int index) const -> u64;
    auto get_good_counter(int index) const -> u64;

private:
    u64 iterations_;
    SharedDataBad data_bad_;
    SharedDataGood data_good_;

    static auto thread_increment_bad(void* arg) -> void*;
    static auto thread_increment_good(void* arg) -> void*;
};

// 线程参数
struct ThreadArg {
    PerfDemo* demo;
    int counter_index;
};

} // namespace arm64lab