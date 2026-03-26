# 代码说明

## main.S

```asm
.section .data
.align 3
print_data:
    .string "big data: %d\n"

.section .text
.globl main
main:
    stp x29, x30, [sp, -16]!   // 保存 FP 和 LR
    mov x0, #6                 // 参数 a = 6
    mov x1, #5                 // 参数 b = 5
    bl compare_data            // 调用 C 函数
    mov x1, x0                 // 保存返回值
    ldr x0, =print_data        // 格式字符串
    bl printf                  // 打印
    mov x0, #0                 // 返回 0
    ldp x29, x30, [sp], 16     // 恢复 FP 和 LR
    ret
```

## compare.c

```c
int compare_data(int a, int b) {
    return (a >= b) ? a : b;   // 返回较大值
}
```

## 执行流程

1. main() 设置 x0=6, x1=5
2. 调用 compare_data
3. compare_data 返回 6 (因为 6 >= 5)
4. 打印 "big data: 6"