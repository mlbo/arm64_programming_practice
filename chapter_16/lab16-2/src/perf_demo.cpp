// chapter_16/lab16-2/src/perf_demo.cpp
// perf c2c 分析演示实现

#include "perf_demo.hpp"
#include <time.h>
#include <stdio.h>

namespace arm64lab {

PerfDemo::PerfDemo(u64 iterations)
    : iterations_(iterations)
    , data_bad_{}
    , data_good_{} {
}

auto PerfDemo::thread_increment_bad(void* arg) -> void* {
    auto* params = static_cast<ThreadArg*>(arg);
    auto* demo = params->demo;
    auto index = params->counter_index;

    u64* counter = nullptr;
    switch (index) {
        case 0: counter = &demo->data_bad_.counter1; break;
        case 1: counter = &demo->data_bad_.counter2; break;
        case 2: counter = &demo->data_bad_.counter3; break;
        case 3: counter = &demo->data_bad_.counter4; break;
    }

    for (u64 i = 0; i < demo->iterations_; ++i) {
        *counter += 1;
    }
    return nullptr;
}

auto PerfDemo::thread_increment_good(void* arg) -> void* {
    auto* params = static_cast<ThreadArg*>(arg);
    auto* demo = params->demo;
    auto index = params->counter_index;

    u64* counter = nullptr;
    switch (index) {
        case 0: counter = &demo->data_good_.counter1; break;
        case 1: counter = &demo->data_good_.counter2; break;
        case 2: counter = &demo->data_good_.counter3; break;
        case 3: counter = &demo->data_good_.counter4; break;
    }

    for (u64 i = 0; i < demo->iterations_; ++i) {
        *counter += 1;
    }
    return nullptr;
}

static auto get_time_ms() -> u64 {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

auto PerfDemo::run_bad() -> u64 {
    data_bad_ = {};

    pthread_t threads[4];
    ThreadArg args[4];

    auto start = get_time_ms();

    for (int i = 0; i < 4; ++i) {
        args[i] = {this, i};
        pthread_create(&threads[i], nullptr, &thread_increment_bad, &args[i]);
    }

    for (int i = 0; i < 4; ++i) {
        pthread_join(threads[i], nullptr);
    }

    return get_time_ms() - start;
}

auto PerfDemo::run_good() -> u64 {
    data_good_ = {};

    pthread_t threads[4];
    ThreadArg args[4];

    auto start = get_time_ms();

    for (int i = 0; i < 4; ++i) {
        args[i] = {this, i};
        pthread_create(&threads[i], nullptr, &thread_increment_good, &args[i]);
    }

    for (int i = 0; i < 4; ++i) {
        pthread_join(threads[i], nullptr);
    }

    return get_time_ms() - start;
}

auto PerfDemo::get_bad_counter(int index) const -> u64 {
    switch (index) {
        case 0: return data_bad_.counter1;
        case 1: return data_bad_.counter2;
        case 2: return data_bad_.counter3;
        case 3: return data_bad_.counter4;
        default: return 0;
    }
}

auto PerfDemo::get_good_counter(int index) const -> u64 {
    switch (index) {
        case 0: return data_good_.counter1;
        case 1: return data_good_.counter2;
        case 2: return data_good_.counter3;
        case 3: return data_good_.counter4;
        default: return 0;
    }
}

} // namespace arm64lab