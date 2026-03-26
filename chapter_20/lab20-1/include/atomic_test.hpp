// chapter_20/lab20-1/include/atomic_test.hpp
// 原子操作测试

#pragma once

#include "atomic.hpp"
#include <pthread.h>

namespace arm64lab {

// ============================================================================
// 原子操作测试类
// ============================================================================

class AtomicTest {
public:
    AtomicTest();

    // 基本操作测试
    auto test_load_store() -> bool;
    auto test_exchange() -> bool;
    auto test_compare_exchange() -> bool;
    auto test_fetch_add() -> bool;

    // 多线程测试
    auto test_multithreaded_add(int num_threads, int iterations) -> bool;

    // CAS 自旋锁测试
    auto test_spinlock() -> bool;

    // 打印结果
    void print_results();

    // 公开计数器供测试使用
    Atomic<u32> counter_;
    Atomic<u64> counter64_;

    // 自旋锁
    Atomic<u32> lock_{0};

    // 测试结果
    bool load_store_ok_{false};
    bool exchange_ok_{false};
    bool cas_ok_{false};
    bool fetch_add_ok_{false};
    bool multithread_ok_{false};
    bool spinlock_ok_{false};
};

// ============================================================================
// 自旋锁实现
// ============================================================================

class SpinLock {
public:
    SpinLock() : locked_(0) {}

    void lock() {
        while (locked_.exchange(1) != 0) {
            // 自旋等待
            while (locked_.load() != 0) {
                // 可以添加 yield 或 pause 指令
            }
        }
    }

    void unlock() {
        locked_.store(0);
    }

private:
    Atomic<u32> locked_;
};

// RAII 锁守卫
class LockGuard {
public:
    explicit LockGuard(SpinLock& lock) : lock_(lock) { lock_.lock(); }
    ~LockGuard() { lock_.unlock(); }

private:
    SpinLock& lock_;
};

} // namespace arm64lab