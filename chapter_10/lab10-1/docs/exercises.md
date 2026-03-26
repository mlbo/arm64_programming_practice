# 练习题

## 基础题

### 1. 简单加法

使用内联汇编实现加法函数：

```cpp
auto add_asm(int a, int b) -> int {
    int result;
    // TODO: 使用 asm 实现 result = a + b
    return result;
}
```

### 2. 原子递增

使用内联汇编实现原子递增：

```cpp
void atomic_inc(int* ptr) {
    // TODO: 使用 ldaddal 指令
}
```

### 3. 读取系统寄存器

实现读取 CurrentEL 寄存器的函数：

```cpp
auto get_current_el() -> u64 {
    u64 el;
    // TODO: 使用 mrs 指令读取 CurrentEL
    return el;
}
```

## 进阶题

### 4. 内存屏障

使用内联汇编实现各种内存屏障：

```cpp
void dmb();  // Data Memory Barrier
void dsb();  // Data Synchronization Barrier
void isb();  // Instruction Synchronization Barrier
```

### 5. 自旋锁

使用内联汇编实现简单的自旋锁：

```cpp
class SpinLock {
public:
    void lock();
    void unlock();
private:
    volatile u32 locked_ = 0;
};
```

## 挑战题

### 6. 位操作宏

实现一组位操作的内联汇编函数：

```cpp
void atomic_or(u32* addr, u32 mask);
void atomic_and(u32* addr, u32 mask);
void atomic_xor(u32* addr, u32 mask);
```