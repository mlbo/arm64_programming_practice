# 练习题

## 基础题

### 1. 实现原子加法

使用 LDXR/STXR 实现原子加法：

```cpp
void atomic_add(u32* addr, u32 delta);
```

### 2. 实现原子交换

```cpp
auto atomic_swap(u32* addr, u32 new_val) -> u32;
```

## 进阶题

### 3. 实现比较交换 (CAS)

```cpp
auto compare_and_swap(u32* addr, u32 expected, u32 new_val) -> bool;
```

### 4. 实现自旋锁

使用 LDXR/STXR 实现自旋锁：

```cpp
class SpinLock {
    void lock();
    void unlock();
};
```

## 挑战题

### 5. 实现无锁队列

使用 CAS 实现无锁队列的 push 和 pop 操作。