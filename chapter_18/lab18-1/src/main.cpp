// chapter_18/lab18-1/src/main.cpp
// 内存模型 Litmus 测试主程序

#include "litmus_tests.hpp"
#include <stdio.h>

auto main() -> int {
    printf("=== ARM64 Memory Model Litmus Tests ===\n\n");

    const int iterations = 10000;

    // Message Passing 测试
    printf("1. Message Passing (MP) Test:\n");
    printf("   测试场景: 线程0写data后设置flag, 线程1读flag后读data\n");
    printf("   预期: 弱内存模型下可能观察到 flag=1 但 data!=55\n\n");

    arm64lab::MessagePassingTest mp_test;
    auto mp_result = mp_test.run(iterations);
    printf("   运行次数: %d\n", mp_result.total_runs);
    printf("   观察到重排次数: %d\n", mp_result.failures);
    printf("   重排率: %.2f%%\n\n", mp_result.failure_rate);

    // Message Passing with Barrier 测试
    printf("2. Message Passing with Barrier (MP+dmb) Test:\n");
    printf("   添加了 dmb 屏障\n");
    printf("   预期: 屏障应阻止重排\n\n");

    arm64lab::MessagePassingBarrierTest mp_barrier_test;
    auto mp_barrier_result = mp_barrier_test.run(iterations);
    printf("   运行次数: %d\n", mp_barrier_result.total_runs);
    printf("   观察到重排次数: %d\n", mp_barrier_result.failures);
    printf("   重排率: %.2f%%\n\n", mp_barrier_result.failure_rate);

    // Store Buffer 测试
    printf("3. Store Buffer (SB) Test:\n");
    printf("   测试场景: x=1;r1=y 和 y=1;r2=x\n");
    printf("   预期: 弱内存模型下可能 r1=0 && r2=0\n\n");

    arm64lab::StoreBufferTest sb_test;
    auto sb_result = sb_test.run(iterations);
    printf("   运行次数: %d\n", sb_result.total_runs);
    printf("   观察到重排次数: %d\n", sb_result.failures);
    printf("   重排率: %.2f%%\n\n", sb_result.failure_rate);

    // Load Buffer 测试
    printf("4. Load Buffer (LB) Test:\n");
    printf("   测试场景: r1=y;x=1 和 r2=x;y=1\n");
    printf("   预期: 弱内存模型下可能 r1=1 && r2=1\n\n");

    arm64lab::LoadBufferTest lb_test;
    auto lb_result = lb_test.run(iterations);
    printf("   运行次数: %d\n", lb_result.total_runs);
    printf("   观察到重排次数: %d\n", lb_result.failures);
    printf("   重排率: %.2f%%\n\n", lb_result.failure_rate);

    printf("=== 测试说明 ===\n");
    printf("- 这些测试展示了 ARM64 弱内存模型的行为\n");
    printf("- 重排率取决于具体的硬件实现\n");
    printf("- 可使用 herdtools7 进行形式化验证:\n");
    printf("  $ herd7 -cat aarch64.cat example.litmus\n\n");

    return 0;
}