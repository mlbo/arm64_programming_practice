# 代码说明

## bl_test 函数

使用临时寄存器保存 x30：

```asm
bl_test:
    mov x6, x30           // 保存返回地址到临时寄存器
    mov x0, 1
    mov x1, 3
    bl csel_test
    mov x30, x6           // 恢复返回地址
    ret
```

## 更好的实现：使用栈

```asm
bl_test_stack:
    stp x29, x30, [sp, #-16]!   // 保存 FP 和 LR 到栈
    mov x29, sp                  // 设置 FP

    mov x0, 1
    mov x1, 3
    bl csel_test

    ldp x29, x30, [sp], #16      // 从栈恢复 FP 和 LR
    ret
```

## 栈帧示例

```asm
func_with_locals:
    stp x29, x30, [sp, #-32]!   // 分配 32 字节，保存 FP/LR
    mov x29, sp

    // 局部变量位于 [x29, #-8] 和 [x29, #-16]
    mov x0, #123
    str x0, [x29, #-8]           // 存储局部变量 1
    mov x1, #456
    str x1, [x29, #-16]          // 存储局部变量 2

    ldp x29, x30, [sp], #32      // 恢复并释放栈空间
    ret
```