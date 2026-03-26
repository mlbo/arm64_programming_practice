# Lab 05-2: CSEL 条件选择指令

## 实验目的

1. 掌握 CSEL 条件选择指令
2. 理解条件执行的概念
3. 学习条件后缀的使用

## 实验内容

1. **CSEL 指令**: 根据条件选择两个值之一
2. **条件后缀**: eq, ne, hi, lo 等

## 核心代码

```asm
csel_test:
    cmp x0, 0
    sub x2, x1, 1    // x2 = x1 - 1
    add x3, x1, 2    // x3 = x1 + 2
    csel x0, x3, x2, eq  // 如果相等，x0 = x3，否则 x0 = x2
    ret
```

## 构建与运行

```bash
make && make run
```