# 代码说明

## my_atomic_write 函数 (有 Bug)

```asm
my_atomic_write:
    adr x6, my_data
1:
    ldxr x2, [x6]      // 独占加载 my_data 到 x2
    mov  x2, x0        // Bug: 覆盖 x2！
    stxr w3, x2, [x6]  // 尝试存储 x2 到 my_data
    cbnz w3, 1b
    mov x0, x2
    ret
```

## 问题分析

1. `ldxr x2, [x6]` 正确加载了 my_data 的值到 x2
2. `mov x2, x0` 立即覆盖了 x2，使其等于参数值
3. `stxr w3, x2, [x6]` 存储的是参数值，而不是修改后的值

## 正确实现

```asm
my_atomic_write_fixed:
    adr x6, my_data
1:
    ldxr x2, [x6]
    // 这里应该对 x2 进行操作
    stxr w3, x2, [x6]
    cbnz w3, 1b
    mov x0, x2
    ret
```