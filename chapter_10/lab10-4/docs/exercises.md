# 练习题

## 基础题

### 1. LSE 原子加法

使用 STADD 指令实现原子加法：

```cpp
auto atomic_fetch_add(u32* addr, u32 delta) -> u32;
```

### 2. LSE 原子或

使用 LDSET 指令实现原子或操作：

```cpp
auto atomic_fetch_or(u32* addr, u32 mask) -> u32;
```

## 进阶题

### 3. 实现自适应原子操作

编写代码检测 LSE 支持，并自动选择最优实现：

```cpp
void atomic_add(u32* addr, u32 delta) {
    if (has_lse()) {
        // 使用 LSE
    } else {
        // 使用 LL/SC
    }
}
```

### 4. 使用 CAS 实现无锁栈

```cpp
template<typename T>
class LockFreeStack {
    void push(T value);
    auto pop() -> T;
};
```

## 挑战题

### 5. 性能对比

编写测试代码比较 LSE 和 LL/SC 的性能差异。

### 6. 实现引用计数

使用 LSE 指令实现线程安全的引用计数智能指针。