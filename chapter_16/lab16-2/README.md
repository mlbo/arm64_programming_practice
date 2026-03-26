# chapter_16/lab16-2: perf c2c 分析 False Sharing

## 概述

本实验使用 Linux `perf c2c` 工具分析 False Sharing 现象。

## perf c2c 工具介绍

`perf c2c` (Cache-to-Cache) 是 Linux 性能分析工具，专门用于检测缓存行竞争和 False Sharing。

### 安装要求

- Linux 内核 4.0+
- perf 工具 (通常在 linux-tools 包中)
- root 权限

## 编译与运行

```bash
# 编译
make

# 普通运行
./perf_demo

# 使用 perf c2c 分析
sudo perf c2c record ./perf_demo
sudo perf c2c report
```

## perf c2c 报告解读

### 关键指标

| 指标 | 含义 |
|------|------|
| HITM | Hit Modified - 缓存命中已修改状态 |
| LLC Misses | Last Level Cache 缺失 |
| Store L1D Hit | L1 数据缓存存储命中 |
| Total Shared Cache Lines | 共享缓存行数量 |

### 典型 False Sharing 特征

- 高 HITM 计数
- 少量共享缓存行但高访问频率
- Store L1D Hit 较低

## 关键代码

### 问题代码
```cpp
struct SharedDataBad {
    u64 counter1;  // 偏移 0
    u64 counter2;  // 偏移 8
    u64 counter3;  // 偏移 16
    u64 counter4;  // 偏移 24
};  // 所有计数器在同一缓存行!
```

### 优化代码
```cpp
struct SharedDataGood {
    alignas(64) u64 counter1;
    alignas(64) u64 counter2;
    alignas(64) u64 counter3;
    alignas(64) u64 counter4;
};  // 每个计数器独占缓存行
```

## 相关章节

- 《ARM64体系结构：编程与实践》第 16 章
- 前一实验: lab16-1 - False Sharing 基础演示