# 代码说明

## 文件结构

```
chapter_16/lab16-2/
├── include/
│   ├── cache_aligned.hpp    # 缓存行对齐模板
│   └── perf_demo.hpp        # perf 分析演示类
├── src/
│   ├── perf_demo.cpp        # 演示实现
│   └── main.cpp             # 主程序
└── Makefile
```

## 核心类设计

### 1. SharedDataBad 结构体

```cpp
struct SharedDataBad {
    u64 counter1;  // 偏移 0
    u64 counter2;  // 偏移 8
    u64 counter3;  // 偏移 16
    u64 counter4;  // 偏移 24
};
```

**内存布局**:
```
+----------+----------+----------+----------+-----...
| counter1 | counter2 | counter3 | counter4 | (可能的其他数据)
+----------+----------+----------+----------+-----...
|<----------- 32 字节 ------------>|

全部在一个 64 字节缓存行内！
```

### 2. SharedDataGood 结构体

```cpp
struct SharedDataGood {
    alignas(64) u64 counter1;
    alignas(64) u64 counter2;
    alignas(64) u64 counter3;
    alignas(64) u64 counter4;
};
```

**内存布局**:
```
缓存行 0 (偏移 0x000):  counter1
缓存行 1 (偏移 0x040):  counter2
缓存行 2 (偏移 0x080):  counter3
缓存行 3 (偏移 0x0C0):  counter4
```

### 3. PerfDemo 类

```cpp
class PerfDemo {
public:
    auto run_bad() -> u64;   // 运行有问题的版本
    auto run_good() -> u64;  // 运行优化版本

private:
    SharedDataBad data_bad_;    // perf c2c 会检测到这里
    SharedDataGood data_good_;  // 无缓存行竞争
};
```

## 多线程实现

使用 4 个线程同时写入不同计数器：

```cpp
// 线程函数
auto thread_increment_bad(void* arg) -> void* {
    // 每个线程写入不同的计数器
    // 但所有计数器在同一缓存行
}
```

### perf c2c 检测原理

当 4 个线程同时写入 `data_bad_` 的不同字段时：
1. 线程 0 写 counter1 → 获取缓存行所有权
2. 线程 1 写 counter2 → 需要使线程 0 的缓存行失效
3. 缓存行在 4 个线程间频繁传递
4. perf c2c 检测到高 HITM 事件

## perf 集成

程序设计便于 perf 分析：

```cpp
// 长时间运行的测试循环
for (u64 i = 0; i < 100000000; ++i) {
    *counter += 1;
}
```

足够多的迭代确保 perf 能采样到足够的缓存事件。

## 输出格式

程序输出包含：
1. 执行时间对比
2. 计数器最终值 (验证正确性)
3. perf 命令提示

```
Test 1: With False Sharing (4 threads)
  Time: 7394 ms
  Counters: 100000000, 100000000, 100000000, 100000000
```