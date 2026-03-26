// chapter_16/lab16-2/src/main.cpp
// perf c2c 分析演示主程序

#include "perf_demo.hpp"
#include <stdio.h>

auto main() -> int {
    printf("=== perf c2c False Sharing Analysis Demo ===\n\n");

    arm64lab::PerfDemo demo(100000000);

    printf("Test 1: With False Sharing (4 threads)\n");
    printf("Run 'perf c2c record ./perf_demo' to analyze\n\n");

    auto bad_time = demo.run_bad();
    printf("  Time: %llu ms\n", (unsigned long long)bad_time);
    printf("  Counters: %llu, %llu, %llu, %llu\n\n",
           (unsigned long long)demo.get_bad_counter(0),
           (unsigned long long)demo.get_bad_counter(1),
           (unsigned long long)demo.get_bad_counter(2),
           (unsigned long long)demo.get_bad_counter(3));

    printf("Test 2: Without False Sharing (cache line aligned)\n");

    auto good_time = demo.run_good();
    printf("  Time: %llu ms\n", (unsigned long long)good_time);
    printf("  Counters: %llu, %llu, %llu, %llu\n\n",
           (unsigned long long)demo.get_good_counter(0),
           (unsigned long long)demo.get_good_counter(1),
           (unsigned long long)demo.get_good_counter(2),
           (unsigned long long)demo.get_good_counter(3));

    printf("Speedup: %.2fx\n\n", (double)bad_time / good_time);

    printf("=== Analysis Tips ===\n");
    printf("Use the following commands to analyze false sharing:\n");
    printf("  $ sudo perf c2c record ./perf_demo\n");
    printf("  $ sudo perf c2c report --stats\n");
    printf("  $ sudo perf c2c report\n\n");

    return 0;
}