# Lab 08-1: 纯汇编实现最大值查找

## 实验目的

1. 理解完整的汇编程序结构
2. 学习数据段和代码段的使用
3. 掌握循环和条件选择

## 核心代码

```asm
main:
    stp x29, x30, [sp, -16]!   // 保存 FP 和 LR
    ldr x0, =my_data           // 加载数组地址
    ldr x1, my_data_count      // 加载数组长度
    add x4, x0, #40            // 计算结束地址
    mov x3, xzr                // 初始化最大值
1:
    ldr x2, [x0], #8           // 加载元素
    cmp x2, x3
    csel x3, x2, x3, hi        // 选择较大值
    cmp x0, x4
    b.ls 1b                    // 循环
    // ... 输出结果
    ldp x29, x30, [sp], 16
    ret
```

## 构建与运行

```bash
make && ./test
```