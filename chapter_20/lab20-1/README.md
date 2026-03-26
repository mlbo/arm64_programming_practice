# chapter_20/lab20-1: 原子操作

## 概述

本实验演示 ARM64 的原子操作实现，包括 LDXR/STXR 指令和 CAS (Compare and Swap) 操作。

## LDXR/STXR 机制

### 独占监视器 (Exclusive Monitor)

ARM64 使用独占监视器实现原子操作：

```
LDXR: 加载并标记监视器
      ┌─────────────────┐
      │ 独占监视器开启   │
      └─────────────────┘
              │
              ▼
      其他线程写入?
       /        \
      是         否
       │          │
       ▼          ▼
   监视器清除   监视器仍有效
       │          │
       ▼          ▼
   STXR 失败   STXR 成功
```

### LL/SC 模式

Load-Linked / Store-Conditional 是一种常见的原子操作模式：

1. **LDXR (Load Exclusive)**: 加载数据，设置独占标记
2. **STXR (Store Exclusive)**: 条件存储，如果独占标记仍有效则成功

## 编译与运行

```bash
make
./atomic_demo
```

## 预期输出

```
=== ARM64 Atomic Operations Demo ===

Running basic operation tests...
Running multithreaded tests...

=== Atomic Operations Test Results ===

1. Load/Store:      PASS
2. Exchange:        PASS
3. Compare-Exchange: PASS
4. Fetch-Add:       PASS
5. Multithreaded:   PASS
6. Spinlock:        PASS

=== Test Summary ===
Passed: 6/6
```

## 核心 API

### Atomic<T> 类

```cpp
arm64lab::Atomic<u32> counter(0);

// 基本操作
counter.load();                // 加载
counter.store(42);             // 存储
counter.exchange(100);         // 交换
counter.fetch_add(10);         // 原子加

// CAS 操作
u32 expected = 100;
counter.compare_exchange_strong(expected, 200);
```

### SpinLock 实现

```cpp
arm64lab::SpinLock lock;
{
    arm64lab::LockGuard guard(lock);
    // 临界区
}
```

## 相关章节

- 《ARM64体系结构：编程与实践》第 20 章
- 前置知识: 内存屏障 (chapter_18)