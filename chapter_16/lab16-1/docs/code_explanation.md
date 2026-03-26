# 代码说明

## 文件结构

```
chapter_16/lab16-1/
├── include/
│   ├── cache_aligned.hpp    # 缓存行对齐模板
│   └── false_sharing.hpp    # False Sharing 测试类
├── src/
│   ├── false_sharing.cpp    # 测试实现
│   └── main.cpp             # 主程序
└── Makefile
```

## 核心类设计

### 1. CacheLineAligned 模板

```cpp
template<typename T>
struct alignas(CACHE_LINE_SIZE) CacheLineAligned {
    T value;

    CacheLineAligned() : value{} {}
    explicit CacheLineAligned(T v) : value(v) {}

    operator T() const { return value; }
    auto operator=(T v) -> CacheLineAligned&;
};
```

**设计要点**：
- 使用 `alignas(64)` 强制 64 字节对齐
- 提供隐式转换，使用方便
- 零运行时开销

### 2. CounterBad vs CounterGood

```cpp
// 问题版本
struct CounterBad {
    u64 count1;  // 偏移 0
    u64 count2;  // 偏移 8 (同一缓存行!)
};

// 优化版本
struct CounterGood {
    alignas(64) u64 count1;  // 缓存行起始
    alignas(64) u64 count2;  // 下一缓存行起始
};
```

**内存布局对比**：
```
CounterBad:
+--------+--------+--------+--------+---...---+
| count1 | count2 |  unused ...                 |  <- 同一缓存行
+--------+--------+--------+--------+---...---+
偏移: 0      8

CounterGood:
+--------+----...----+ (缓存行 1)
| count1 | padding   |
+--------+----...----+
偏移: 0

+--------+----...----+ (缓存行 2)
| count2 | padding   |
+--------+----...----+
偏移: 64
```

### 3. FalseSharingTest 类

```cpp
class FalseSharingTest {
public:
    auto test_bad() -> u64;      // 测试 false sharing
    auto test_good() -> u64;     // 测试优化版本

private:
    CounterBad counter_bad_;     // 对比测试
    CounterGood counter_good_;
};
```

## 多线程实现

使用 pthread 创建两个线程：
- 线程 1：访问 count1
- 线程 2：访问 count2

关键点：
- 线程函数使用 `static` 成员函数
- 通过 `this` 指针访问测试数据
- 使用 `pthread_join` 等待线程完成

## 计时实现

```cpp
class Timer {
    auto elapsed_ms() const -> u64;  // 返回经过的毫秒数
};
```

使用 `clock_gettime(CLOCK_REALTIME)` 获取高精度时间。