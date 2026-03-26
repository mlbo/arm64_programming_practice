// chapter_20/lab20-1/src/atomic_test.cpp
// 原子操作测试实现

#include "atomic_test.hpp"
#include <stdio.h>

namespace arm64lab {

AtomicTest::AtomicTest()
    : counter_(0)
    , counter64_(0) {
}

auto AtomicTest::test_load_store() -> bool {
    counter_.store(42);
    auto val = counter_.load();

    load_store_ok_ = (val == 42);
    return load_store_ok_;
}

auto AtomicTest::test_exchange() -> bool {
    counter_.store(10);
    auto old = counter_.exchange(20);
    auto current = counter_.load();

    exchange_ok_ = (old == 10 && current == 20);
    return exchange_ok_;
}

auto AtomicTest::test_compare_exchange() -> bool {
    counter_.store(100);

    // CAS 成功场景
    u32 expected = 100;
    bool success = counter_.compare_exchange_strong(expected, 200);
    auto val = counter_.load();

    if (!success || val != 200) {
        cas_ok_ = false;
        return false;
    }

    // CAS 失败场景
    expected = 150;  // 错误的期望值
    success = counter_.compare_exchange_strong(expected, 300);
    val = counter_.load();

    // 应该失败，expected 被更新为当前值
    cas_ok_ = (!success && val == 200 && expected == 200);
    return cas_ok_;
}

auto AtomicTest::test_fetch_add() -> bool {
    counter_.store(0);

    auto old1 = counter_.fetch_add(5);
    auto old2 = counter_.fetch_add(10);
    auto current = counter_.load();

    fetch_add_ok_ = (old1 == 0 && old2 == 5 && current == 15);
    return fetch_add_ok_;
}

// 多线程测试参数
struct ThreadArg {
    AtomicTest* test;
    int iterations;
};

static auto thread_add_func(void* arg) -> void* {
    auto* params = static_cast<ThreadArg*>(arg);
    for (int i = 0; i < params->iterations; ++i) {
        ++params->test->counter_;
    }
    return nullptr;
}

auto AtomicTest::test_multithreaded_add(int num_threads, int iterations) -> bool {
    counter_.store(0);

    pthread_t threads[8];
    ThreadArg args[8];

    for (int i = 0; i < num_threads; ++i) {
        args[i] = {this, iterations};
        pthread_create(&threads[i], nullptr, &thread_add_func, &args[i]);
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    auto expected = static_cast<u32>(num_threads * iterations);
    auto actual = counter_.load();

    multithread_ok_ = (actual == expected);
    return multithread_ok_;
}

auto AtomicTest::test_spinlock() -> bool {
    counter_.store(0);
    SpinLock lock;

    const int iterations = 10000;

    // 创建两个线程，每个线程在锁保护下递增计数器
    auto thread_func = [](void* arg) -> void* {
        auto* data = static_cast<void**>(arg);
        auto* test = static_cast<AtomicTest*>(data[0]);
        auto* lk = static_cast<SpinLock*>(data[1]);
        auto iters = *static_cast<int*>(data[2]);

        for (int i = 0; i < iters; ++i) {
            LockGuard guard(*lk);
            ++test->counter_;
        }
        return nullptr;
    };

    void* thread_data[3] = {this, &lock, const_cast<int*>(&iterations)};

    pthread_t t1, t2;
    pthread_create(&t1, nullptr, thread_func, thread_data);
    pthread_create(&t2, nullptr, thread_func, thread_data);
    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);

    spinlock_ok_ = (counter_.load() == 2 * iterations);
    return spinlock_ok_;
}

void AtomicTest::print_results() {
    printf("=== Atomic Operations Test Results ===\n\n");

    printf("1. Load/Store:      %s\n", load_store_ok_ ? "PASS" : "FAIL");
    printf("2. Exchange:        %s\n", exchange_ok_ ? "PASS" : "FAIL");
    printf("3. Compare-Exchange: %s\n", cas_ok_ ? "PASS" : "FAIL");
    printf("4. Fetch-Add:       %s\n", fetch_add_ok_ ? "PASS" : "FAIL");
    printf("5. Multithreaded:   %s\n", multithread_ok_ ? "PASS" : "FAIL");
    printf("6. Spinlock:        %s\n", spinlock_ok_ ? "PASS" : "FAIL");

    printf("\n=== Test Summary ===\n");
    int passed = load_store_ok_ + exchange_ok_ + cas_ok_ +
                 fetch_add_ok_ + multithread_ok_ + spinlock_ok_;
    printf("Passed: %d/6\n", passed);
}

} // namespace arm64lab