# 练习题

## 基础题

### 1. 概念理解

**问题**: 什么是 False Sharing？它为什么会影响性能？

**提示**: 考虑缓存行、MESI 协议、缓存一致性流量。

---

### 2. 代码分析

**问题**: 以下代码是否存在 False Sharing？如何修复？

```cpp
struct SharedData {
    int a;
    int b;
    int c;
    int d;
};
```

---

### 3. 缓存行大小

**问题**: 为什么缓存行对齐使用 64 字节？如何查看当前系统的缓存行大小？

**提示**: 使用 `getconf LEVEL1_DCACHE_LINESIZE` 或查看 `/sys/devices/system/cpu/cpu0/cache/` 目录。

---

## 进阶题

### 4. 多线程计数器

**问题**: 设计一个多线程计数器，每个线程独立递增自己的计数器，最后汇总结果。要求避免 False Sharing。

**框架代码**:
```cpp
class ThreadSafeCounter {
    // 实现你的设计
public:
    void increment(int thread_id);
    u64 get_total() const;
};
```

---

### 5. 性能测量

**问题**: 修改本实验代码，测试不同迭代次数下的性能差异：
- 10,000,000 次
- 100,000,000 次
- 1,000,000,000 次

记录并分析结果。

---

### 6. 数组访问模式

**问题**: 在数组测试中，为什么选择索引 0 和 90 进行对比？
如果选择索引 0 和 8，结果会如何？

**提示**: 计算缓存行边界。

---

## 挑战题

### 7. 模板泛化

**问题**: 实现 `CacheLineAligned<T>` 模板，支持：
- 基本类型 (int, long, etc.)
- 自定义结构体
- 数组类型

```cpp
template<typename T>
struct alignas(64) CacheLineAligned {
    // 实现你的设计
};
```

---

### 8. 使用 perf 分析

**问题**: 使用 `perf c2c record` 和 `perf c2c report` 分析本实验程序，找出发生 False Sharing 的缓存行地址。

**步骤**:
```bash
perf c2c record ./false_sharing_demo
perf c2c report
```

解释报告中的关键指标：
- HITM
- LLC Misses
- Store L1D Hit

---

### 9. NUMA 感知

**问题**: 在 NUMA 系统上，False Sharing 的影响可能更大。为什么？设计一个实验验证这一点。

---

## 思考题

### 10. 编译器优化

**问题**: 编译器优化级别 (-O0, -O1, -O2, -O3) 如何影响 False Sharing 测试结果？

---

### 11. 硬件预取

**问题**: 硬件预取器如何影响 False Sharing 的检测？

---

### 12. 实际应用

**问题**: 在哪些实际场景中需要考虑 False Sharing？
- 内核数据结构
- 高性能计算
- 游戏引擎
- 数据库系统

举例说明。