# 代码说明

## bl_test 函数 (正确实现)

本 lab 使用临时寄存器方式保存 x30：

```asm
.global bl_test
bl_test:
    mov x6, x30           // 保存返回地址
    mov x0, 1
    mov x1, 3
    bl csel_test
    mov x30, x6           // 恢复返回地址
    ret
```

## 执行流程

1. 调用 `bl_test` 时，x30 = 调用者的返回地址
2. `mov x6, x30` 将返回地址保存到 x6
3. `bl csel_test` 调用 csel_test，x30 被修改
4. `mov x30, x6` 恢复原来的返回地址
5. `ret` 正确返回调用者

## 对比 lab05-3

lab05-3 没有 `mov x6, x30` 和 `mov x30, x6`，导致返回地址丢失。

## 更好的实现

使用栈保存是更通用的方法：

```asm
bl_test:
    stp x29, x30, [sp, #-16]!
    mov x0, 1
    mov x1, 3
    bl csel_test
    ldp x29, x30, [sp], #16
    ret
```