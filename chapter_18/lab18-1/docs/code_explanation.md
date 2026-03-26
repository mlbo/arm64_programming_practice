# 代码说明

## 文件结构

```
chapter_18/lab18-1/
├── include/
│   ├── memory_model.hpp   # 内存屏障和工具
│   └── litmus_tests.hpp   # Litmus 测试类
├── src/
│   ├── litmus_tests.cpp   # 测试实现
│   └── main.cpp           # 主程序
└── Makefile
```

## 核心设计

### 1. 内存屏障封装

```cpp
// 数据内存屏障
inline void dmb() {
    asm volatile("dmb ish" ::: "memory");
}

// 存储屏障
inline void dmb_st() {
    asm volatile("dmb ishst" ::: "memory");
}

// 加载屏障
inline void dmb_ld() {
    asm volatile("dmb ishld" ::: "memory");
}
```

**设计要点**：
- 使用 inline 函数，零开销抽象
- `asm volatile` 确保不被优化掉
- `"memory"` clobber 防止编译器重排

### 2. Litmus 测试框架

```cpp
template<typename TestFunc>
auto run_litmus_test(TestFunc func, int iterations) -> LitmusResult {
    int failures = 0;
    for (int i = 0; i < iterations; ++i) {
        if (!func()) {
            ++failures;
        }
    }
    return {iterations, failures, (double)failures / iterations * 100.0};
}
```

**设计要点**：
- 模板函数支持任意测试类型
- 返回统计结果
- 失败率便于分析

### 3. Message Passing 测试

```cpp
class MessagePassingTest {
    auto run_once() -> bool;  // 运行一次测试

private:
    alignas(64) u64 data_;    // 缓存行对齐
    alignas(64) u64 flag_;
};
```

**内存布局**:
```
+----------+----------+
| data     | padding  |  <- 缓存行 1
+----------+----------+
| flag     | padding  |  <- 缓存行 2
+----------+----------+
```

### 4. 测试结果

```cpp
struct LitmusResult {
    int total_runs;      // 总运行次数
    int failures;        // 观察到重排次数
    double failure_rate; // 失败率
};
```

## 多线程实现

### 线程函数

```cpp
auto MessagePassingTest::thread0_func(void* arg) -> void* {
    auto* self = static_cast<MessagePassingTest*>(arg);
    self->data_ = 55;      // 写数据
    // 无屏障 - 可能重排
    self->flag_ = 1;       // 设置标志
    return nullptr;
}
```

### 结果检查

```cpp
auto MessagePassingTest::run_once() -> bool {
    // ...
    // 检查是否观察到重排: flag=1 但 data!=55
    return !(result_flag_ == 1 && result_data_ != 55);
}
```

## 屏障对比

### 无屏障版本

```cpp
// 可能观察到重排
self->data_ = 55;
self->flag_ = 1;
```

### 有屏障版本

```cpp
// 屏障阻止重排
self->data_ = 55;
dmb_st();  // 存储屏障
self->flag_ = 1;
```