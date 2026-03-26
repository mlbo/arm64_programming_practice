# 练习题

## 练习 1: 寻址模式

分析以下代码，写出每条指令执行后寄存器的值：

```asm
mov x1, 0x80000
mov x2, 0x12345678
str x2, [x1]
ldr x0, [x1, #8]!
```

问题：
1. 执行 `str x2, [x1]` 后，地址 0x80000 的值是多少？
2. 执行 `ldr x0, [x1, #8]!` 后，x1 的值是多少？

## 练习 2: 内存拷贝

修改 `my_memcpy_test` 函数，使其能够拷贝任意字节数（不仅仅是 8 的倍数）。

提示：考虑使用字节拷贝（ldrb/strb）处理剩余字节。

## 练习 3: 条件选择

编写一个汇编函数，实现以下功能：

```c
// 如果 a > b，返回 a - b
// 如果 a <= b，返回 b - a
long abs_diff(long a, long b);
```

要求：使用 CMP 和 CSEL 指令。

## 练习 4: BL 与返回地址

在 lab05-3 中，`bl_test` 函数为什么会跑飞？如何修复？

分析代码：
```asm
.global bl_test
bl_test:
    mov x0, 1
    mov x1, 3
    bl csel_test      // BL 会修改 x30
    ret               // 返回地址已丢失！
```

## 练习 5: 标志位分析

分析以下代码执行后 NZCV 标志位的值：

```asm
mov x0, #0
ldr x1, =0x7fffffffffffffff
adds x0, x1, x1
```

问题：
1. N 标志位的值是多少？为什么？
2. Z 标志位的值是多少？为什么？
3. C 标志位的值是多少？为什么？
4. V 标志位的值是多少？为什么？

## 练习 6: 编程实现

使用内联汇编实现以下 C++ 函数：

```cpp
// 返回较大值
inline auto max_value(long a, long b) -> long {
    // TODO: 使用 CMP 和 CSEL 实现
}
```