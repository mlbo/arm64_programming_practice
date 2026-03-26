# Lab 06-3: 原子操作 (有 Bug)

## 实验目的

1. 理解 LDXR/STXR 独占加载/存储
2. 认识原子操作的正确实现方式
3. 学习调试并发问题

## Bug 说明

```asm
my_atomic_write:
    adr x6, my_data
1:
    ldxr x2, [x6]      // 独占加载
    mov  x2, x0        // Bug: 覆盖了加载的值！
    stxr w3, x2, [x6]
    cbnz w3, 1b
    mov x0, x2
    ret
```

**问题**: `mov x2, x0` 覆盖了 `ldxr` 加载的值，导致 `stxr` 无法正确执行。

## 预期行为

原子写入可能失败，因为 `ldxr` 和 `stxr` 的配对关系被破坏。

## 修复方案

参见 lab06-3-fixed 的正确实现。