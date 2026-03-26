# 练习题

## 基础题

### 1. 内存屏障基础

**问题**: 解释 dmb, dsb, isb 三种屏障的区别和使用场景。

---

### 2. Litmus 测试理解

**问题**: 在 Message Passing 测试中，为什么可能观察到 flag=1 但 data=0？

**提示**: 考虑存储缓冲区 (Store Buffer) 的影响。

---

### 3. 屏障代码分析

**问题**: 以下代码是否正确？如果不正确，如何修复？

```cpp
// 线程 0
data = 55;
flag = 1;

// 线程 1
while (flag == 0);
use(data);  // 是否能保证看到 data=55?
```

---

## 进阶题

### 4. 自旋锁实现

**问题**: 使用 LDXR/STXR 实现一个自旋锁，确保正确的内存序。

```cpp
class SpinLock {
public:
    void lock();
    void unlock();
private:
    u32 locked_ = 0;
};
```

---

### 5. 添加新的 Litmus 测试

**问题**: 实现 "2+2 Write" Litmus 测试：

```
P0: x = 1; r1 = y;
P1: y = 1; r2 = x;

检查: r1=0 && r2=0 是否可能？
```

---

### 6. herdtools7 使用

**问题**: 创建一个 .litmus 文件，使用 herdtools7 验证 Message Passing 测试。

---

## 挑战题

### 7. 原子操作实现

**问题**: 实现支持内存序的 Atomic<T> 类：

```cpp
template<typename T>
class Atomic {
public:
    auto load(MemoryOrder order) const -> T;
    void store(T val, MemoryOrder order);
    auto exchange(T val, MemoryOrder order) -> T;
};
```

---

### 8. SeqCst 实现

**问题**: 实现顺序一致性 (Sequential Consistency) 的内存访问：
- 所有线程看到相同的操作顺序
- 使用哪些 ARM64 指令？

---

### 9. 性能对比

**问题**: 对比以下同步方式的性能：
- dmb ish (完整屏障)
- dmb ishst (仅存储屏障)
- ldar/stlr (Acquire/Release)

编写基准测试程序。

---

## 思考题

### 10. 为什么 ARM64 使用弱内存模型？

**问题**: 弱内存模型相比强内存模型有什么优势？

**提示**: 考虑性能、功耗、缓存一致性流量。

---

### 11. 编译器屏障 vs 硬件屏障

**问题**: 解释以下代码的区别：

```cpp
// 代码 A
asm volatile("" ::: "memory");

// 代码 B
asm volatile("dmb ish" ::: "memory");
```

---

### 12. Linux 内核屏障

**问题**: 阅读 Linux 内核源码，找到 smp_wmb(), smp_rmb(), smp_mb() 在 ARM64 上的实现。

---

### 13. C++ 内存序

**问题**: 将以下 C++ 代码翻译为 ARM64 汇编：

```cpp
std::atomic<int> x(0);
x.store(1, std::memory_order_release);
int v = x.load(std::memory_order_acquire);
```