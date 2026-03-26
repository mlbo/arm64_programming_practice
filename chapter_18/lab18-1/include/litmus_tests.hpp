// chapter_18/lab18-1/include/litmus_tests.hpp
// Litmus 测试用例
// 参考 ncnn 项目风格

#pragma once

#include "memory_model.hpp"
#include <pthread.h>
#include <atomic>

namespace arm64lab {

// ============================================================================
// Message Passing (MP) 测试
// ============================================================================
// 经典的 Message Passing 场景
// 测试: 线程0 写入 data, 然后设置 flag
//       线程1 读取 flag, 然后读取 data
// 问题: 在弱内存模型下, 可能观察到 flag=1 但 data 仍为旧值

class MessagePassingTest {
public:
    MessagePassingTest();

    // 运行一次测试
    // 返回 true = 正确 (未观察到重排)
    // 返回 false = 观察到重排
    auto run_once() -> bool;

    // 运行多次测试
    auto run(int iterations) -> LitmusResult;

private:
    alignas(64) u64 data_;
    alignas(64) u64 flag_;
    alignas(64) u64 result_flag_;
    alignas(64) u64 result_data_;

    pthread_t thread0_;
    pthread_t thread1_;

    static auto thread0_func(void* arg) -> void*;
    static auto thread1_func(void* arg) -> void*;
};

// ============================================================================
// Message Passing with Barrier (MP+Barrier)
// ============================================================================

class MessagePassingBarrierTest {
public:
    MessagePassingBarrierTest();
    auto run_once() -> bool;
    auto run(int iterations) -> LitmusResult;

private:
    alignas(64) u64 data_;
    alignas(64) u64 flag_;
    alignas(64) u64 result_flag_;
    alignas(64) u64 result_data_;

    pthread_t thread0_;
    pthread_t thread1_;

    static auto thread0_func(void* arg) -> void*;
    static auto thread1_func(void* arg) -> void*;
};

// ============================================================================
// Store Buffer (SB) 测试
// ============================================================================
// 两个线程各自写入不同变量，然后读取对方的写入
// 测试: x=1; r1=y 和 y=1; r2=x 是否可能同时读到 0

class StoreBufferTest {
public:
    StoreBufferTest();
    auto run_once() -> bool;
    auto run(int iterations) -> LitmusResult;

private:
    alignas(64) u64 x_;
    alignas(64) u64 y_;
    alignas(64) u64 r1_;
    alignas(64) u64 r2_;

    pthread_t thread0_;
    pthread_t thread1_;

    static auto thread0_func(void* arg) -> void*;
    static auto thread1_func(void* arg) -> void*;
};

// ============================================================================
// Load Buffer (LB) 测试
// ============================================================================

class LoadBufferTest {
public:
    LoadBufferTest();
    auto run_once() -> bool;
    auto run(int iterations) -> LitmusResult;

private:
    alignas(64) u64 x_;
    alignas(64) u64 y_;
    alignas(64) u64 r1_;
    alignas(64) u64 r2_;

    pthread_t thread0_;
    pthread_t thread1_;

    static auto thread0_func(void* arg) -> void*;
    static auto thread1_func(void* arg) -> void*;
};

} // namespace arm64lab