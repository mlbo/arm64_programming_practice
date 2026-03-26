// chapter_16/lab16-1/src/main.cpp
// False Sharing 演示程序入口

#include "false_sharing.hpp"
#include <stdio.h>

auto main() -> int {
    printf("=== False Sharing Demo ===\n\n");

    arm64lab::FalseSharingTest test(100000000);

    // 测试结构体版本 (存在 false sharing vs 优化后)
    printf("Struct-based test:\n");
    auto bad_time = test.test_bad();
    printf("  With false sharing:    %llu ms\n", (unsigned long long)bad_time);

    auto good_time = test.test_good();
    printf("  Without false sharing: %llu ms\n", (unsigned long long)good_time);
    printf("  Speedup: %.2fx\n\n", (double)bad_time / good_time);

    // 测试数组版本 (相邻元素 vs 远距离元素)
    printf("Array-based test:\n");
    auto adjacent_time = test.test_array_adjacent();
    printf("  Adjacent elements (false sharing): %llu ms\n", (unsigned long long)adjacent_time);

    auto distant_time = test.test_array_distant();
    printf("  Distant elements (no sharing):     %llu ms\n", (unsigned long long)distant_time);
    printf("  Speedup: %.2fx\n\n", (double)adjacent_time / distant_time);

    printf("=== Test Complete ===\n");
    return 0;
}