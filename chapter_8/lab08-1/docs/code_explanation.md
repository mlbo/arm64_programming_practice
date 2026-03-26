# 代码说明

## main 函数

```asm
main:
    stp x29, x30, [sp, -16]!   // 建立栈帧
    ldr x0, =my_data           // 加载数组地址
    ldr x1, my_data_count      // 加载元素个数 (6)
    add x4, x0, #40            // 计算结束地址 (数组大小 48 字节)
    mov x3, xzr                // 初始化最大值为 0
1:
    ldr x2, [x0], #8           // 加载元素，指针后移
    cmp x2, x3                 // 比较
    csel x3, x2, x3, hi        // 选择较大值
    cmp x0, x4                 // 检查是否到达末尾
    b.ls 1b                    // 未到达则继续
    ldr x0, =print_data
    mov x1, x3
    bl printf                  // 打印最大值
    mov x0, #0
    ldp x29, x30, [sp], 16     // 恢复栈帧
    ret
```

## 数据段

```asm
.section .data
.align 3
my_data:
    .quad 1, 2, 5, 8, 10, 12   // 数据数组
my_data_count:
    .quad 6                    // 元素个数
print_data:
    .string "big data: %d\n"   // 格式字符串
```