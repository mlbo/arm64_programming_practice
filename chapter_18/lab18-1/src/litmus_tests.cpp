// chapter_18/lab18-1/src/litmus_tests.cpp
// Litmus 测试实现

#include "litmus_tests.hpp"
#include <stdio.h>

namespace arm64lab {

// ============================================================================
// Message Passing Test
// ============================================================================

MessagePassingTest::MessagePassingTest()
    : data_(0), flag_(0), result_flag_(0), result_data_(0) {
}

auto MessagePassingTest::thread0_func(void* arg) -> void* {
    auto* self = static_cast<MessagePassingTest*>(arg);
    // 写入数据，然后设置标志
    self->data_ = 55;
    // 没有屏障！可能被重排
    self->flag_ = 1;
    return nullptr;
}

auto MessagePassingTest::thread1_func(void* arg) -> void* {
    auto* self = static_cast<MessagePassingTest*>(arg);
    // 读取标志，然后读取数据
    self->result_flag_ = self->flag_;
    // 没有屏障！可能被重排
    self->result_data_ = self->data_;
    return nullptr;
}

auto MessagePassingTest::run_once() -> bool {
    // 重置状态
    data_ = 0;
    flag_ = 0;
    result_flag_ = 0;
    result_data_ = 0;

    // 创建线程
    pthread_create(&thread0_, nullptr, &thread0_func, this);
    pthread_create(&thread1_, nullptr, &thread1_func, this);

    // 等待完成
    pthread_join(thread0_, nullptr);
    pthread_join(thread1_, nullptr);

    // 检查是否观察到重排: flag=1 但 data!=55
    // 这在弱内存模型下可能发生
    return !(result_flag_ == 1 && result_data_ != 55);
}

auto MessagePassingTest::run(int iterations) -> LitmusResult {
    return run_litmus_test([this]() { return run_once(); }, iterations);
}

// ============================================================================
// Message Passing with Barrier Test
// ============================================================================

MessagePassingBarrierTest::MessagePassingBarrierTest()
    : data_(0), flag_(0), result_flag_(0), result_data_(0) {
}

auto MessagePassingBarrierTest::thread0_func(void* arg) -> void* {
    auto* self = static_cast<MessagePassingBarrierTest*>(arg);
    self->data_ = 55;
    dmb_st();  // 存储屏障
    self->flag_ = 1;
    return nullptr;
}

auto MessagePassingBarrierTest::thread1_func(void* arg) -> void* {
    auto* self = static_cast<MessagePassingBarrierTest*>(arg);
    self->result_flag_ = self->flag_;
    dmb_ld();  // 加载屏障
    self->result_data_ = self->data_;
    return nullptr;
}

auto MessagePassingBarrierTest::run_once() -> bool {
    data_ = 0;
    flag_ = 0;
    result_flag_ = 0;
    result_data_ = 0;

    pthread_create(&thread0_, nullptr, &thread0_func, this);
    pthread_create(&thread1_, nullptr, &thread1_func, this);

    pthread_join(thread0_, nullptr);
    pthread_join(thread1_, nullptr);

    return !(result_flag_ == 1 && result_data_ != 55);
}

auto MessagePassingBarrierTest::run(int iterations) -> LitmusResult {
    return run_litmus_test([this]() { return run_once(); }, iterations);
}

// ============================================================================
// Store Buffer Test
// ============================================================================

StoreBufferTest::StoreBufferTest()
    : x_(0), y_(0), r1_(0), r2_(0) {
}

auto StoreBufferTest::thread0_func(void* arg) -> void* {
    auto* self = static_cast<StoreBufferTest*>(arg);
    self->x_ = 1;
    self->r1_ = self->y_;
    return nullptr;
}

auto StoreBufferTest::thread1_func(void* arg) -> void* {
    auto* self = static_cast<StoreBufferTest*>(arg);
    self->y_ = 1;
    self->r2_ = self->x_;
    return nullptr;
}

auto StoreBufferTest::run_once() -> bool {
    x_ = 0;
    y_ = 0;
    r1_ = 0;
    r2_ = 0;

    pthread_create(&thread0_, nullptr, &thread0_func, this);
    pthread_create(&thread1_, nullptr, &thread1_func, this);

    pthread_join(thread0_, nullptr);
    pthread_join(thread1_, nullptr);

    // 检查是否观察到 SB 模式: r1=0 && r2=0
    // 这在弱内存模型下可能发生
    return !(r1_ == 0 && r2_ == 0);
}

auto StoreBufferTest::run(int iterations) -> LitmusResult {
    return run_litmus_test([this]() { return run_once(); }, iterations);
}

// ============================================================================
// Load Buffer Test
// ============================================================================

LoadBufferTest::LoadBufferTest()
    : x_(0), y_(0), r1_(0), r2_(0) {
}

auto LoadBufferTest::thread0_func(void* arg) -> void* {
    auto* self = static_cast<LoadBufferTest*>(arg);
    self->r1_ = self->y_;
    self->x_ = 1;
    return nullptr;
}

auto LoadBufferTest::thread1_func(void* arg) -> void* {
    auto* self = static_cast<LoadBufferTest*>(arg);
    self->r2_ = self->x_;
    self->y_ = 1;
    return nullptr;
}

auto LoadBufferTest::run_once() -> bool {
    x_ = 0;
    y_ = 0;
    r1_ = 0;
    r2_ = 0;

    pthread_create(&thread0_, nullptr, &thread0_func, this);
    pthread_create(&thread1_, nullptr, &thread1_func, this);

    pthread_join(thread0_, nullptr);
    pthread_join(thread1_, nullptr);

    // 检查是否观察到 LB 模式: r1=1 && r2=1
    return !(r1_ == 1 && r2_ == 1);
}

auto LoadBufferTest::run(int iterations) -> LitmusResult {
    return run_litmus_test([this]() { return run_once(); }, iterations);
}

} // namespace arm64lab