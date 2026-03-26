# 原子操作知识点

## 1. 为什么需要原子操作？

### 并发问题示例

```cpp
// 非原子递增
counter++;  // 实际是: load -> add -> store

// 多线程下的问题:
// 线程1: load (counter=0)
// 线程2: load (counter=0)  <- 都读到 0
// 线程1: store (counter=1)
// 线程2: store (counter=1)  <- 覆盖！应该是 2
```

### 原子操作保证

- **不可分割**: 整个操作要么完成，要么未开始
- **可见性**: 其他线程看到完整的结果
- **顺序性**: 编译器和 CPU 不重排

## 2. ARM64 原子指令

### LDXR/STXR (ARMv8.0)

```
LDXR Xt, [Xn]     ; 独占加载
STXR Ws, Xt, [Xn] ; 独占存储, Ws=0 成功, Ws=1 失败
```

**特点**:
- 使用独占监视器
- LL/SC 模式
- 可能失败需要重试

### LDAR/STLR (ARMv8.0 Acquire/Release)

```
LDAR Xt, [Xn]  ; Acquire 加载
STLR Xt, [Xn]  ; Release 存储
```

**特点**:
- 自动内存屏障
- 无需显式 dmb
- 更高效

### LSE 扩展 (ARMv8.1+)

```
LDADD Xt, Xt, [Xn]  ; 原子加
CAS   Xt, Xt, [Xn]  ; 原子比较交换
SWP   Xt, Xt, [Xn]  ; 原子交换
```

**特点**:
- 单条指令完成
- 更高效
- 无需循环重试

## 3. 独占监视器

### 三层监视器

| 监视器类型 | 范围 | 说明 |
|------------|------|------|
| 内部 | 单核 | L1 缓存一致性 |
| 外部 | 多核芯片 | 芯片内一致性 |
| 系统 | 整个系统 | 跨芯片一致性 |

### 监视器操作

```
LDXR: 设置本地监视器
STXR: 检查并清除监视器
CLREX: 显式清除监视器
DMB: 可能影响监视器
```

## 4. 内存序

### Relaxed

- 无顺序保证
- 仅保证原子性

```cpp
// Relaxed 操作可能被重排
a.store(1, Relaxed);
b.store(2, Relaxed);  // 可能先执行
```

### Acquire

- 阻止后续读写重排到前面
- 用于读取共享数据

```cpp
// Acquire 保证之后操作不重排
if (flag.load(Acquire)) {
    use(data);  // 不会在 load 之前执行
}
```

### Release

- 阻止之前读写重排到后面
- 用于发布数据

```cpp
// Release 保证之前操作不重排
data = 42;
flag.store(1, Release);  // data 写入不会在之后
```

### SeqCst

- 最强的顺序保证
- 全局一致的操作顺序

## 5. 常见原子操作

### Compare and Swap (CAS)

```cpp
bool cas(T& expected, T desired) {
    T old = load();  // 独占加载
    if (old != expected) {
        expected = old;
        return false;
    }
    return store(desired);  // 独占存储
}
```

### Fetch and Add

```cpp
T fetch_add(T delta) {
    loop:
        T old = LDXR;
        T new = old + delta;
        if (STXR failed) goto loop;
        return old;
}
```

## 6. ABA 问题

### 问题场景

```
线程1: load (A) -> 准备 CAS -> 被抢占
线程2: A -> B -> A
线程1: CAS 成功 (但中间发生了变化!)
```

### 解决方案

1. 双宽度 CAS (CAS2/DWCAS)
2. 版本号/标记位
3. Hazard Pointers