# 知识点：C 调用汇编

## 1. extern 声明

在 C 中调用汇编函数需要先声明：

```c
extern int compare_data(int a, int b);
extern void my_asm_function(void);
```

## 2. 符号命名

汇编中的符号名必须与 C 中声明的一致：

```asm
.globl compare_data    // 必须匹配 C 中的函数名
compare_data:
    // ...
    ret
```

## 3. 参数传递

| C 参数 | 汇编寄存器 |
|--------|-----------|
| 第 1 个参数 | x0 |
| 第 2 个参数 | x1 |
| 第 3 个参数 | x2 |
| ... | ... |
| 返回值 | x0 |

## 4. 保存寄存器

如果汇编函数使用了 x19-x28，必须保存和恢复：

```asm
func:
    stp x19, x20, [sp, #-16]!
    // 使用 x19, x20
    ldp x19, x20, [sp], #16
    ret
```