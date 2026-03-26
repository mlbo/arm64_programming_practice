// chapter_20/lab20-1/src/main.cpp
// 原子操作测试主程序

#include "atomic_test.hpp"
#include <stdio.h>

auto main() -> int {
    printf("=== ARM64 Atomic Operations Demo ===\n\n");

    arm64lab::AtomicTest test;

    // 基本操作测试
    printf("Running basic operation tests...\n\n");

    test.test_load_store();
    test.test_exchange();
    test.test_compare_exchange();
    test.test_fetch_add();

    // 多线程测试
    printf("Running multithreaded tests...\n\n");

    test.test_multithreaded_add(4, 100000);
    test.test_spinlock();

    // 打印结果
    test.print_results();

    printf("\n=== LDXR/STXR Explanation ===\n");
    printf("LDXR (Load Exclusive Register):\n");
    printf("  - 加载数据并标记独占监视器\n");
    printf("  - 监控该地址的写入\n\n");

    printf("STXR (Store Exclusive Register):\n");
    printf("  - 尝试独占存储\n");
    printf("  - 如果监视器仍有效则成功 (返回 0)\n");
    printf("  - 如果被其他写入打断则失败 (返回非 0)\n\n");

    printf("CAS (Compare and Swap) 实现:\n");
    printf("  1: ldxr w0, [addr]     // 独占加载\n");
    printf("     cmp w0, expected    // 比较\n");
    printf("     b.ne fail           // 不匹配则失败\n");
    printf("     stxr w1, new, [addr] // 独占存储\n");
    printf("     cbnz w1, 1b         // 重试如果失败\n");
    printf("     success!\n\n");

    return 0;
}