# chapter_16/lab16-1: False Sharing 演示

## 概述

本实验演示多核处理器中的 **False Sharing** 现象及其对性能的影响。

## 什么是 False Sharing?

False Sharing 发生在多核系统中，当不同处理器核心访问同一缓存行的不同变量时。虽然这些变量在逻辑上独立，但它们共享同一缓存行 (通常 64 字节)，导致缓存一致性协议频繁传递缓存行所有权，严重影响性能。

## MESI 协议背景

现代处理器使用 MESI 协议维护缓存一致性：
- **M (Modified)**: 已修改，仅当前缓存拥有最新数据
- **E (Exclusive)**: 独占，仅当前缓存拥有该行
- **S (Shared)**: 共享，多个缓存可能拥有该行
- **I (Invalid)**: 无效

当多个核心写入同一缓存行的不同位置时，会导致缓存行在不同核心间频繁传递。

## 编译与运行

```bash
make
./false_sharing_demo
```

## 预期输出

```
=== False Sharing Demo ===

Struct-based test:
  With false sharing:    ~7000 ms
  Without false sharing: ~3000 ms
  Speedup: ~2.3x

Array-based test:
  Adjacent elements (false sharing): ~7000 ms
  Distant elements (no sharing):     ~3000 ms
  Speedup: ~2.3x

=== Test Complete ===
```

## 关键代码

### 问题代码 (存在 False Sharing)
```cpp
struct CounterBad {
    u64 count1;  // 可能在同一缓存行
    u64 count2;  // 导致 false sharing
};
```

### 优化代码 (缓存行对齐)
```cpp
struct CounterGood {
    alignas(64) u64 count1;  // 独占缓存行
    alignas(64) u64 count2;  // 独占缓存行
};
```

## 相关章节

- 本实验对应《ARM64体系结构：编程与实践》第 16 章
- 下一实验: lab16-2 - 使用 perf 工具分析 False Sharing