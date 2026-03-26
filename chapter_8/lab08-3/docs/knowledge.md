# 知识点：汇编调用 C

## 1. BL 指令

```asm
bl function_name    // 跳转到函数，保存返回地址到 x30
```

## 2. 参数准备

调用前将参数放入 x0-x7：

```asm
mov x0, #10         // 第 1 个参数
mov x1, #20         // 第 2 个参数
mov x2, #30         // 第 3 个参数
bl my_c_function    // 调用
// x0 = 返回值
```

## 3. 保存 x30

如果汇编中调用了其他函数，必须保存 x30：

```asm
main:
    stp x29, x30, [sp, -16]!   // 保存
    // ... 调用其他函数
    ldp x29, x30, [sp], 16     // 恢复
    ret
```

## 4. 调用 printf

```asm
ldr x0, =format_string    // 格式字符串地址
mov x1, #100              // 第 1 个参数
bl printf
```

## 5. 符号声明

C 函数默认是全局符号，可以直接调用：

```asm
// 不需要额外声明
bl compare_data
```