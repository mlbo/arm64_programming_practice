# Lab10-3: LDXR/STXR 原子操作

## 实验目的

学习 ARM64 的 LL/SC (Load-Link/Store-Conditional) 原子操作模式。

## 实验内容

- LDXR/STXR 指令原理
- 自旋锁实现
- 原子计数器

## 知识点

### LL/SC 模式

```asm
1: ldxr x0, [x1]     ; 独占加载
   add  x0, x0, #1   ; 修改
   stxr w2, x0, [x1] ; 独占存储
   cbnz w2, 1b       ; 失败则重试
```

## 运行方法

```bash
make && make run
```