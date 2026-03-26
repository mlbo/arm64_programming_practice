# chapter_18/lab18-1: 内存模型 Litmus 测试

## 概述

本实验演示 ARM64 弱内存模型的特性，使用 Litmus 测试验证内存重排行为。

## ARM64 内存模型

ARM64 采用弱内存模型 (Weak Memory Model)，允许特定的内存操作重排：

| 重排类型 | 是否允许 | 说明 |
|----------|----------|------|
| Load-Load | 允许 | 加载可能被重排 |
| Load-Store | 允许 | 加载和存储可能重排 |
| Store-Store | 允许 | 存储可能被重排 |
| Store-Load | 允许 | 存储和加载可能重排 |

## 内存屏障

ARM64 提供以下内存屏障指令：

- **dmb (Data Memory Barrier)**: 数据内存屏障
- **dsb (Data Synchronization Barrier)**: 数据同步屏障
- **isb (Instruction Synchronization Barrier)**: 指令同步屏障

### dmb 变体

- `dmb ish`: 内部共享域的完整屏障
- `dmb ishst`: 仅存储屏障
- `dmb ishld`: 仅加载屏障

## 编译与运行

```bash
make
./litmus_test
```

## 预期输出

```
=== ARM64 Memory Model Litmus Tests ===

1. Message Passing (MP) Test:
   运行次数: 10000
   观察到重排次数: 可能 > 0
   重排率: 取决于硬件

2. Message Passing with Barrier (MP+dmb) Test:
   运行次数: 10000
   观察到重排次数: 0
   重排率: 0.00%

...
```

## herdtools7 工具

使用 herdtools7 进行形式化验证：

```bash
# 安装
sudo apt install opam
opam init
opam install herdtools7

# 运行
herd7 -cat aarch64.cat example.litmus
```

## 相关章节

- 《ARM64体系结构：编程与实践》第 18 章
- 参考资料: [ARM Memory Model](https://developer.arm.com/documentation/)