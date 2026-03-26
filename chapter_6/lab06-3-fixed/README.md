# Lab 06-3-Fixed: 原子操作 (正确实现)

## 实验目的

1. 理解 LDXR/STXR 的正确使用方式
2. 学习原子操作的实现模式
3. 掌握内存屏障的使用

## 正确实现

```asm
my_atomic_write:
    adr x6, my_data
1:
    ldxr x2, [x6]      // 独占加载
    mov  x2, x0        // 设置新值
    stxr w3, x2, [x6]  // 独占存储
    cbnz w3, 1b        // 失败则重试
    mov x0, x2
    ret
```

**注意**: 这个实现仍然有问题（见知识点说明），正确的原子写入应该使用不同的寄存器。

## 构建与运行

```bash
make && make run
```