// chapter_16/lab16-1/src/false_sharing.cpp
// False Sharing 演示实现

#include "false_sharing.hpp"
#include <time.h>
#include <stdio.h>

namespace arm64lab {

Timer::Timer() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    start_sec_ = ts.tv_sec;
    start_nsec_ = ts.tv_nsec;
}

auto Timer::elapsed_ms() const -> u64 {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    u64 end_sec = ts.tv_sec;
    u64 end_nsec = ts.tv_nsec;

    return (end_sec - start_sec_) * 1000 + (end_nsec - start_nsec_) / 1000000;
}

auto Timer::reset() -> void {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    start_sec_ = ts.tv_sec;
    start_nsec_ = ts.tv_nsec;
}

FalseSharingTest::FalseSharingTest(u64 iterations)
    : iterations_(iterations)
    , counter_bad_{}
    , counter_good_{}
    , data_array_{} {
}

auto FalseSharingTest::thread_func_bad_count1(void* arg) -> void* {
    auto* self = static_cast<FalseSharingTest*>(arg);
    for (u64 i = 0; i < self->iterations_; ++i) {
        self->counter_bad_.count1 += i;
    }
    return nullptr;
}

auto FalseSharingTest::thread_func_bad_count2(void* arg) -> void* {
    auto* self = static_cast<FalseSharingTest*>(arg);
    for (u64 i = 0; i < self->iterations_; ++i) {
        self->counter_bad_.count2 += i;
    }
    return nullptr;
}

auto FalseSharingTest::thread_func_good_count1(void* arg) -> void* {
    auto* self = static_cast<FalseSharingTest*>(arg);
    for (u64 i = 0; i < self->iterations_; ++i) {
        self->counter_good_.count1 += i;
    }
    return nullptr;
}

auto FalseSharingTest::thread_func_good_count2(void* arg) -> void* {
    auto* self = static_cast<FalseSharingTest*>(arg);
    for (u64 i = 0; i < self->iterations_; ++i) {
        self->counter_good_.count2 += i;
    }
    return nullptr;
}

// 数组访问线程参数
auto FalseSharingTest::test_bad() -> u64 {
    counter_bad_.count1 = 0;
    counter_bad_.count2 = 0;

    pthread_t thread1, thread2;
    Timer timer;

    pthread_create(&thread1, nullptr, &thread_func_bad_count1, this);
    pthread_create(&thread2, nullptr, &thread_func_bad_count2, this);
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);

    return timer.elapsed_ms();
}

auto FalseSharingTest::test_good() -> u64 {
    counter_good_.count1 = 0;
    counter_good_.count2 = 0;

    pthread_t thread1, thread2;
    Timer timer;

    pthread_create(&thread1, nullptr, &thread_func_good_count1, this);
    pthread_create(&thread2, nullptr, &thread_func_good_count2, this);
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);

    return timer.elapsed_ms();
}

// 数组访问线程参数
struct ArrayThreadArg {
    FalseSharingTest* test;
    int index;
    u64 iterations;
};

static auto array_thread_func(void* arg) -> void* {
    auto* params = static_cast<ArrayThreadArg*>(arg);
    for (u64 i = 0; i < params->iterations; ++i) {
        params->test->data_array_[params->index] += 1;
    }
    return nullptr;
}

auto FalseSharingTest::test_array_adjacent() -> u64 {
    for (int i = 0; i < 100; ++i) {
        data_array_[i] = 0;
    }

    pthread_t thread1, thread2;
    ArrayThreadArg arg1{this, 0, iterations_};
    ArrayThreadArg arg2{this, 1, iterations_};

    Timer timer;
    pthread_create(&thread1, nullptr, &array_thread_func, &arg1);
    pthread_create(&thread2, nullptr, &array_thread_func, &arg2);
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);

    return timer.elapsed_ms();
}

auto FalseSharingTest::test_array_distant() -> u64 {
    for (int i = 0; i < 100; ++i) {
        data_array_[i] = 0;
    }

    pthread_t thread1, thread2;
    ArrayThreadArg arg1{this, 0, iterations_};
    ArrayThreadArg arg2{this, 90, iterations_};

    Timer timer;
    pthread_create(&thread1, nullptr, &array_thread_func, &arg1);
    pthread_create(&thread2, nullptr, &array_thread_func, &arg2);
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);

    return timer.elapsed_ms();
}

} // namespace arm64lab