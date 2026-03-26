# 练习题

## 练习 1: 分析 Bug

使用 GDB 单步调试 lab06-3，观察 `ldxr` 和 `stxr` 的行为。

问题：
1. `ldxr` 后 x2 的值是多少？
2. `mov x2, x0` 后 x2 的值是多少？
3. 这个 Bug 会导致什么问题？

## 练习 2: 实现原子交换

实现一个原子交换函数：

```asm
// 原子交换 *addr 和 new_val，返回旧值
atomic_swap:
    // x0 = addr, x1 = new_val
    // 返回 x0 = old_val
```

## 练习 3: 实现原子加法

实现一个原子加法函数：

```asm
// 原子将 delta 加到 *addr，返回旧值
atomic_fetch_add:
    // x0 = addr, x1 = delta
    // 返回 x0 = old_val
```

## 练习 4: 多核测试

在多核环境中测试原子操作，观察竞争条件。

## 练习 5: CLREX 指令

CLREX 指令的作用是什么？什么时候需要使用？