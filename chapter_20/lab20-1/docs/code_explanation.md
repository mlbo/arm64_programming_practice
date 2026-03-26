# 代码说明

## 文件结构

```
chapter_20/lab20-1/
├── include/
│   ├── atomic.hpp       # Atomic<T> 模板类
│   └── atomic_test.hpp  # 测试类
├── src/
│   ├── atomic_test.cpp  # 测试实现
│   └── main.cpp         # 主程序
└── Makefile
```

## 核心类设计

### 1. Atomic<T> 模板

```cpp
template<typename T>
class Atomic {
public:
    auto load(MemoryOrder order) const -> T;
    void store(T val, MemoryOrder order);
    auto exchange(T val, MemoryOrder order) -> T;
    auto compare_exchange_strong(T& expected, T desired, ...) -> bool;
    auto fetch_add(T delta, MemoryOrder order) -> T;

    auto operator++() -> T;
    auto operator--() -> T;

private:
    mutable T value_;
};
```

### 2. load 实现

```cpp
auto load(MemoryOrder order) const -> T {
    T result;
    switch (order) {
        case MemoryOrder::Relaxed:
            asm volatile("ldr %w0, [%1]"
                         : "=r"(result)
                         : "r"(&value_)
                         : "memory");
            break;
        case MemoryOrder::Acquire:
        case MemoryOrder::SeqCst:
            asm volatile("ldar %w0, [%1]"  // Acquire load
                         : "=r"(result)
                         : "r"(&value_)
                         : "memory");
            break;
    }
    return result;
}
```

### 3. store 实现

```cpp
void store(T val, MemoryOrder order) {
    switch (order) {
        case MemoryOrder::Relaxed:
            asm volatile("str %w0, [%1]"
                         :
                         : "r"(val), "r"(&value_)
                         : "memory");
            break;
        case MemoryOrder::Release:
        case MemoryOrder::SeqCst:
            asm volatile("stlr %w0, [%1]"  // Release store
                         :
                         : "r"(val), "r"(&value_)
                         : "memory");
            break;
    }
}
```

### 4. CAS 实现

```cpp
auto compare_exchange_strong(T& expected, T desired) -> bool {
    u32 success_flag;
    T old_val;

    asm volatile(
        "1: ldxr %w0, [%3]\n"      // 独占加载
        "   cmp %w0, %w4\n"        // 比较期望值
        "   b.ne 2f\n"             // 不匹配则跳转
        "   stxr %w1, %w5, [%3]\n" // 独占存储
        "   cbnz %w1, 1b\n"        // 失败则重试
        "   mov %w1, #1\n"         // 成功
        "   b 3f\n"
        "2: mov %w1, #0\n"         // 失败
        "3:"
        : "=&r"(old_val), "=&r"(success_flag)
        : "r"(&value_), "r"(expected), "r"(desired)
        : "memory", "cc");

    if (!success_flag) {
        expected = old_val;  // 更新期望值为实际值
    }
    return success_flag;
}
```

### 5. fetch_add 实现

```cpp
auto fetch_add(T delta, MemoryOrder) -> T {
    T old_val, new_val;
    u32 tmp;

    asm volatile(
        "1: ldxr %w0, [%3]\n"      // 独占加载
        "   add %w1, %w0, %w4\n"   // 加法
        "   stxr %w2, %w1, [%3]\n" // 独占存储
        "   cbnz %w2, 1b"          // 失败则重试
        : "=&r"(old_val), "=&r"(new_val), "=&r"(tmp)
        : "r"(&value_), "r"(delta)
        : "memory");

    return old_val;
}
```

## SpinLock 实现

### 加锁

```cpp
void lock() {
    while (locked_.exchange(1) != 0) {
        // 自旋等待
        while (locked_.load() != 0) {
            // 等待锁释放
        }
    }
}
```

### 解锁

```cpp
void unlock() {
    locked_.store(0);
}
```

### 优化版本

使用 CLREX 和 WFE 指令可以优化功耗:

```cpp
void lock() {
    do {
        while (locked_.load() != 0) {
            asm volatile("wfe");  // 低功耗等待
        }
    } while (locked_.exchange(1) != 0);
}
```

## 内存序选择

| 操作 | 推荐内存序 |
|------|------------|
| 简单读写 | Relaxed |
| 标志检查 | Acquire |
| 数据发布 | Release |
| 锁操作 | AcqRel |
| 复杂同步 | SeqCst |