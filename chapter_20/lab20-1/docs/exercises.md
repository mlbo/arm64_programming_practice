# 练习题

## 基础题

### 1. LDXR/STXR 理解

**问题**: 解释 LDXR 和 STXR 的工作原理。为什么 STXR 可能失败？

---

### 2. 原子操作基础

**问题**: 以下操作是否是原子的？

```cpp
int x = 0;
x++;           // 是否原子？
x = x + 1;     // 是否原子？
x = 42;        // 是否原子？
```

---

### 3. CAS 使用

**问题**: 使用 CAS 实现一个原子递增函数：

```cpp
auto atomic_increment(Atomic<u32>& counter) -> u32;
```

---

## 进阶题

### 4. 无锁栈

**问题**: 使用 CAS 实现一个无锁栈的 push 操作：

```cpp
struct Node {
    int data;
    Node* next;
};

class LockFreeStack {
public:
    void push(int value);
    bool pop(int& value);

private:
    Atomic<Node*> head_{nullptr};
};
```

---

### 5. 自旋锁改进

**问题**: 改进 SpinLock 实现，添加以下功能：
- 公平性 (避免饥饿)
- 可重入性
- 超时机制

---

### 6. 内存序实践

**问题**: 以下代码应该使用什么内存序？

```cpp
// 生产者
data = compute();
flag.store(1, ???);  // ???

// 消费者
while (flag.load(???) == 0);  // ???
use(data);
```

---

## 挑战题

### 7. 无锁队列

**问题**: 实现一个 MPSC (多生产者单消费者) 无锁队列。

---

### 8. LSE 扩展检测

**问题**: 编写代码检测 CPU 是否支持 LSE 扩展，并在支持时使用 LSE 指令。

```cpp
bool has_lse();
```

**提示**: 读取 ID_AA64ISAR0_EL1 寄存器。

---

### 9. 原子操作性能对比

**问题**: 对比以下原子操作的吞吐量：
- LDXR/STXR 循环
- LSE 原子指令
- 带锁的互斥量

编写基准测试程序。

---

## 思考题

### 10. ABA 问题

**问题**: 什么是 ABA 问题？如何解决？

```cpp
// 场景:
// 线程1: read A, prepare CAS(A->C)
// 线程2: A -> B -> A
// 线程1: CAS succeeds (但中间有变化!)
```

---

### 11. 伪共享影响

**问题**: 原子操作如何受到 False Sharing 的影响？如何优化？

---

### 12. RCU vs 原子操作

**问题**: 比较 RCU (Read-Copy-Update) 和原子操作的使用场景。

---

### 13. 内存屏障开销

**问题**: 分析以下代码的性能差异：

```cpp
// 版本 1
dmb();
x = 1;
dmb();

// 版本 2
stlr x, [addr]  // Release store
```